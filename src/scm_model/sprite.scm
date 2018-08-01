;; (define-module (qmapper-sprite)
;;   #:use-module (srfi srfi-1)
;;   #:use-module (qmapper-std))

(defcppclass Sprite
  (public
   (properties
    (int x 0)
    (int y 0)
    (float angle 0.0)
    (std__string parentMapId "")
    (std__string name "")
    (obj* obj_ptr nil)
    (bool loadingDone #f))
   (functions
    (render ()
	    (render-object! (Sprite-obj_ptr (this)))))))

(define-and-reg (is-sprite? spr)
  (equal? (type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
