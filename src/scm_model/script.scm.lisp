(defpackage :qmapper.script
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.script)

(defcppclass Script
  (public
   (properties
    (std__string contents "")
    (std__string name "")
    ;; TODO redo validators in pure scheme
    (std__string ns "user" ;; (doesntContainNs)
		 )
    (scriptTypes script_type 'scheme))
   (functions
    (is-scheme? ()
		(let ((script-type (Script-script_type (this))))
		  (equal? script-type 'scheme)))
    (is-glsl? ()
	      (let ((script-type (Script-script_type (this))))
		(equal? script-type 'glsl))))))

(export-all :qmapper.script)

;; (scm-puts "Loaded script")
