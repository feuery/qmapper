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
		(let ((script-type (Script-script_type *this*)))
		  ;; (format t "script type: ~a, is it scheme? ~a~%" script-type (equalp script-type 'scheme))
		  (string= (format nil "~a" script-type) "scheme")))
    (is-glsl? ()
    	      (let ((script-type (Script-script_type *this*)))
    		(equalp script-type 'glsl))))))
