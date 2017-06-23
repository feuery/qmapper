;;;; propertier.lisp
;; (asdf:load-system "propertier")

;; Build by invoking sbcl
;; Making sure this file script is loadable by asdf (`ln -s ${pwd} /usr/share/common-lisp/source/propertier` in my arch box)
;; And in repl:
;; (asdf:load-system "propertier")
;; (propertier:build!)

(in-package #:propertier)

(defvar *input-dir* '())
(defvar *output-dir* '())
(defvar running? t)
(defvar *compilation-queue* (make-queue))
(defvar *building?* nil)

(defclass propertyclass ()
  ((classname
    :initform 'undefined
    :accessor classname)
   (propertylist ;; name - type alist
    :initform '()
    :accessor propertylist)
   (header-str
    :initform ""
    :accessor header-str)
   (src-str
    :initform ""
    :accessor src-str)
   (header-filename
    :initform ""
    :accessor header-filename)
   (src-filename
    :initform ""
    :accessor src-filename)))

(defun str (&rest s)
  (->> s
       (mapcar #'(lambda (ss)
		   (if (symbolp ss)
		       (symbol-name ss)
		       ss)))   
       (cons 'string)
       (apply #'concatenate)))

(defmacro stdout-str (&rest forms)
  (let ((s (gensym)))
  `(let* ((,s (make-string-output-stream))
	  (*standard-output* ,s))
     (progn
       ,@forms)
     (get-output-stream-string ,s))))

(defun slurp (path)
  (with-open-file (stream path)
    (let ((data (make-string (file-length stream))))
      (read-sequence data stream)
      data)))

(defun read-file (file)
  (let ((code (-> file
		  slurp
		  read-from-string)))
    (setf (nth 1 code) `(quote ,(nth 1 code)))
    (format t "Code: ~A~%" code)
    code))

(defparameter classobj nil)

(defmacro public (&rest forms)
  (format t "public: ~%")
  (dolist (form forms)
    (eval form)))

(defmacro protected (&rest forms)
  (format t "protected: ~%")
  (dolist (form forms)
    (eval form)))

(defmacro private (&rest forms)
  (format t "private: ~%")
  (dolist (form forms)
    (eval form)))

(defun fields* (&rest forms)
  (dolist (l forms)
    (let* ((rforms (reverse l))
	   (default (car rforms))
	   (name (cadr rforms))
	   (type (regex-replace-all "__"
				    (->> l
					 butlast
					 butlast
					 (format nil "~{~a ~}")
					 ) "::")))
	(format t "~A ~A = ~A;~%" type name (prin1-to-string default)))))

(defmacro fields (&rest forms)
  `(fields* ,@(mapcar (lambda (x) `(quote ,x)) forms)))

(defun properties* (&rest forms)
  (dolist (l forms)
    (let* ((rforms (reverse l))
	   (default (car rforms))
	   (name (cadr rforms))
	   (type (regex-replace-all "__"
				    (->> l
					 butlast
					 butlast
					 (format nil "~{~a~^ ~}")) "::")))
      (push `(,name . ,type) (propertylist classobj))
      (format t "~A get~A();~%
void set~A(~A val);~%

~A ~A_field = ~A;~%" type (string-capitalize name)
(string-capitalize name) type
type name (prin1-to-string default)))))

(defmacro properties (&rest forms)
  `(properties* ,@(mapcar (lambda (x) `(quote ,x)) forms)))

(defun last-satisfies (pred coll)
  (-> pred
      (remove-if-not coll)
      last
      car))

(defmacro functions (&rest forms)
  (declare (optimize (debug 3)))
  (dolist (form forms)
    (let ((name (last-satisfies #'atom form))
	  (type (regex-replace-all "__" (->> form
					     (remove-if-not #'atom)
					     butlast
					     (format nil "~{~A~^ ~}"))
				   "::"))
		    
	  (params (->> forms
		       car
		       (remove-if #'atom)
		       (mapcar (lambda (param-list)
				 (format nil "~{~A~^ ~}"
					 (mapcar (lambda (param)
						   (regex-replace-all "__" (format nil "~a" param) "::"))
						 param-list))))
		       (format nil "~{~A~^, ~}"))))
      (format t "virtual ~A ~A(~A) = 0;~%" type name params))))
    
(defun headers (forms)
  (let ((*print-case* :downcase))
    (-> forms
	eval)))

(defun source (forms class-object)
  (declare (optimize (debug 3)))
  (let ((*print-case* :downcase)
	(class-name (cadadr forms)))
    (setf (classname class-object) class-name)
    (format t "#include <~a.h>~%" class-name)
    (format t "///////// CPP FILE STARTS HERE, generated at ~a~%" (now))
    (dolist (x (propertylist class-object))
      (let ((name (car x))
	    (type (regex-replace-all "__"
				     (format nil "~a" (cdr x))
				     "::")))
	(format t "~A ~A::get~A() {~% return ~A_field; ~%};~%"
		type class-name (-> name symbol-name string-capitalize) name)
	(format t "void ~A::set~A(~A val) {~% ~A_field = val; ~%}~%" class-name (-> name symbol-name string-capitalize) type name)))))

(defun load-def-file (path)
  (let* ((file-contents (read-file path))
	 (headd (stdout-str (headers file-contents)))
	 (obj (make-instance 'propertyclass))
	 (cpp (stdout-str (source file-contents obj)))
	 (header-filename (str *output-dir* "/" (format nil "~a" (classname obj)) ".h"))
	 (cpp-filename (str *output-dir* "/" (format nil "~a" (classname obj)) ".cpp")))
    (setf (src-str obj) cpp)
    (setf (header-str obj) headd)
    (setf (header-filename obj) header-filename)
    (setf (src-filename obj) cpp-filename)
    obj))

(defun set-watch-on (dirpath)
  (setf running? t)
  (make-thread (lambda ()
		 (in-package #:propertier)
		 (open-fsnotify)
		 (add-watch dirpath)
		 (loop while running?
		    do (let ((e (get-events)))
			 (dolist (event e)
			   (let ((path (car event))
				 (type (cdr event)))
			     (when (or (eq type :CREATE)
				       (eq type :MODIFY))
			       (push-queue path *compilation-queue*))))
			 (sleep 3))))))

(defvar *baseclass-header-start*
  "#ifndef propertierbase
#define propertierbase
//// generated at #<date-time 2017-06-17 18:44:10.914 {10067809C3}>

class propertierbase
{
public:
  propertierbase() {}
  ~propertierbase() {}
  virtual const char* type_name(const char* propertyname) = 0;
  const char** names();
")

(defvar *baseclass-header-stop* 
"
};
#endif
")

(defun handle-base-class (property-types)
  (let ((header-filename (str *output-dir* "/propertierbase.h")))
    (with-open-file (f header-filename :direction :output :if-exists :overwrite :if-does-not-exist :create)
      (write-string *baseclass-header-start* f)
      (write-string
       (->> property-types
	    remove-duplicates
	    (mapcar #'(lambda (type)
			(str
			 (format nil "virtual void set(const char* propertyname, ~a value) = 0;~%" type)
			 (format nil "virtual ~a get(const char* propertyname, bool *success) = 0;~%" type)))))
       f)		       
      (write-string *baseclass-header-stop* f))))

(defun compilation-server-loop ()
  (loop while running?
     do (let* ((*print-case* :downcase)
	       (file-to-compile (pop-queue *compilation-queue*))
	       (file-type (-> (format nil "~A" file-to-compile)					     
			      reverse
			      (subseq 0 3)
			      reverse)))
	  (if (and (string= file-type "def")
		   (probe-file file-to-compile))
	      (let* ((compilation-objs (->> (list-directory ".")
					    (remove-if-not #'(lambda (x)
							       (string= (pathname-type x) "def")))
					    (mapcar #'load-def-file)))
		     (property-types (->> compilation-objs
					  (mapcar #'propertylist)
					  (mapcar #'cdr))))
		(handle-base-class property-types)
		(dolist (obj compilation-objs)
		  (with-open-file (f (header-filename obj) :direction :output :if-exists :overwrite :if-does-not-exist :create)
		    (write-string (header-str obj) f))
		  (with-open-file (f (src-filename obj) :direction :output :if-exists :overwrite :if-does-not-exist :create)
		    (write-string (src-str obj) f)))))
	  (sleep 2))))

(defun start-compilation-server ()
  (set-watch-on *input-dir*)
  (if *building?*
      (compilation-server-loop)
      (make-thread #'compilation-server-loop)))


  
;; ;; (ql:quickload "cl-arrows")
;; ;; (ql:quickload "quickproject")
;; ;; (ql:quickload "cl-fsnotify")
;; ;; (ql:quickload :lparallel)
;; ;; (ql:quickload :simple-date-time)
;; ;; (ql:quickload :cl-ppcre)
;; (ql:quickload :cl-fad)
;; ;; (require :quickproject)

;; (defun range (start &optional end)
;;   (if end
;;       (loop for i from start below end collect i)
;;       (loop for i from 0 below start collect i)))


;; (defun flatten (l)
;;   (cond ((null l) nil)
;;         ((atom l) (list l))
;;         (t (loop for a in l appending (flatten a)))))


;; (defun fill-property-names (classname)
;;   (format t "~A() {~%" classname)
       
;;   (mapcar (lambda (property-pair i)
;; 	    (let ((name (car property-pair)))
;; 	      (format t "r[~D] = \"~A\";~%" i name)))
;; 	  property-container
;; 	  (range (length property-container)))

;;   (format t "}~%")
  
;;   (format t "const char* type_name(const char* propertyname) {~%")

;;   (->> property-container
;;        (mapcar (lambda (x)
;; 		 (let ((name (car x))
;; 		       (type (regex-replace-all "__"
;; 					    (format nil "~a" (cdr x))
;; 					    "::")))
;; 		   (format t "if (strcmp(propertyname, \"~A\") == 0) return \"~A\";~%" name type)))))
;;   (format t "return \"\";~%}~%")

;;   (format t "const char** names() { return r; }~%")
;;   (format t "int property_count = ~a;~%" (length property-container))

;;   (format t "template<typename T>~% void set(const char* propertyname, T value) { ~%")
;;   (->> property-container
;;        (mapcar (lambda (x)
;; 		 (let ((name (car x)))
;; 		   (format t "if(strcmp(propertyname, \"~A\") == 0) ~A_field = value;~%" name name)))))
;;   (format t "}~% template<typename T>~% T get(const char* propertyname, bool* success) {~%")
;;   (->> property-container
;;        (mapcar (lambda (x)
;; 		 (let ((name (car x)))
;; 		   (format t "if(strcmp(propertyname, \"~A\") == 0) {~%*success = true; ~% return ~A_field; ~%}~%" name name)))))
;;   (format t "/////~%")
;;   (format t "T t;~% *success = false; ~% return t; ~%}~%")
;;   (format t "/////~%")
;;   (format t "const char* r[~D];~%" (length property-container)))

;; (defmacro inherits (&rest classes)
;;   (format t "~{~a~^,~}~%{~%" (mapcar (lambda (class)
;; 				    (regex-replace-all "__"
;; 						       (format nil "~a" class)
;; 						       "::"))
;; 				    classes)))

;; (defmacro defcppclass (class include-list &rest forms)
;;   (let ((classname (cadr class))
;; 	(contains-inherit? (> (->> forms
;; 				(remove-if-not (lambda (f)
;; 						 (eq (car f) 'inherits)))
;; 				length) 0)))
;;     (format t "#ifndef ~a~%#define ~a~%" classname classname)
;;     (format t "//// generated at ~a~%" (now))
;;     (format t "#include<cstring>~%")
;;     (format t "#include<propertierbase.h>~%")
;;     (dolist (file include-list)
;;       (format t "#include~a~%" file))

;;     (format t "class ~A: public propertierbase ~%" classname)

;;     (if contains-inherit?
;; 	(format t ", ")
;; 	(format t " { ~%"))
    
;;     (dolist (form forms)
;;       (eval form))
;;     (format t "public: ~%")
;;     (fill-property-names classname)
;;     (format t "};~%")
;;     (setf *compiled-class* class)
;;     (format t "#endif")))

;; (defun get-argv (key)
;;   (let ((key (if (symbolp key)
;; 		 key
;; 		 (read-from-string key))))
;;     (assert (= (mod (length (cdr sb-ext:*posix-argv*)) 2) 0))
;;     (getf (plistify (cdr sb-ext:*posix-argv*)) key)))

;; (defun main ()
;;   (setf running? nil)
;;   (unless *output-dir*
;;     (setf *output-dir* (get-argv "--output-dir")))
;;   (unless *input-dir*
;;       (setf *input-dir* (get-argv "--input-dir")))
;;   (mapcar (lambda (x)
;; 	    (pop-queue *compilation-queue*)) (range (queue-count *compilation-queue*)))

;;   (cond
;;     ((not *output-dir*) (format t "You need to set value to *output-dir*. Command line syntax is \"--output-dir path\". Call (main) again after setfing it"))
;;     ((not *input-dir*) (format t "You need to set value to *input-dir*. Command line syntax is \"--input-dir path\". Call (main) again after setfing it"))
;;     (t (start-compilation-server))))

;; (defun build! ()
;;   (setf *building?* t)
;;   (sb-ext:save-lisp-and-die "./image" :toplevel #'main :executable t))

