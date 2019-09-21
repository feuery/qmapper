(defpackage :qmapper.sprite
  (:use :common-lisp
	:cl-strings
	:qmapper.export
	:cl-arrows
	:qmapper.std
	:qmapper.tileset
	:qmapper.root))

(in-package :qmapper.sprite)

(defun-export! validate-x (x &rest rr)
  (< x 100))

(defcppclass Sprite
    (public
     (properties
      (x 0 #'validate-x)
      (y 0 (lambda (y old-obj)
	     (< y 100)))
      (angle 0.0)
      (gravity-vector #[0 1] ;; (lambda (g)
			     ;;   (and (fset:seq? g)
			     ;; 	    (every #'numberp (fset:convert 'list g))))
		      )
      (parentMapId "")
      (name "")
      (loadingDone nil)
      (gl-key nil))))


(defun-export! sprite-render (sprite)
  (let* ((angle (sprite-angle sprite))
	 (gl-key (sprite-gl-key sprite))
	 (x (sprite-x sprite))
	 (y (sprite-y sprite)))
    (set-image-x :MAP sprite x)
    (set-image-y :MAP sprite y)
    ;; spriteRotater already sets sprite's rotation in radians, so no deg->rad transformation needed
    (set-image-rotation :MAP sprite angle)
    (render-img :MAP gl-key)))
  

(defun-export! load-sprite-rootless (path)
  (cond ((and (stringp path)
	      (probe-file path))
	 (let ((img (load-img path)))
	   (make-sprite :x 0 :y 0 :angle 0.0 :parentMapId nil :name (car (last (split path "/"))) :loadingDone t :gl-key img)))
	((or (keywordp path)
	     (stringp path))
	 (let ((img path))
	   (make-sprite :x 0 :y 0 :angle 0.0 :parentMapId nil :name "new sprite" :loadingDone t :gl-key img)))
	(t (error "called load-sprite-rootless with invalid data"))))
 
(defun-export! load-sprite (root path)
  (let* ((img  (load-img path))
	 (id   (gensym))
	 (mapInd (root-chosenmap root)))
    (format t "Updating root ~%")
    (push-sprite2 root mapInd id (make-sprite :x 0 :y 0 :angle 0.0 :parentMapId mapInd :name (car (last (split path "/"))) :loadingDone t :gl-key img))))

(defun-export! is-sprite? (spr)
  (equalp (q-type-of spr)
	  "Sprite"))

;; (scm-puts "Loaded sprite")
