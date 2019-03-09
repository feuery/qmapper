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
    (ns "user" ;; (doesntContainNs)
		 )
    (script_type 'lisp))
   (functions
    (is-lisp? ()
		(let ((script-type (Script-script_type *this*)))
		  ;; (format t "script type: ~a, is it lisp? ~a~%" script-type (equalp script-type 'lisp))
		  (string= (format nil "~a" script-type) "lisp")))
    (is-glsl? ()
    	      (let ((script-type (Script-script_type *this*)))
    		(equalp script-type 'glsl))))))
