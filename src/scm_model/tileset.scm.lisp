(defpackage :qmapper.tileset
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.tileset)

(defcppclass Tileset
  (public
   (properties
    (std__string name "")
    (Script* vertexShader nullptr)
    (Script* fragmentShader nullptr)
    (std__vector<std__vector<Tile*>>* tiles '()))))

(export-all :qmapper.tileset)
