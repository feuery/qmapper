(defpackage :qmapper.tile
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.tile)

(defcppclass Tile 
  (public
   (properties
    (int x 0)
    (int y 0)
    (std__string tileset) 
    (int rotation 0))))

(export-all :qmapper.tile)
