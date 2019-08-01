(require :asdf)

#-quicklisp
(let ((quicklisp-init (merge-pathnames "quicklisp/setup.lisp"
                                       (user-homedir-pathname))))
  (when (probe-file quicklisp-init)
    (load quicklisp-init)))

(ql:register-local-projects)

(defpackage #:qmapper
  (:use :cl :asdf :ql))

(in-package #:qmapper)

(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows)
  (ql:quickload :fset)
  (ql:quickload :rutils)
  (ql:quickload :cl-strings)
  (ql:quickload :cl-ppcre)
  (ql:quickload :usocket)
  (ql:quickload :bordeaux-threads))



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
	       (:file "editor_events"
		      :depends-on ("qmapper_std"
				   "export"))
	       (:file "engine_events"
		      :depends-on ("qmapper_std"
				   "export"))
	       (:file "selection-tool"
		      :depends-on ("qmapper_std"
				   "export"
				   "root"
				   "map"
				   "tile"
				   "editor_events"))
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
			     "export"))
	       (:file "transitions"
		:depends-on ("qmapper_std"
			     "root"
			     "export"))
	       (:file "keyboard_loop"
		      :depends-on ("qmapper_std"
				   "engine_events"
				   "root"
				   "export"))
	       (:file "test-framework"
		:depends-on ("export"
			     "qmapper_std"
			     "root"))
	       (:file "doc-server"
		      :depends-on ("qmapper_std"
				   "root"
				   "script"
				   "export"
				   "root"))
	       (:file "resize-test"
		:depends-on ("qmapper_std"
			     "test-framework"
			     "root"
			     "map"))))
