(defpackage #:qmapper
  (:use :cl :asdf))

(in-package #:qmapper)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows)
  (ql:quickload :fset))

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
	       (:file "root.scm"
		      :depends-on ("script.scm"))
  	       (:file "sprite.scm")
  	       (:file "tile.scm")
  	       (:file "layer.scm"
		      :depends-on ("tile.scm"))
  	       (:file "tileset.scm")
	       (:file "treecache"
		      :depends-on ("qmapper_std"))
  	       (:file "map.scm"
		      :depends-on ("qmapper_std"
				   "treecache"
				   "root.scm"
				   "tileset.scm"
				   "sprite.scm"
				   "script.scm"
				   "layer.scm"
				   "animatedsprite.scm"
				   "tile.scm")))
  )
