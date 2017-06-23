;;;; propertier.asd

(asdf:defsystem #:propertier
  :description "Describe propertier here"
  :author "feuer <feuer@feuerx.net>"
  :license "Specify license here"
  :depends-on (#:cl-arrows #:cl-fsnotify :lparallel #:simple-date-time #:cl-ppcre #:cl-fad)
  :serial t
  :components ((:file "package")
               (:file "propertier")))
