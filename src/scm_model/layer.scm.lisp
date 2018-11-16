(defpackage :qmapper.layer
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.tile
	:qmapper.root))

(in-package :qmapper.layer)

(defcppclass Layer
  (public
   (properties
    (name "")
    (opacity 255)
    (visible t)
    (tiles '())
    ;; (parent nil)
    )
   (functions
    (width ()
	   (length (Layer-tiles *this*)))
    (height ()
	    (length (car (Layer-tiles *this*)))))))

(defun-export! make-tiles (w h)
  (let ((all-tiles
	 (repeatedly (lambda (x)
  		       (repeatedly (lambda (y)
  				     (make-Tile 0 0 0 0 nil)) h)) w)))
    all-tiles))

