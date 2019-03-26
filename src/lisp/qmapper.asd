(defpackage #:qmapper
  (:use :cl :asdf))

(in-package #:qmapper)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows)
  (ql:quickload :fset)
  (ql:quickload :rutils)
  (ql:quickload :cl-strings)
  (ql:quickload :cl-ppcre))



(asdf:defsystem #:qmapper
  :description "QMapper's main package file"
  :author "feuer <feuer@feuerx.net>"
  :license "Refer to github"
  :depends-on ("cl-arrows" "fset")
  :components ((:file "qmapper_std"
		      ;; :depends-on (:cl-arrows)
		      :depends-on ("export"))
	       (:file "animatedsprite"
		:depends-on ("qmapper_std"
			     "sprite"
			     "tileset"
			     ;; "map"
			     "export"
			     "root"))
  	       
  	       (:file "script"
		:depends-on ("qmapper_std"))
	       (:file "export")
  	       (:file "map"
		:depends-on ("qmapper_std"
			     "animatedsprite"
			     "export"
			     "root"
			     "tileset"
			     "sprite"
			     "script"
			     "layer"
			     "tile"))
	       (:file "root"
		:depends-on ("script" ;; "map"
			     "export"
			     ))
  	       (:file "sprite"
		:depends-on ("tileset"))
  	       (:file "tile"
		:depends-on ("root"))
  	       (:file "layer"
		:depends-on ("tile"))
  	       (:file "tileset"
		:depends-on ("qmapper_std"
			     "tile"
			     "root"
			     "export"))))
