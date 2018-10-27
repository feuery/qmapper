(defpackage :qmapper.export
  (:use :common-lisp
	:cl-arrows
	:qmapper.std)
  (:import-from :fset :empty-map :convert :with :lookup))

(in-package :qmapper.export)

(defvar-export! explode nil)
(defvar-export! get-current-doc nil)

(defvar-export! lambdas (empty-map))
