(defpackage #:qmapper
  (:use :cl :asdf))

(in-package #:qmapper)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows)
  (ql:quickload :fset)
  (ql:quickload :rutils)
  (ql:quickload :cl-strings))

(asdf:defsystem #:qmapper
  :description "QMapper's main package file"
  :author "feuer <feuer@feuerx.net>"
  :license "Refer to github"
  :depends-on ("cl-arrows" "fset")
  :components ((:file "qmapper_std"
		      ;; :depends-on (:cl-arrows)
		      )
	       (:file "animatedsprite.scm"
		      :depends-on ("qmapper_std"
				   "root.scm"))
  	       
  	       (:file "script.scm"
		      :depends-on ("qmapper_std"))
	       (:file "export"
		      :depends-on ("qmapper_std"))
  	       (:file "map.scm"
		      :depends-on ("qmapper_std"
				   "export"
				   "root.scm"
				   "tileset.scm"
				   "sprite.scm"
				   "script.scm"
				   "layer.scm"
				   "animatedsprite.scm"
				   "tile.scm"))
	       (:file "root.scm"
		      :depends-on ("script.scm" ;; "map.scm"
				   "export"
						))
  	       (:file "sprite.scm")
  	       (:file "tile.scm")
  	       (:file "layer.scm"
		      :depends-on ("tile.scm"))
  	       (:file "tileset.scm"
		      :depends-on ("qmapper_std"
				   "tile.scm"
				   "root.scm"
				   "export"))))
