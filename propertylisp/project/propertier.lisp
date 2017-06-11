;;;; propertier.lisp
;; (asdf:load-system "propertier")

;; Build by invoking sbcl
;; Making sure this file script is loadable by asdf (`ln -s ${pwd} /usr/share/common-lisp/source/propertier` in my arch box)
;; And in repl:
;; (asdf:load-system "propertier")
;; (propertier:build!)

(in-package #:propertier)

(defun plistify (l)
  (assert (= 0 (mod (length l) 2) 0))
  (defun -plistify (l prop?)
    (if l
	(cons (if prop?
		  (read-from-string (car l))
		  (car l))
	      (-plistify (cdr l)
			 (not prop?)))
	'()))
  (-plistify l t))

;; (ql:quickload "cl-arrows")
;; (ql:quickload "quickproject")
;; (ql:quickload "cl-fsnotify")
;; (ql:quickload :lparallel)
;; (ql:quickload :simple-date-time)
;; (require :quickproject)

;; (quickproject:make-project "~/Dropbox/qt-test/propertylisp/project/"
;; 			   :depends-on '(cl-arrows)
;; 			   :name "propertier"
;; 			   :author "feuer <feuer@feuerx.net>")

(defvar property-container '())
(defvar *compiled-class* nil)

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

(defun str (&rest s)
  (->> s
       (mapcar #'(lambda (ss)
		   (if (symbolp ss)
		       (symbol-name ss)
		       ss)))   
       (cons 'string)
       (apply #'concatenate)))

(defun last-satisfies (pred coll)
  (-> pred
      (remove-if-not coll)
      last
      car))

(defun range (start &optional end)
  (if end
      (loop for i from start below end collect i)
      (loop for i from 0 below start collect i)))

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
	   (type (->> l
		       butlast
		       butlast
		       (format nil "~{~a ~}"))))
	(format t "~A ~A = ~A;~%" type name (prin1-to-string default)))))

(defmacro fields (&rest forms)
  `(fields* ,@(mapcar (lambda (x) `(quote ,x)) forms)))

