;;;; package.lisp

(defpackage #:propertier
  (:use #:cl #:cl-arrows #:cl-fsnotify #:lparallel.queue #:sb-thread #:simple-date-time #:cl-ppcre)
  (:import-from #:sb-posix :getcwd)
  (:export :defcppclass
	   :build!
	   :main
	   :protected
	   :public
	   :private
	   :properties
	   :functions
	   :fields))

