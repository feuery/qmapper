;;;; package.lisp

(defpackage #:propertier
  (:use #:cl #:cl-arrows #:cl-fsnotify #:lparallel.queue #:sb-thread #:simple-date-time)
  (:import-from #:sb-posix :getcwd)
  (:export :defcppclass
	   :build!
	   :protected
	   :public
	   :private
	   :properties
	   :functions
	   :fields))

