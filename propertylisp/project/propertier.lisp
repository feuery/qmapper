;;;; propertier.lisp

;; (asdf:load-system "propertier")

(in-package #:propertier)

;; (ql:quickload "cl-arrows")
;; (ql:quickload "quickproject")
;; (ql:quickload "cl-fsnotify")
;; (ql:quickload :lparallel)
;; (require :quickproject)

;; (quickproject:make-project "~/Dropbox/qt-test/propertylisp/project/"
;; 			   :depends-on '(cl-arrows)
;; 			   :name "propertier"
;; 			   :author "feuer <feuer@feuerx.net>")

(defvar property-container '())

(defun read-file (file)
  (read-from-string 
   (format nil "~{~A~^~%~}"
	   (with-open-file (f file)
	     (loop for line = (read-line f nil :eof) 
		until (eq line :eof)
		collecting line)))))

;; (open-fsnotify

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

(defmacro fields (&rest forms)
  (dolist (l forms)
    (destructuring-bind (type name default) l
      (format t "~A ~A = ~A;~%" type name default))))

(defmacro properties (&rest forms)
  (dolist (l forms)
    (destructuring-bind (type name default) l
      ;; (push name propertynames)
      ;; (push type propertytypes)
      (push `(,name . ,type) property-container)
      (format t "~A get~A();~%
void set~A(~A val);~%

~A ~A_field = ~A;~%" type (string-capitalize name)
(string-capitalize name) type
type name default))))

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
      (format t "~A ~A(~A);~%" type name params))))

;; (defun get-properties (forms)
;;    (->> forms
;; 	(mapcar (lambda (visibility-form)
;; 		  (->> visibility-form
;; 		       (remove-if-not (lambda (x)
;; 					(equal (car x)
;; 					       'properties

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

  (format t "T t;~% *success = false; ~% return t; ~%}~%")
  (format t "const char* r[~D];~%" (length property-container)))
			
			

(defmacro defcppclass (&rest forms)
  (let* ((class (car forms)) 
	 (forms (cdr forms)))
    ;; (setf propertynames '())
    ;; (setf propertytypes '())
    (setf property-container '())
    (format t "#include<cstring>~%")
    (format t "class ~A {~%" class)
    (dolist (form forms)
      (eval form))
    (format t "public: ~%")
    (fill-property-names class)
    (format t "};~%")))

(defun headers (forms)
  (eval forms))

(defun source (forms)
  (let ((classname (cadr forms)))
    (format t "///////// CPP FILE STARTS HERE ~%")
    ;; (push `(,name . ,type) property-container)
    (dolist (x property-container)
      (let ((name (car x))
	    (type (cdr x)))
	(format t "~A ~A::get~A() {~% return ~A_field; ~%};~%"
		type classname (string-capitalize name) name)
	(format t "void ~A::set~A(~A val) {~% ~A_field = val; ~%}~%" classname (string-capitalize name) type name)))
    (format t "//////// CPP FILE ENDS HERE~%")))

(defvar running? t)
(defvar *compilation-queue* (make-queue))
;; (pop-queue *compilation-queue*)

;; (dolist (x (range 20))
;;   (push-queue (format nil "~A" x) *compilation-queue*))					  

(defun set-watch-on (dirpath)
  (setf running? t)
  (make-thread (lambda () 
		 (open-fsnotify)
		 (add-watch dirpath)
		 (loop while running?
		    do (let ((e (get-events)))
			 (dolist (event e)
			   (let ((path (car event))
				 (type (cdr event)))
			     ;; Tästä voisi yrittää tehdä jonkun ikuisen striimin?
			     ;; Josta sitten filtteröidään vain *.def - tiedostot compilation-queueen
			     (when (or (eq type :CREATE)
				       (eq type :MODIFY))
			       (format t "Pushing ~A to compilation queue~%" path)
			       (push-queue path *compilation-queue*))))
			 (sleep 3))))))

(defun start-compilation-server (input-dir output-dir)
  (set-watch-on input-dir)

  (make-thread (lambda ()
		 (loop while running?
		    do (let* ((to-compile (pop-queue *compilation-queue*))
			      (file-type (-> to-compile
					     reverse
					     (subseq 0 3)
					     reverse)))
			 (if (and (string= file-type "def")
				  (probe-file to-compile))
			     (let ((*print-case :downcase))
			       (format t "~%Compiling ~A~%~%" to-compile))
			     (format t "~A ei täyttänyt vaatimuksia ~%" to-compile))

			 (sleep 2))))))

;; (start-compilation-server "/home/feuer/Dropbox/qt-test/propertylisp/project" "")

;; (setf running? nil)

;; (queue-count *compilation-queue*)
;; (mapcar (lambda (x)
;; 	  (pop-queue *compilation-queue*)) (range 14))

;; (let ((*print-case* :downcase)
;;       (forms (read-file "./tile.def")))
;;   (headers forms)
;;   (source forms))
