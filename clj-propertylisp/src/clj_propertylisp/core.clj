;; For now this is intended to be ran as a repl
;; Mostly because I don't dislike cider-repl as much as I dislike what SLIME does
(ns clj-propertylisp.core
  (:require [hawk.core :as hawk]
            [clj-time.core :as t]
            [clj-time.format :as tf]
            [potpuri.core :refer [map-vals map-keys]]
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
  (go
    (>! compilation-queue content)))

(defn handler [ctx {:keys [kind file]}]
  (when (and (not= kind :delete)
             (not (re-find #"#" (.getName file))))
    (let [parent-dir (getContainerDirPath file)
          compilation-set (->> parent-dir
                               file-seq
                               (filter (fn [f]
                                         (let [name (.getName f)]
                                           (and (not (re-find #"#" name))
                                                (re-find #"\.def$" name)))))
                               
                               (map (fn [f]
                                      [(.getName f) (slurp f)]))
                               (into {}))]
      (push-compilation! compilation-set))))

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
                    (int name "Map 1"))
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
                       (std__string name "Map 1")
                       (std__vector<std__string>* names nullptr))
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

               (mapcat tokenize-privacy-level)
               (reduce concat))})
   

;; (tokenize test-data)
;; {:class-name map,
;;  :includes ("<root.h>" "<layer.h>" "<vector>" "<string>"),
;;  :declared-classes ("root" "layer"),
;;  :forms
;;  ({:form (std__vector<Layer*>* layers nullptr),
;;    :privacy public,
;;    :type fields}
;;   {:form (std__string name "Map 1"),
;;    :privacy public,
;;    :type properties}
;;   {:form (void parent (root* p)), :privacy public, :type functions}
;;   {:form (int width (void)), :privacy public, :type functions}
;;   {:form (int height (void)), :privacy public, :type functions}
;;   {:form (root* parent (void)), :privacy public, :type functions}
;;   {:form (std__vector<Layer*>* layers nullptr),
;;    :privacy private,
;;    :type fields}
;;   {:form (int name "Map 1"), :privacy private, :type properties}
;;   {:form (void parent (root* p)), :privacy private, :type functions}
;;   {:form (int width (void)), :privacy private, :type functions}
;;   {:form (int height (void)), :privacy private, :type functions}
;;   {:form (root* parent (void)), :privacy private, :type functions})}

(defn get-stuff [type tokenized-forms]
  (->> tokenized-forms
       :forms
       flatten
       (filter (comp (partial = type) :type))))

(def get-fns (partial #'get-stuff 'functions))
(def get-properties (partial #'get-stuff 'properties))
(def get-fields (partial #'get-stuff 'fields))

(defn get-includes-of-set [class-set]
  (let [compiled-files (->> class-set
                            (map (comp #(str "<" % ">")
                                       #(str/replace % #".def" ".h")
                                       :filename))
                            set)]
    (->> class-set
         (map :includes)
         flatten
         (filter (complement (partial contains? compiled-files)))
         (apply hash-set))))

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

(defn get-set-strs-implementation [types]
  (->> types
       (map typesymbol->str)
       (map (fn [type]
              (format "void Propertierbase::set(const char *propertyname, %s value) 
{

}

%s Propertierbase::get(const char *propertyname, bool *success, %s type_helper)
{

}\n" type type type)))
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
  virtual int property_count() = 0;
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
        cpp (format "#include<propertierbase.h>
Propertierbase::Propertierbase() 
{

}

Propertierbase::~Propertierbase() 
{

}

%s"
                    (-> props
                        make-get-sets-of-propertierbase
                        get-set-strs-implementation))]
    {:header header
     :implementation cpp}))


(defn codegen [{:keys [forms class-name declared-classes includes filename] :as tokenized-class}]
  (let [props (get-properties tokenized-class)
        props-by-types (->> props
                            (group-by (comp first :form))
                            (map-keys typesymbol->str))
        
        set-contents (->> props-by-types
                          (map (fn [[type props]]
                                   (str "virtual void set(const char* propertyname, " type " value) {
" (->> props
       (map (fn [{:keys [form]}]
              (let [[prop-type prop-name _] form
                    prop-name ((comp str/capitalize name) prop-name)]
                (str
                 "if(strcmp(propertyname, \"" prop-name "\") == 0) { " prop-name"_field = value; return; }"))))
       (str/join "\n"))
                                        " }")))
                          (str/join "\n"))
        get-contents (->> props-by-types
                          (map (fn [[type props]]
                                 (str "virtual " type " get(const char* propertyname, bool *success, " type " type_helper) {
"
                                      (->> props
                                           (map (fn [{:keys [form]}]
                                                  (let [[_ prop-name _ ] form
                                                        prop-name ((comp str/capitalize name) prop-name)]
                                                    (str "if(strcmp(propertyname, \"" prop-name "\") == 0) {
  *success = true;
  return " prop-name "_field;
}"))))
                                           (str/join "\n"))
                                      " *success = false; " type " invalid_data; return invalid_data;
}")))
                          (str/join "\n"))
                          
        class-content (str "#ifndef " class-name "e\n#define " class-name "e\n"
                           "\n#include<propertierbase.h>\n"
                       (->> includes
                            (map (partial str "#include" ))
                            (str/join "\n"))
                       "\n#include<cstring>\n"
                       (->> declared-classes
                            (map (partial str "class "))
                            (str/join ";\n"))
                       
                       ";\nclass " class-name ": public Propertierbase {\n " (->> forms
                                                           (map (fn [{:keys [form privacy type]}]
                                                                  (format "%s: %s" privacy 
                                                                          (case type
                                                                            fields (let [[type name default-val] form]
                                                                                     (format "%s %s = %s;" 
                                                                                             (typesymbol->str type)
                                                                                             name
                                                                                             default-val))
                                                                            functions (let [[return-type name param-list] form]
                                                                                        (format "virtual %s %s %s = 0;" (typesymbol->str return-type)
                                                                                                name
                                                                                                param-list))

                                                                            properties (let [[prop-type prop-name default-val] form
                                                                                             prop-type (typesymbol->str prop-type)
                                                                                             prop-name ((comp str/capitalize name) prop-name)]
                                                                                         (format "virtual void set%s(%s val);
virtual %s get%s();
%s %s_field = %s;" prop-name prop-type
                                                                                                 prop-type prop-name
                                                                                                 prop-type prop-name (pr-str default-val)))))))
                                                           (str/join "\n"))
                           set-contents
                           get-contents
                           "\npublic: " class-name "();\n"
                           "\nconst char * r[" (count props) "];"
                           "\nconst char** names() { return r; }\n"
                           "\nvirtual const char* type_identifier() { return \"" class-name "\"; }"
                           "\nvirtual int property_count() { return " (count props) "; }"
                           "\nvirtual const char* type_name(const char *propertyname) {\n"

                           (->> props-by-types
                                (map (fn [[type props]]
                                       (->> props
                                            (map (fn [{[_ prop-name _] :form}]
                                                   (str "if(strcmp(propertyname, \"" prop-name "\") == 0) return \"" type "\";")))
                                            (str/join "\n"))))
                                (str/join "\n"))
                           "return \"\";\n}\n"
                           
                           "\n};\n#endif")
        cpp-content (str "#include <" (str/replace filename #".def" ".h") ">
////// generated at " (tf/unparse (tf/formatter :date-time) (t/now)) "

"
                         (->> props-by-types
                              (map (fn [[type props]]
                                     (->> props
                                          (map (fn [{:keys [form]}]
                                                 (let [[_ prop-name _] form
                                                       prop-name ((comp str/capitalize name) prop-name)]
                                                   (str "void " class-name "::set" prop-name "(" type " val) {
" prop-name "_field = val;
}
                                                        " type " " class-name "::get" prop-name "() {
return " prop-name "_field;
}"))))
                                          (str/join "\n"))))
                              (str/join "\n"))"
"
                         class-name "::" class-name "() {
"
                         (->> props
                              (map-indexed (fn [index {[_ prop-name] :form}]
                                             (str "r[" index "] = \"" prop-name "\";")))
                              (str/join "\n")) "
}")]
    {:header class-content
     :implementation cpp-content}))

        
#_(codegen (tokenize another-test))

(defn read-string-report-fail [str]
  (try
    (read-string str)
    (catch RuntimeException ex
      (println "Todennäköinen EOF stringillä " (pr-str str))
      (throw ex))))

(defn start-compiler-backend! []
  (go-loop [compilation-set (<! compilation-queue)]
    (try
      (let [tokenized-classes (->> compilation-set
                                   (mapv (fn [[filename content]]
                                           (-> content
                                               read-string-report-fail
                                               tokenize
                                               (assoc :filename filename)))))
            tokenized-classes-with-names (zipmap (keys compilation-set)
                                                      tokenized-classes)
            {base-header :header
             base-impl :implementation} (make-propertier-base tokenized-classes)

            class-output (map-vals codegen tokenized-classes-with-names)]

        (let [propertier-h-f (io/file (str @output-dir "/propertierbase.h"))
              propertier-c-f (io/file (str @output-dir "/propertierbase.cpp"))]
          (spit propertier-h-f base-header)
          (spit propertier-c-f base-impl))
        
        (doseq [[file-name {:keys [header implementation]}] class-output
                :let [file-name (str/replace file-name #"\.def" "")
                      header-file (io/file (str @output-dir "/" file-name ".h"))
                      cpp-file (io/file (str @output-dir "/" file-name ".cpp"))]]
          (spit header-file header)
          (spit cpp-file implementation)))
      (catch Exception ex
        (clojure.pprint/pprint ex)))
    (recur (<! compilation-queue))))

(defn start-compilation-d!
  ([]
   (if (and @input-dir
            @output-dir)
     (start-compilation-d! @input-dir
                           @output-dir)
     (println "No either input or output dir set")))
  ([local-input-dir local-output-dir]
   (if (and local-input-dir
            (not @input-dir))
     (reset! input-dir local-input-dir))
   (if (and local-output-dir
            (not @output-dir))
     (reset! output-dir local-output-dir))

   (hawk/watch! [{:paths [local-input-dir]
                  :handler #'handler}])
   (start-compiler-backend!)))
