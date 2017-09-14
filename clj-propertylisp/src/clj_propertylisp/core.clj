;; For now this is intended to be ran as a repl
;; Mostly because I don't dislike cider-repl as much as I dislike what SLIME does
(ns clj-propertylisp.core
  (:require [hawk.core :as hawk]
            [clojure.java.io :as io]))

(def input-dir (atom nil))
(def output-dir (atom nil))
(def compilation-queue (atom []))

(add-watch compilation-queue :lol (fn [_ _ _ a]
                                      (println "Added stuff to compilation queue")
                                      (clojure.pprint/pprint a)))

(defn getContainerDirPath [path]
  (io/file (.getParent (if (string? path)
                         (io/file path)
                         path))))

(defn push-compilation! [content]
  (swap! compilation-queue conj content))

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


(start-compilation-d! "/home/feuer2/Dropbox/qt-test/src/lisp-defs"
                      "/home/feuer2/Dropbox/qt-test/src/generated")
