(defpackage :qmapper.layer
  (:use :common-lisp
        :cl-arrows
	:qmapper.export
	:qmapper.std
	:qmapper.tile
   :qmapper.root)
  (:import-from :fset :size))

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
	   (size (Layer-tiles *this*)))
    (height ()
	    (size (fset-first (Layer-tiles *this*)))))))

(defun-export! make-tiles (w h)
  (let ((all-tiles
	 (repeatedly (lambda (x)
  		       (repeatedly (lambda (y)
  				     (make-Tile 0 0 0 0 nil)) h)) w)))
    all-tiles))

