;; For now this is intended to be ran as a repl
;; Mostly because I don't dislike cider-repl as much as I dislike what SLIME does
(ns clj-propertylisp.core
  (:require [hawk.core :as hawk]
            [clj-time.core :as t]
            [clj-time.format :as tf]
            [clojure.core.async :refer [go go-loop <! >! chan]]
            [clojure.string :as str]
            [clojure.java.io :as io]))

(def input-dir (atom nil))
(def output-dir (atom nil))
(def compilation-queue (chan))

(defn getContainerDirPath [path]
  (io/file (.getParent (if (string? path)
                         (io/file path)
                         path))))

(defn push-compilation! [content]
  (>! compilation-queue content))

(defn handler [ctx {:keys [kind file]}]                             
  (when (and (not= kind :delete)
             (not (re-find #"#" (.getName file))))
    (let [parent-dir (getContainerDirPath file)
          compilation-set (->> parent-dir
                               file-seq
                               (filter (fn [f]
                                         (println "ähvä o " (class f))
                                         (let [name (.getName f)]
                                           (and (not (re-find #"#" name))
                                                (re-find #"\.def$" name)))))
                               
                               (map (fn [f]
                                      [(.getName f) (slurp f)]))
                               (into {}))]
      (push-compilation! compilation-set))))

(defn start-compilation-d!
  ([]
   (if (and @input-dir
            @output-dir)
     (start-compilation-d! @input-dir
                           @output-dir)
     (println "No either input or output dir set")))
  ([input-dir output-dir]
   (hawk/watch! [{:paths [input-dir]
                  :handler #'handler}])))


;; slurp every *.def file in input-dir
;; and create a compilation set of them
;; and push THAT to the *compilation-queue*


#_(start-compilation-d! "/home/feuer2/Dropbox/qt-test/src/lisp-defs"
                      "/home/feuer2/Dropbox/qt-test/src/generated")

(defn typesymbol->str [type]
  (-> type
      name
      (str/replace #"__" "::")))

(def test-data '(defcppclass map ("<root.h>" "<layer.h>" "<vector>" "<string>")
                  (declare-class "root" "layer")
                  (public
                   (fields
                    (std__vector<Layer*>* layers nullptr))
                   (properties
                    (std__string name "Map 1"))
                   (functions
                    (void parent (root* p)))
                   (functions
                    (int width (void ))
                    (int height (void))
                    (root* parent (void))))
                  
                  (private
                   (fields
                    (std__vector<Layer*>* layers nullptr))
                   (properties
                    (std__string name "Map 1"))
                   (functions
                    (void parent (root* p)))
                   (functions
                    (int width (void ))
                    (int height (void))
                    (root* parent (void))))))
(def another-test '(defcppclass layer ("<tile.h>" "<string>" "<vector>" "<map.h>")
                     (declare-class "map")
                     (public
                      (fields
                       (std__vector<std__vector<tile>>* tiles nullptr))
                      (properties
                       (std__string name ""))
                      (functions
                       (int getWidth (void))
                       (int getHeight (void)))
                      (functions
                       (void set_parent (map* p)))
                      (functions
                       (map* parent (void))))
                     (protected
                      (fields
                       (int W 0)
                       (int H 0)
                       (map* p nullptr)))) )

(def test-set [test-data another-test])

(defn tokenize-type [privacy-level [f & forms]]
  (map (fn [form]
         {:form form
          :privacy privacy-level
          :type f})
       forms))

(defn tokenize-privacy-level [[f & forms]]
  (map (partial tokenize-type f) forms))

(defn pick-declared-classes [def]
  (->> def
       (filter seq?)
       (filter (comp (partial = 'declare-class) first))
       (mapcat (partial drop 1))))

(defn tokenize [[_ class-name include-list :as def]]
  {:class-name class-name
   :includes include-list
   :declared-classes (pick-declared-classes def)
   :forms (->> def
               (filter seq?)
               (filter (fn [[s & lol]]
                         (contains? #{'public 'private 'protected} s)))

               (mapcat tokenize-privacy-level))})

;; (tokenize test-data)
;; {:class-name map,
;;  :includes ("<root.h>" "<layer.h>" "<vector>" "<string>"),
;;  :declared-classes ("root" "layer"),
;;  :forms
;;  (({:form (std__vector<Layer*>* layers nullptr),
;;     :privacy public,
;;     :type fields})
;;   ({:form (std__string name "Map 1"),
;;     :privacy public,
;;     :type properties})
;;   ({:form (void parent (root* p)), :privacy public, :type functions})
;;   ({:form (int width (void)), :privacy public, :type functions}
;;    {:form (int height (void)), :privacy public, :type functions}
;;    {:form (root* parent (void)), :privacy public, :type functions})
;;   ({:form (std__vector<Layer*>* layers nullptr),
;;     :privacy private,
;;     :type fields})
;;   ({:form (std__string name "Map 1"),
;;     :privacy private,
;;     :type properties})
;;   ({:form (void parent (root* p)),
;;     :privacy private,
;;     :type functions})
;;   ({:form (int width (void)), :privacy private, :type functions}
;;    {:form (int height (void)), :privacy private, :type functions}
;;    {:form (root* parent (void)), :privacy private, :type functions}))}

(defn get-stuff [type tokenized-forms]
  (->> tokenized-forms
       :forms
       flatten
       (filter (comp (partial = type) :type))))

(def get-fns (partial #'get-stuff 'functions))
(def get-properties (partial #'get-stuff 'properties))
(def get-fields (partial #'get-stuff 'fields))

(defn get-includes-of-set [class-set]
  (->> class-set
       (map :includes)
       flatten
       (apply hash-set)))

(defn make-get-sets-of-propertierbase [props]
  (->> props
       (map (comp first :form))
       set))

(defn get-set-strs [types]
  (->> types
       (map typesymbol->str)
       (map (fn [type]
              (format "virtual void set(const char *propertyname, %s value);
virtual %s get(const char *propertyname, bool *success, %s type_helper);\n" type type type)))
       (str/join "")))
  

(defn make-propertier-base [tokenized-classes]
  (let [props (->> tokenized-classes
                   (map get-properties)
                   flatten)
        _ (assert (not (empty? props)))
        header (format "#ifndef propertierbasee
#define propertierbasee
//// generated at %s

%s

class Propertierbase 
{
public:
Propertierbase ();
virtual ~Propertierbase ();
  virtual const char* type_name(const char* propertyname) = 0;
  virtual const char** names() = 0;
  virtual const char* type_identifier() = 0;
%s

};
#endif"
                       ;; timestamp
                       (tf/unparse (tf/formatters :date-time)  (t/now))
                       ;; relevant includes
                       (->> (get-includes-of-set tokenized-classes)
                            (map #(str "#include" % ""))
                            (str/join "\n"))
                       ;; get-set-definitions
                       (-> props
                           make-get-sets-of-propertierbase
                           get-set-strs)
                       )
        ]
    header))

#_(make-propertier-base (map tokenize test-set))




;; (defn start-compiler-backend! []
;;   (go-loop [compilation-set (<! compilation-queue)]

;;     (->> compilation-set
;;          (mapv (comp tokenize read-line second))
;;          (make-propertier-base))
    
;;     (doseq [[file-name content] compilation-set
;;             :let [content-as-lisp (read-line content)
;;                   tokenized (tokenize content-as-lisp)]]
      

                  
