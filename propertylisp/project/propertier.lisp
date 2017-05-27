;;;; propertier.lisp

(in-package #:propertier)

;; (ql:quickload "cl-arrows")
;; (ql:quickload "quickproject")
;; (require :quickproject)

;; (quickproject:make-project "~/Dropbox/qt-test/propertylisp/project/"
;; 			   :depends-on '(cl-arrows)
;; 			   :name "propertier"
;; 			   :author "feuer <feuer@feuerx.net>")

(defun read-file (file)
  (read-from-string 
   (format nil "~{~A~^~%~}"
	   (with-open-file (f file)
	     (loop for line = (read-line f nil :eof) 
		until (eq line :eof)
		collecting line)))))

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

;; (defvar propertynames '())
;; (defvar propertytypes '())

(defvar property-container '())

(let ((*print-case* :downcase)
      (forms (read-file "./tile.def")))
  (headers forms))
