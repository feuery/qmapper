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
    (int x 0)
    (int y 0)
    (float angle 0.0)
    (std__string parentMapId "")
    (std__string name "")
    (obj* obj_ptr nil)
    (bool loadingDone nil))))

(defun is-sprite? (spr)
  (equalp (q-type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
