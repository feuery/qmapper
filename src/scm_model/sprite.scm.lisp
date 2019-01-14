(defpackage :qmapper.sprite
  (:use :common-lisp
	:cl-strings
	:cl-arrows
	:qmapper.std
	:qmapper.tileset
	:qmapper.root))

(in-package :qmapper.sprite)

(defcppclass Sprite
  (public
   ;; (functions)
   (properties
    (x 0)
    (y 0)
    (angle 0.0)
    (parentMapId "")
    (name "")
    (loadingDone nil)
    (gl-key nil))))
 
(defun-export! load-sprite (root path)
  (let* ((img  (load-img path))
	 (id   (gensym))
	 (mapInd (root-chosenmap root)))
    (format t "Updating root ~%")
    (-> root		      
	(update-prop 'sprites (lambda (sprites)
				(-> sprites
				    (set-prop id 
					      (set-prop (make-sprite 0 0 0.0 mapInd (car (last (split path "/"))) t img) 'id id)))))
	(update-prop-in (list 'maps mapInd 'sprites) (lambda (sprites)
						       (cons id sprites))))))
(defun-export! is-sprite? (spr)
  (equalp (q-type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