(defun properties* (&rest forms)
  (dolist (l forms)
    (let* ((rforms (reverse l))
	   (default (car rforms))
	   (name (cadr rforms))
	   (type (->> l
		      butlast
		      butlast
		      (format nil "~{~a ~}"))))
      (push `(,name . ,type) property-container)
      (format t "~A get~A();~%
void set~A(~A val);~%

~A ~A_field = ~A;~%" type (string-capitalize name)
(string-capitalize name) type
type name default))))

(defmacro properties (&rest forms)
  `(properties* ,@(mapcar (lambda (x) `(quote ,x)) forms)))


(defun flatten (l)
  (cond ((null l) nil)
        ((atom l) (list l))
        (t (loop for a in l appending (flatten a)))))

(defmacro functions (&rest forms)
  (dolist (form forms)
    (let ((name (last-satisfies #'atom form))
	  (type (->> form
		     (remove-if-not #'atom)
		     butlast
		     (format nil "~{~A~^ ~}")))	    
		    
	  (params (->> forms
		       car
		       (remove-if #'atom)
		       (mapcar (lambda (param-list)
				 (format nil "~{~A~^ ~}"
					 param-list)))
		       (format nil "~{~A~^, ~}"))))
      (format t "virtual ~A ~A(~A) = 0;~%" type name params))))

(defun fill-property-names (classname)
  (format t "~A() {~%" classname)
       
  (mapcar (lambda (property-pair i)
	    (let ((name (car property-pair)))
	      (format t "r[~D] = \"~A\";~%" i name)))
	  property-container
	  (range (length property-container)))

  (format t "}~%")
  
  (format t "const char* type_name(const char* propertyname) {~%")

  (->> property-container
       (mapcar (lambda (x)
		 (let ((name (car x))
		       (type (cdr x)))
		   (format t "if (strcmp(propertyname, \"~A\") == 0) return \"~A\";~%" name type)))))
  (format t "return \"\";~%}~% template<typename T>~% void set(const char* propertyname, T value) { ~%")
  (->> property-container
       (mapcar (lambda (x)
		 (let ((name (car x)))
		   (format t "if(strcmp(propertyname, \"~A\") == 0) ~A_field = value;~%" name name)))))
  (format t "}~% template<typename T>~% T get(const char* propertyname, bool* success) {~%")
  (->> property-container
       (mapcar (lambda (x)
		 (let ((name (car x)))
		   (format t "if(strcmp(propertyname, \"~A\") == 0) {~%*success = true; ~% return ~A_field; ~%}~%" name name)))))
  (format t "/////~%")
  (format t "T t;~% *success = false; ~% return t; ~%}~%")
  (format t "/////~%")
  (format t "const char* r[~D];~%" (length property-container)))
			
			

(defmacro defcppclass (class &rest forms)
  (let ((classname (cadr class)))
    (format t "//// generated at ~a~%" (now))
    (format t "#include<cstring>~%")
    (format t "class ~A {~%" classname)
    (dolist (form forms)
      (eval form))
    (format t "public: ~%")
    (fill-property-names classname)
    (format t "};~%")
    (setf *compiled-class* class)))

(defun headers (forms)
  (let ((*print-case* :downcase))
    (setf *compiled-class* nil)
    (-> forms
	eval)))

(defun source (forms)
  (let ((*print-case* :downcase)
	(classname (cadadr forms)))
    (format t "#include <~a.h>~%" classname)
    (format t "///////// CPP FILE STARTS HERE, generated at ~a~%" (now))
    (dolist (x property-container)
      (let ((name (car x))
	    (type (cdr x)))
	(format t "~A ~A::get~A() {~% return ~A_field; ~%};~%"
		type classname (-> name symbol-name string-capitalize) name)
	(format t "void ~A::set~A(~A val) {~% ~A_field = val; ~%}~%" classname (-> name symbol-name string-capitalize) type name)))))

(defvar running? t)
(defvar *compilation-queue* (make-queue))

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

(defmacro stdout-str (&rest forms)
  (let ((s (gensym)))
  `(let* ((,s (make-string-output-stream))
	  (*standard-output* ,s))
     (progn
       ,@forms)
     (get-output-stream-string ,s))))

(defun compilation-server-loop ()
  (loop while running?
     do (let* ((*print-case* :downcase)
	       (to-compile (pop-queue *compilation-queue*))
	       (file-type (-> (format nil "~A" to-compile)					     
			      reverse
			      (subseq 0 3)
			      reverse)))
	  (setf property-container '())
	  (if (and (string= file-type "def")
		   (probe-file to-compile))
	      (let* ((file-contents (read-file to-compile))
		     (headd (stdout-str (headers file-contents)))
		     (cpp (stdout-str (source file-contents)))
		     (header-filename (str *output-dir* "/" (format nil "~a" (cadr *compiled-class*)) ".h"))
		     (cpp-filename (str *output-dir* "/" (format nil "~a" (cadr *compiled-class*)) ".cpp")))
		(format t "Outputting sources to ~A and ~A~%" header-filename cpp-filename)
		(when *compiled-class*
		  (with-open-file (f header-filename :direction :output :if-exists :overwrite :if-does-not-exist :create)
		    (write-string headd f))
		  (with-open-file (f cpp-filename :direction :output :if-exists :overwrite :if-does-not-exist :create)
		    (write-string cpp f))
		  (setf *compiled-class* nil)
		  (format t "Wrote files ~a and ~a~%" header-filename cpp-filename))))
	  (sleep 2))))

(defvar *building?* nil)

(defun start-compilation-server ()
  (set-watch-on *input-dir*)

  (if *building?*
      (compilation-server-loop)
      (make-thread #'compilation-server-loop)))

(defun get-argv (key)
  (let ((key (if (symbolp key)
		 key
		 (read-from-string key))))
    (assert (= (mod (length (cdr sb-ext:*posix-argv*)) 2) 0))
    (getf (plistify (cdr sb-ext:*posix-argv*)) key)))

(defvar *input-dir* '())
(defvar *output-dir* '())

(defun main ()
  (setf running? nil)
  (setf *output-dir* (get-argv "--output-dir"))
  (setf *input-dir* (get-argv "--input-dir"))
  (mapcar (lambda (x)
	    (pop-queue *compilation-queue*)) (range (queue-count *compilation-queue*)))

  (cond
    ((not *output-dir*) (format t "You need to set value to *output-dir*. Command line syntax is \"--output-dir path\". Call (main) again after setfing it"))
    ((not *input-dir*) (format t "You need to set value to *input-dir*. Command line syntax is \"--input-dir path\". Call (main) again after setfing it"))
    (t (start-compilation-server))))

(defun build! ()
  (setf *building?* t)
  (sb-ext:save-lisp-and-die "./image" :toplevel #'main :executable t))
