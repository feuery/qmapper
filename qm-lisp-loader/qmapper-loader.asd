;;;; qmapper-loader.asd

(asdf:defsystem #:qmapper-loader
  :description "Describe qmapper-loader here"
  :author "Feuer <feuer@feuerx.net>"
  :license  "Specify license here"
  :version "0.0.1"
  :serial t
  :depends-on (#:fset #:cl-arrows #:cl-ppcre #:cl-fad)
  :components ((:file "package")
               (:file "qmapper-loader")))
