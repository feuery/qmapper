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

(defun-export! load-sprite-rootless (path)
  (cond ((stringp path) (let ((img (load-img path)))
			  (make-sprite 0 0 0.0 nil (car (last (split path "/"))) t img)))
	((keywordp path) (let ((img path))
			   (make-sprite 0 0 0.0 nil "new sprite" t img)))
	(t (error "called load-sprite-rootless with invalid data"))))
 
(defun-export! load-sprite (root path)
  (let* ((img  (load-img path))
	 (id   (gensym))
	 (mapInd (root-chosenmap root)))
    (format t "Updating root ~%")
    (push-sprite2 root mapInd id (make-sprite 0 0 0.0 mapInd (car (last (split path "/"))) t img))))

(defun-export! is-sprite? (spr)
  (equalp (q-type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
