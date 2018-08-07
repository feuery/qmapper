;; (define-module (qmapper-script)
;;   #:use-module (srfi srfi-1)
;;   #:use-module (qmapper-std))

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

;; (scm-puts "Loaded script")
