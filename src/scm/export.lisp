(defpackage :qmapper.export
  (:use :common-lisp
	:cl-arrows
	:qmapper.std)
  (:import-from :fset :empty-map :convert :with :lookup))

(in-package :qmapper.export)

(defvar-export! explode nil)
(defvar-export! get-current-doc nil)

(defvar-export! lambdas (empty-map))

(defvar-export! load-image nil)
(defvar-export! image-w nil)
(defvar-export! image-h nil)
(defvar-export! copy-image nil)

(defvar-export! add-to-drawingqueue nil)
(defvar-export! clear-drawingqueue nil)
(defvar-export! set-img-x nil)
(defvar-export! set-img-y nil)

(defvar-export! do-schedule-lambda nil)

