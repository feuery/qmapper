(defpackage :qmapper.tileset
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.tileset)

(defcppclass Tileset
  (public
   (properties
    (name "")
    (vertexShader nullptr)
    (fragmentShader nullptr)
    (tiles '()))))

(export-all :qmapper.tileset)
