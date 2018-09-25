(defpackage :qmapper.sprite
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.sprite)


(defcppclass Sprite
  (public
   (functions
    (render ()
	    (render-object! (Sprite-obj_ptr *this*)))
    (lol ()
	 "Lollo"))
   (properties
    (x 0)
    (y 0)
    (angle 0.0)
    (parentMapId "")
    (name "")
    (obj_ptr nil)
    (loadingDone nil))))

(defun-export! is-sprite? (spr)
  (equalp (q-type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
