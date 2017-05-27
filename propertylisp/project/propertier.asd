;;;; propertier.asd

(asdf:defsystem #:propertier
  :description "Describe propertier here"
  :author "feuer <feuer@feuerx.net>"
  :license "Specify license here"
  :depends-on (#:cl-arrows)
  :serial t
  :components ((:file "package")
               (:file "propertier")))
