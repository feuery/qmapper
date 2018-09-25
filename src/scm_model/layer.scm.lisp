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
	   (length (first (Layer-tiles (this)))))
    (height ()
	    (length (caar (Layer-tiles (this))))))))

(defun-export! make-tiles (w h)
  (repeatedly (lambda (x)
  		(repeatedly (lambda (y)
  			      (make-Tile 0 0 nil 0)) h)) w)
  nil
  )

;; (export-all :qmapper.layer)
