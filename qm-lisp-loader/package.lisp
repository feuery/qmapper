;; ;;;; package.lisp
(eval-when (:compile-toplevel :load-toplevel :execute)
  (ql:quickload 'cl-arrows)
  (ql:quickload :fset)
  (ql:quickload :rutils)
  (ql:quickload :cl-strings)
  (ql:quickload :cl-ppcre)
  (ql:quickload :lisp-unit)
  (ql:quickload :cl-fad))

(defpackage #:qmapper-loader
  (:use #:cl ;; :ql
	:cl-ppcre)
  (:import-from :fset :empty-map :empty-seq :seq :insert :convert :with :lookup :wb-map-from-list :fset-setup-readtable)
  
  (:shadowing-import-from :fset :map :map? :seq?))
