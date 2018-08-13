(defpackage :qmapper.script
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	;; :qmapper.root
	))

(in-package :qmapper.script)

(defcppclass Script
  (public
   (properties
    (contents "")
    (name "")
    ;; TODO redo validators in pure scheme
    (ns "user" ;; (doesntContainNs)
		 )
    (script_type 'scheme))
   (functions
    (is-scheme? ()
		(let ((script-type (Script-script_type (this))))
		  (equal? script-type 'scheme)))
    (is-glsl? ()
	      (let ((script-type (Script-script_type (this))))
		(equal? script-type 'glsl))))))

(export-all :qmapper.script)

;; (scm-puts "Loaded script")
