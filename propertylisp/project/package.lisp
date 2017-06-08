;;;; package.lisp

(defpackage #:propertier
  (:use #:cl #:cl-arrows #:cl-fsnotify #:lparallel.queue #:sb-thread)
  (:import-from #:sb-posix :getcwd)
  (:export :defcppclass
	   :protected
	   :public
	   :private
	   :properties
	   :functions
	   :fields))

