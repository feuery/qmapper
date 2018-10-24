(defpackage :qmapper.export
  (:use :common-lisp
	:cl-arrows
	:qmapper.std))

(in-package :qmapper.export)

(defvar-export! explode nil)
(defvar-export! get-current-doc nil)
