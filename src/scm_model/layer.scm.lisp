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
    (std__string name "")
    (unsigned_char opacity 255)
    (bool visible t)
    (std__vector<std__vector<Tile*>>* tiles '())
    (Map* parent nil))
   (functions
    (width ()
	   (length (first (Layer-tiles (this)))))
    (height ()
	    (length (caar (Layer-tiles (this))))))))

(defun make-tiles (w h)
  (repeatedly (lambda (x)
		(repeatedly (lambda (y)
			      (make-Tile 0 0 nil 0)) h)) w))

(export-all :qmapper.layer)
