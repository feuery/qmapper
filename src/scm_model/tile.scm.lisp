(defpackage :qmapper.tile
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.tile)

(defcppclass Tile 
  (public
   (properties
    (x 0)
    (y 0)
    (tileset 0) 
    (rotation 0))))

(export-all :qmapper.tile)
