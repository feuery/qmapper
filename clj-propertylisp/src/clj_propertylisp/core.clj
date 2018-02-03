(ns clj-propertylisp.core
  (:require [hawk.core :as hawk]
            [clj-time.core :as t]
            [clj-time.format :as tf]
            [potpuri.core :refer [map-vals map-keys]]
            [clojure.core.async :refer [go go-loop <! >! chan]]
            [clojure.string :as str]
            [clojure.java.io :as io])
  (:gen-class))

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

#_(start-compilation-d! "/home/feuer2/Dropbox/qt-test/src/lisp-defs"
                      "/home/feuer2/Dropbox/qt-test/src/generated")

(defn typesymbol->str [type]
  (-> type
      name
      (str/replace #"___" ", ")
      (str/replace #"__" "::")
      (str/replace #"_" " ")))

(def test-data '(defcppclass Script ("<string>" "<script-types.h>" "<nsDoesnExistHeader.h>")
  (public
   (properties
    (std__string contents)
    (std__string name "")
    ;; TODO make immutable
    ;; The fourth parameter is a list of validators that are unary C fns that onSet take the new value as param and return a bool that controls if the new value is suitable
    ;; Every validator fn has to return true for it to be set
    ;; If there are no validators, you don't need to write an empty list ()
    (std__string ns "user" (nsDoesntExist nsDoesntReallyExist))
    (scriptTypes script_type scheme)))))

#_(:header (codegen (assoc (tokenize test-data) :filename "lol.def")))

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

;; Why should dissoc work with vectors? What's consistency?
(defn vec-remove
  "remove elem in coll"
  [coll pos]
  (vec (concat (subvec coll 0 pos) (subvec coll (inc pos)))))

(defn tokenize-type [privacy-level [f & forms]]
  (map (fn [form]
         (cond (= f 'properties) {:form (if (> (count form) 3)
                                          (vec-remove (vec form) 3)
                                          form)
                                  :privacy privacy-level
                                  :validators (if (> (count form) 3)
                                                (nth form 3))
                                  :type f}
               :t {:form form
                   :privacy privacy-level
                   :type f}))
       forms))

(defn tokenize-privacy-level [[f & forms]]
  (map (partial tokenize-type f) forms))

(defn pick-declared-classes [def]
  (->> def
       (filter seq?)
       (filter (comp (partial = 'declare-class) first))
       (mapcat (partial drop 1))))

(defn tokenize [[_ class-name include-list :as def]]
  (let [forms (->> def
                   (filter seq?)
                   (filter (fn [[s & lol]]
                             (contains? #{'public 'private 'protected} s)))

                   (mapcat tokenize-privacy-level)
                   (reduce concat))]
    {:class-name class-name
     :includes include-list
     :declared-classes (pick-declared-classes def)
     :abstract? (contains? (->> forms
                                (map :type)
                                set) 'functions)
     :forms forms}))
   

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
              (format "virtual void set(std::string propertyname, %s value);
virtual %s get(std::string propertyname, bool *success, %s type_helper) const;\n" type type type)))
       (str/join "")))

(defn typefy-param-list [param-list]
  (try
    (cond
      (empty? param-list) param-list
      (= (count param-list) 1) (seq [(typesymbol->str (first param-list))])
      :t (try (clojure.pprint/cl-format nil "(~{~s ~s~^, ~})" 
                                        (loop [param-list param-list
                                               index 0
                                               acc []]
                                          (if-not (empty? param-list)
                                            (recur (rest param-list)
                                                   (inc index)
                                                   (if (even? index)
                                                     (conj acc (symbol (typesymbol->str (first param-list))))
                                                     (conj acc (first param-list))))
                                            (seq acc))))
              
              (catch Exception ex
                (clojure.pprint/pprint ex)
                (println "Param-list: ")
                (clojure.pprint/pprint param-list)
                ""
                #_(throw ex))))
    (catch Exception ex
      (clojure.pprint/pprint ex)
      (clojure.pprint/pprint param-list)
      "")))

(defn get-set-strs-implementation [types]
  (->> types
       (map typesymbol->str)
       (map (fn [type]
              (format "void Propertierbase::set(std::string propertyname, %s value) 
{

}

%s Propertierbase::get(std::string propertyname, bool *success, %s type_helper) const
{

}\n" type type type)))
       (str/join "")))
  

(defn make-propertier-base [tokenized-classes]
  (let [props (->> tokenized-classes
                   (map get-properties)
                   flatten)
        _ (assert (not (empty? props)))
        prop-types (->> props
                        (map (comp first :form ))
                        set)
        declared-classes (->> tokenized-classes
                              (mapcat :declared-classes)
                              set)
        classes-to-compile (->> tokenized-classes
                                (map :class-name)
                                set)
        header (format "#ifndef propertierbasee
#define propertierbasee
//// generated at %s

%s

#include <cstdlib>

#include<json.hpp>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLFunctions>

#include <unordered_map>

%s
class Propertierbase 
{
public:
Propertierbase ();
virtual ~Propertierbase ();
  virtual std::string type_name(std::string propertyname) const = 0;
  virtual std::vector<std::string> names() = 0;
  virtual std::string type_identifier() = 0;
  virtual int property_count() = 0;

  virtual std::string getId() const
  {
    return Id_field;
  }

  virtual void setId(std::string v) 
  {
    Id_field = v;
  }

  void set(std::string propName, Propertierbase *b);
  Propertierbase* get(std::string propertyname) const;

%s

  std::unordered_map<std::string, std::vector<std::string>> field_error_map;

  std::string getErrorsOf(std::string field);
  void clearErrorsOf(std::string field);

  virtual std::string toJSON() const = 0;
  virtual void fromJSON(const char* json) = 0;

protected:
  std::string Id_field = std::string(std::to_string(rand()));

};

using nlohmann::json;

Q_DECLARE_METATYPE(Propertierbase*);

#endif"
                       ;; timestamp
                       (tf/unparse (tf/formatters :date-time)  (t/now))

                       ;; relevant includes
                       (->> (get-includes-of-set tokenized-classes)
                            (map #(str "#include" % ""))
                            (str/join "\n"))
                       
                       ;; declared-classes
                       (clojure.pprint/cl-format nil
                                                 "~{class ~A;\n~}" declared-classes)
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

%s

void Propertierbase::set(std::string propName, Propertierbase *b)
{
  auto prop_typename = type_name(propName);
  %s
  else {
    printf(\"Invalid type %%s\\n\", prop_typename.c_str());
    throw \"\";
  }
}

Propertierbase* Propertierbase::get(std::string propertyname) const
{
  auto prop_typename = type_name(propertyname);
  %s
  return nullptr;
}

std::string Propertierbase::getErrorsOf(std::string field) {
 std::string result;

 std::vector<std::string> vec = field_error_map[field];
 for(auto i = vec.begin(); i < vec.end(); i++) {
   result += *i + \"\\n\";
 }

 return result ;
}

void Propertierbase::clearErrorsOf(std::string field) {
 field_error_map[field].clear();
}
"
                    (-> props
                        make-get-sets-of-propertierbase
                        get-set-strs-implementation)

                    (->> prop-types
                         (filter (fn [type]
                                   (contains? classes-to-compile (-> type
                                                                     name
                                                                     (str/replace #"\*" "")
                                                                     symbol))))
                         (map (fn [class]
                                (str "if(prop_typename == std::string(std::string(\"" (-> class
                                                                                                     name
                                                                                                     (str/replace #"\*" "")) "\"))) {
" class " t = reinterpret_cast<" class ">(b);
set(propName, t);
return;
}")))
                         (str/join "\n"))

                    (->> prop-types
                         (filter (fn [type]
                                   (contains? classes-to-compile (-> type
                                                                     name
                                                                     (str/replace #"\*" "")
                                                                     symbol))))
                         (map (fn [class]
                                (str "if(prop_typename == std::string(std::string(\"" (-> class
                                                                                                     name
                                                                                                     (str/replace #"\*" "")) "\"))) {
    bool success = false;
    " class " b = nullptr;
    b = get(propertyname, &success, b);
    if(success) return (Propertierbase*)b;
    else return nullptr;
  }")))
                         (str/join "\n")))]
    {:header header
     :implementation cpp}))

;; https://rosettacode.org/wiki/Count_occurrences_of_a_substring#Clojure
(defn count-substring [txt sub]
  (count (re-seq (re-pattern sub) txt)))

(defn codegen [all-classes {:keys [forms abstract? class-name declared-classes includes filename] :as tokenized-class}]
  (let [compiled-classes (set (mapcat second
                                      (map-vals (fn [{:keys [class-name]}]
                                                  [class-name (symbol (str (name class-name) "*"))])
                                                all-classes)))
        collection-of-compiled-classes (set (mapcat (fn [class-sym]
                                                      (map symbol 
                                                           [(str "std__vector<" (name class-sym) ">*")
                                                            (str "std__vector<" (name class-sym) "*>*")
                                                            (str "std__vector<std__vector<" (name class-sym) ">>*")
                                                            (str "std__vector<std__vector<" (name class-sym) "*>>*")])) compiled-classes))
        is-class-abstract? (into {}
                                 (map second
                                      (map-vals (fn [{:keys [class-name abstract?]}]
                                                  [class-name abstract?]) all-classes)))
        
        ;; _ (locking *out*
        ;;     (println "all classes: ")
        ;;     (clojure.pprint/pprint is-class-abstract?))
        props (conj (get-properties tokenized-class)
                    {:form '[std__string Id ""], :privacy 'public, :type 'properties})        
        props-by-types (->> props
                            (group-by (comp first :form))
                            (map-keys typesymbol->str))

        validator-gen (fn [valid-symbol]
                        (str (name valid-symbol)
                             "(value, error_reporter)"))
        
        set-contents (->> props-by-types
                          (map (fn [[type props]]
                                   (str "virtual void set(std::string propertyname, " type " value) {
" (->> props
       (map (fn [{:keys [form validators]}]
              (let [[prop-type prop-name _] form]
                (str

                 (if validators (str "auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string(\"" prop-name "\")];
	printf(\"Pushing error %s\\n\", msg.c_str());
	vec.push_back(msg);};"))
                 
                 "if(propertyname == std::string(\"" prop-name "\") "
                 (if validators
                   (str " && " (str/join " && "
                                         (map validator-gen validators))))
                 ") { " (-> prop-name name str/capitalize) "_field = value; return; }"))))
       (str/join "\n"))
                                        " }")))
                          (str/join "\n"))
        get-contents (->> props-by-types
                          (map (fn [[type props]]
                                 (str "virtual " type " get(std::string propertyname, bool *success, " type " type_helper) const {
"
                                      (->> props
                                           (map (fn [{:keys [form]}]
                                                  (let [[_ prop-name _ ] form]
                                                    (str "if(propertyname == std::string(\"" prop-name "\")) {
  *success = true;
  return " (-> prop-name name str/capitalize) "_field;
}"))))
                                           (str/join "\n"))
                                      " *success = false; " type " invalid_data; return invalid_data;
}")))
                          (str/join "\n"))
                          
        class-content (str "#ifndef " class-name "e\n#define " class-name "e\n"
                           "\n#include<propertierbase.h>\n"
                           "\n#include<json.hpp>\n"
                           "\nusing nlohmann::json;\n"
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
                                                                                                                       (typefy-param-list 
                                                                                                                        param-list)))

                                                                                                   properties (let [[prop-type prop-name default-val] form
                                                                                                                    prop-type (typesymbol->str prop-type)
                                                                                                                    prop-name ((comp str/capitalize name) prop-name)]
                                                                                                                (if default-val
                                                                                                                  (format "virtual void set%s(%s val);
virtual %s get%s() const;
%s %s_field = %s;" prop-name prop-type
                                                                                                                          prop-type prop-name
                                                                                                                          prop-type prop-name (pr-str default-val))
                                                                                                                  (format "virtual void set%s(%s val);
virtual %s get%s() const;
%s %s_field;" prop-name prop-type
                                                                                                                          prop-type prop-name
                                                                                                                          prop-type prop-name)))))))
                                                           (str/join "\n"))
                           set-contents
                           get-contents
                           "\npublic: virtual std::string toJSON() const;
 virtual void fromJSON(const char* json);"
                           "\npublic: " class-name "();\n"
                           "\nstd::vector<std::string> r;"
                           "\nstd::vector<std::string> names() { return r; }\n"
                           "\nvirtual std::string type_identifier() { return std::string(\"" class-name "\"); }"
                           "\nvirtual int property_count() { return " (count props) "; }"
                           "\nvirtual std::string type_name(std::string propertyname) const {\n"

                           (->> props-by-types
                                (map (fn [[type props]]
                                       (->> props
                                            (map (fn [{[_ prop-name _] :form}]
                                                   (str "if(propertyname == std::string(\"" prop-name "\")) return std::string(\"" (str/replace type #"\*" "") "\");")))
                                            (str/join "\n"))))
                                (str/join "\n"))
                           "return std::string(\"\");\n}\n"
                           
                           "\n};\n\n"
                           class-name "* to" (str/capitalize class-name) "(Propertierbase *b);"
                           "\n
    void to_json(json& j, const " class-name "& c);
    void from_json(const json& j, " class-name "& c);
    void to_json(json& j, const " class-name "* c);
    void from_json(const json& j, " class-name "* c);
    void to_json(json& j, const std::vector<" class-name "*>* v); "
"    void to_json(json& j, const std::vector<std::vector<" class-name "*>>* v);
    void to_json(json& j, const std::vector<std::vector<" class-name ">>* v);"
                           "\n#endif")
        cpp-content (str "#include <" (str/replace filename #".def" ".h") ">\n"
                         (if abstract? 
                           (str "#include <" (str/lower-case class-name) "Container.h>\n")
                           "")
"#include <json.hpp>
////// generated at " (tf/unparse (tf/formatter :date-time) (t/now)) "

"
                         (->> props-by-types
                              (map (fn [[type props]]
                                     (->> props
                                          (map (fn [{:keys [form validators]}]
                                                 (let [[_ prop-name _] form
                                                       prop-name ((comp str/capitalize name) prop-name)]
                                                   (if (not= prop-name "Id")
                                                     (str "void " class-name "::set" prop-name "(" type " value) { 
"
                                                          (if validators (str "auto error_reporter = [&](std::string msg) {
    std::vector<std::string>& vec = field_error_map[std::string(\"" prop-name "\")];
	printf(\"Pushing error %s\\n\", msg.c_str());
	vec.push_back(msg);};"
                                                                              "if(" (str/join " && "
                                                                                                                                                (map validator-gen validators)) ")"))
                                                           prop-name "_field = value;
}
                                                        " type " " class-name "::get" prop-name "() const {
return " prop-name "_field;
}")
                                                     ""))))
                                          (str/join "\n"))))
                              (str/join "\n"))"
"
                         class-name "::" class-name "() {
"
                         (->> props
                              (map-indexed (fn [index {[_ prop-name] :form}]
                                             (str "r.push_back(std::string(std::string(\"" prop-name "\")));")))
                              (str/join "\n")) "
}"
class-name "* to" (str/capitalize class-name) "(Propertierbase *b)
 {
if(b->type_identifier() == std::string(\"" class-name "\")) {
  return static_cast<" class-name "*>(b);
}
else {
printf(\"\\\"to" (str/capitalize class-name) " called with \\\"%s\\\"\\n\", b->type_identifier().c_str());
throw \"\";
}
}

std::string " class-name "::toJSON() const
{
nlohmann::json j {
"
(->> props
     (filter (fn [{[type] :form}]
               (or (contains? compiled-classes type)
                   (contains? collection-of-compiled-classes type)
                   (contains? #{'std__string
                                'int
                                'float
                                'double
                                'unsigned_char
                                'short
                                'long} type))))
     (map (fn [{:keys [form]}]
            (let [[type prop-name & _] form
                  prop-name ((comp str/capitalize name) prop-name)]
              (str "{\"" prop-name "\", get" prop-name "()" 
                   "}"))))
     (str/join ",\n")) "\n};
return j.dump();
}
void " class-name "::fromJSON(const char* json_str)
{
json j = json::parse(json_str);
"
(->> props
     (filter (fn [{[type] :form}]
               (or (contains? compiled-classes type)
                   (contains? collection-of-compiled-classes type)
                   (contains? #{'std__string
                                'int
                                'float
                                'double
                                'unsigned_char
                                'short
                                'long} type))))
     (map (fn [{[type prop-name] :form}]
            (let [collection? (.startsWith (name type) "std__vector")
                  pointer? (.endsWith (name type) "*")
                  collection-pointer? (and collection?
                                           (re-matches #"\*>+$" (name type)))
                  ;; _ (println "Type o " type " (" (class type) "), mutta onko se pointah?" (pr-str collection-pointer?))
                  prop-name ((comp str/capitalize name) prop-name)
                  type (-> (typesymbol->str type)
                           (str/replace #"\*$" ""))]
              (if collection?

                (let [amount-of-vectors (count-substring type "std::vector") ;; This could be assumed to be max 2
                      type (-> type
                               (str/replace #"std::vector<" "")
                               (str/replace #">*\*?" "")
                               #_(str/replace #"\*$" "container"))
                      abstract? (is-class-abstract? (symbol type))]
                  (println "Amount-of-vectors: " amount-of-vectors)
                  (str/join "\n"
                            (concat (reverse
                                     (map (fn [count]
                                            (let [id (- amount-of-vectors count)]
                                              (condp = count
                                                0 (str (if abstract?
                                                         (str type " *o = new " type "container;\n")
                                                         (str type " o;\n"))
"std::string tmp = it" (dec id) "->dump();
const char *c_tmp = tmp.c_str();
o" (if (or collection-pointer? abstract?) "->" ".") "fromJSON(c_tmp);"
                                                       
                                                       (if (> amount-of-vectors 1)
                                                         (str
"                                                       vec.push_back(o);
}
                                                       get" prop-name "()->push_back(vec);")
                                                         (str "get" prop-name "()->push_back(o);\n}")))
                                                (dec amount-of-vectors) (str "for(auto it" id " = it" (dec id) "->begin(); it" id " != it" (dec id) "->end(); it" id "++) {")
                                                ;; else
                                                (str "for(auto it" id " = j[\"" prop-name "\"].begin(); it" id " != j[\"" prop-name "\"].end(); it" id "++) {
" (if (> amount-of-vectors 1 )
    (str "std::vector<" type "> vec;\n ")))))) (range (inc amount-of-vectors))))
                                    (repeat (dec amount-of-vectors) "}\n"))))

                (if pointer?
                  (str "*get" prop-name "() = j[\"" prop-name "\"].get<" type ">();")
                  (str "set" prop-name "(j[\"" prop-name "\"]);"))))))
     (str/join "\n"))
"
}

using nlohmann::json;

    void to_json(json& j, const " class-name "& c) {
        j = json::parse(c.toJSON());
    }

    void from_json(const json& j, " class-name "& c) {
        c.fromJSON(j.get<std::string>().c_str());
    }
    void to_json(json& j, const " class-name "* c) {
      if(c)
        j = json::parse(c->toJSON());
      else
        j = json::parse(\"{\\\"error\\\": \\\"c is null\\\"}\");
    }

    void to_json(json& j, const std::vector<" class-name "*>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           j.push_back(*a);
         }
    }
}
    void to_json(json& j, const std::vector<std::vector<" class-name ">>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void to_json(json& j, const std::vector<std::vector<" class-name "*>>* v) {
    if(v) {
         for(auto a = v->begin(); a != v->end(); a++) {
           json j2;
           for(auto o = a->begin(); o != a->end(); o++) {
             j2.push_back(*o);
           }
           j.push_back(j2);
         }
    }
}

    void from_json(const json& j, " class-name "* c) {
        c->fromJSON(j.get<std::string>().c_str());
}
"
)]
    {:header class-content
     :implementation cpp-content}))

        
#_(codegen (tokenize another-test))

(defn read-string-report-fail [str]
  (try
    (read-string str)
    (catch RuntimeException ex
      (println "Todennäköinen EOF stringillä " (pr-str str))
      (throw ex))))

#_(-> (tokenize test-data)
    (assoc :filename "lol.def")
    codegen)


(defn start-compiler-backend! []
  (go-loop [compilation-set (<! compilation-queue)]
    (try
      (let [tokenized-classes (->> compilation-set
                                   (mapv (fn [[filename content]]
                                           (-> content
                                               read-string-report-fail
                                               tokenize
                                               (assoc :filename filename)))))
            ;; _ (clojure.pprint/pprint tokenized-classes)
            tokenized-classes-with-names (zipmap (keys compilation-set)
                                                      tokenized-classes)
            {base-header :header
             base-impl :implementation} (make-propertier-base tokenized-classes)

            class-output (map-vals (partial codegen tokenized-classes-with-names) tokenized-classes-with-names)]

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

(defn -main [input output]
  (println "Starting compilation daemon which compiles " input "->" output)
  (start-compilation-d! input output))

(def tile '(defcppclass Tile ()
  (public
   (properties
    (int x 0)
    (int y 0)
    (int tileset 0)
    (int rotation 0)))))
