(defpackage #:qmapper
  (:use :cl :asdf))

(in-package #:qmapper)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows))

(asdf:defsystem #:qmapper
  :description "QMapper's main package file"
  :author "feuer <feuer@feuerx.net>"
  :license "Refer to github"
  :depends-on ("cl-arrows")
  :components ((:file "qmapper_std"
		      ;; :depends-on (:cl-arrows)
		      )
	       (:file "animatedsprite.scm"
		      :depends-on ("qmapper_std"
				   "root.scm"))
  	       (:file "layer.scm")
  	       (:file "map.scm")
  	       (:file "root.scm")
  	       (:file "script.scm")
  	       (:file "sprite.scm")
  	       (:file "tile.scm")
  	       (:file "tileset.scm"))
  )
