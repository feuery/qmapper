;; (let ((quicklisp-init (merge-pathnames "quicklisp/setup.lisp"
;;                                        (user-homedir-pathname))))
;;   (when (probe-file quicklisp-init)
;;     (load quicklisp-init)))

;; (ql:quickload "cl-arrows")
;; (ql:quickload "split-sequence")
;; (ql:quickload :fset)

(defpackage :qmapper.std
  (:use :common-lisp
	:cl-arrows)
  (:import-from :fset :empty-map :convert :with :lookup))

;; (use 'cl-arrows)

(in-package :qmapper.std)

(defmacro defmacro-export! (name &rest rst)
  `(progn
     ;(format t "Current package is ~a~%" *package*)
     (defmacro ,name ,@rst)
     (export (quote ,name))))

(defmacro-export! defvar-export!
    (name value)
  `(progn
     (defvar ,name ,value)
     (export (quote ,name))))

(defmacro-export! defun-export! (name &rest rst)
  `(progn
     (defun ,name ,@rst)
     (export (quote ,name))))

;; (defun export-all (pkg)
;;   (let ((pack (find-package pkg)))
;;     (do-all-symbols (sym pack)
;;       (if (and (eql (symbol-package sym) pack)
;; 	       ;; Tää rikkoo defcppclassin
;; 	       ;; (fboundp sym)
;; 	       (not (equalp (symbol-name sym) "PUBLIC")))
;; 	  (progn
;; 	    (format t "Exporting ~a~%" sym)
;; 	    (export sym)))
;;       ;; (if (and (eql (symbol-package sym) pack)
;;       ;; 	       (not (fboundp sym))
;;       ;; 	       (not (equalp (symbol-name sym) "PUBLIC")))
;;       ;; 	  (format t "~a is not fboundp~%" sym))
;;       )))

;; (define-macro (define-and-reg nameparams . body)
;;   (let ((name (car nameparams))
;; 	(params (cdr nameparams)))
;;     `(begin
;;        (define-public (,name ,@params)
;; 	 ,@body)
;;        (register-fn! ,(symbol->string name) ,name)
;;        ,name)))

(defun-export! dec (n)
  (- n 1))
(defun-export! inc (n)
  (+ n 1))

(defun-export! range (max)
  (defun-export! -range (max acc)
    (if (= 0 max)
	acc
	(-range (dec max) (cons max acc))))

  (-range max '()))

(defun-export! partial (f &rest args)
  (lambda (&rest rst-args)
    (apply f (concatenate 'list args rst-args))))

;; (defun-export! filter (f coll)
;;   (remove-if-not f coll))

(defun-export! repeatedly (fn n)
  (if (equalp n 0)
      '()
      (cons (funcall fn n) (repeatedly fn (dec n)))))

(defun-export! str (&rest strs)
  (apply (partial #'concatenate 'string)
	 (mapcar (lambda (o)
		   (write-to-string o :escape nil))
		   strs)))

(defun-export! drop (list n)
  (if (> n 0)
      (drop (cdr list)
	    (dec n))
      list))

(defun-export! partition-by2 (pred lst)
  (reduce (lambda (acc e)
	    (if (funcall pred e)
		(let* ((s (car acc))
		       (lst (cadr acc))
		       (rst (drop acc 2)))
		  (cons s (cons (cons e lst)
				rst)))
		(cons e (cons acc '()))))
	  lst :initial-value '()))

(defun-export! take (list n)
  (defun-export! -take (list n acc)
    (if (> n 0)
	(-take (cdr list)
	       (dec n)
	       (cons (car list) acc))
	acc))
  (reverse (-take list n '())))

(defun-export! take-while (pred coll)
  (labels ((-take-while (list acc)
	   (if (or (not list)
	       	   (not (funcall pred (car list))))
	       acc
	       (-take-while (cdr list)
	       		    (cons (car list) acc)))))
    (reverse (-take-while coll '()))))


(defun-export! partition-by (f coll)
  ;; (declare (optimize (debug 3)))
  (if coll
      (let* ((fst (first coll))
             (fv (funcall f fst))
             (run (cons fst (take-while (lambda (v)
					  (equalp fv (funcall f v)))
					(cdr coll))))
	     (run (if (= (length run) 1)
		      (car run)
		      run)))
        (cons run (partition-by f (drop coll (if (listp run)
						 (length run)
						 1)))))))

(defun-export! drop-while (fn lst)
  ;; (declare (optimize (debug 3)))
  ;; (break)
  (if (or (not (funcall fn (car lst)))
	  (not lst))
      lst
      (drop-while fn (cdr lst))))

(defun-export! plist-get (lst key)
  ;; (declare (optimize (debug 3)))
  ;; (break)
  (if (not lst)
      '()
      (let ((list (drop-while (lambda (e)
				(if (not (symbolp e))
				    (let ((result 
					   (not (equalp e key))))
				      result)
				    (let* ((e (symbol-name  e))
					   (key (symbol-name key))
					   (result 
					    (not (equalp e key))))
				      result))) lst)))
	(if (not list)
	    '()
	    (car
	     (drop list
		   1))))))


(defun-export! slice (l offset n)
  (take (drop l offset) n))

(defun-export! assoc-to-ind (lst ind value)
  (if (< ind (length lst))
      (let ((beginning (slice lst 0 ind))
	    (end (drop lst (inc ind))))
	(concatenate 'list beginning (list value) end))
      (error  "Overflow in assoc-to-ind")))

(defvar *this* nil)
(defvar *call-logs* t)
(export '*call-logs*)
(export '*this*)

(defun-export! alist-cons (k v list)
  (cons (cons k v) list))

(defun-export! sym (str)
  (let ((*read-eval* nil))
    (read-from-string str)))

(defun-export! zipmap (keys vals)
    (loop for k in keys
       for v in vals
       collecting (list k v)))

(defmacro-export! defcppclass (classname &rest rst)
  ;; (declare (optimize (debug 3)))
  ;; (break)
  (let* ((visibility-stripped (apply #'append
				     (mapcan #'cdr rst)))
	 ;; (removeMe (format t "visibility-stripped: ~a~%" visibility-stripped))
  	 (partitioned (partition-by #'listp visibility-stripped))
	 ;; (_ (prin1 partitioned))
	 (props (plist-get partitioned 'properties))
	 (fields (plist-get partitioned 'fields))
	 ;; (lololo (format t "fields on ~a~%" fields))
  	 (fields (reverse (->> (concatenate 'list fields
	 				    props)
	 		       (mapcar (lambda (field)
					 ;; oli cadr
	 				 (car field))))))
	 (fields-with-accessors (cons 'progn (concatenate 'list (->> fields
	 							     (mapcan (lambda (field)
	 								       (let* ((getter (sym (str (symbol-name classname)
	 												"-"
	 												(symbol-name field))))
	 									      (setter (sym (str "set-"
	 												(symbol-name classname)
	 												"-"
	 												(symbol-name field)
	 												"!"))))
										 (list `(defun-export! ,getter (this)
											  (unless this
											    (format t "~a failed, this is nil~%" (prin1-to-string (quote ,getter)))
											    ;; Let's try to cause a core dump...
											    (ext:quit -11))
											  (lookup this ',field))
										       `(defun-export! ,setter (this val)
											  (with this ',field val))))))))))
  	 (funcs (->> (plist-get partitioned 'functions)
		     (mapcar (lambda (fn)
			       (let ((name ;; (first fn)
				      
				      (sym (str (symbol-name classname)
						   	"-"
						   	(symbol-name (first fn)))))
				     (param-list (second fn))
				     (body (drop fn 2))
				     (this (sym (format nil "~a" (gensym)))))
				 `(defun-export! ,name (,this ,@param-list)
				    (let ((*this* ,this))
				      ,@body)))))))
	 (ctr-name (sym (str "make-" (symbol-name classname))))
	 (ctr `(defun-export! ,ctr-name (,@(reverse fields))
		 (let* ((hash (empty-map))
			(fields (list 'id ,@(mapcar (lambda (f)
		 				  (list 'quote f))
		 				fields)))
			(values (list (gensym) ,@fields))
			(fields-and-vals (cons (cons 'type-name (list ,(symbol-name classname)))
					       (zipmap fields values))))

		   (reduce (lambda (hashmap k-v)
			     (let ((k (car k-v))
				   (v (cadr k-v)))
			       (with hashmap k v))) fields-and-vals :initial-value hash)))))

		   ;; (labels ((doIt (fields values acc)
		   ;; 	      (if fields
		   ;; 		  (let ((f (car fields))
		   ;; 			(v (car values)))
		   ;; 		    (doIt (cdr fields)
		   ;; 			  (cdr values)
		   ;; 			  (cons `(,f . ,v) acc)))
		   ;; 		  acc)))
		   ;;   (doIt (list ,@(mapcar (lambda (f)
		   ;; 			     (list 'quote f))
		   ;; 			   fields))
		   ;; 	   (list ,@fields)
		   ;; 	   `((type-name . ,,(symbol-name classname)))))
	 ;; (pred-name (sym (str (symbol-name classname) "?")))
    `(progn
       ,ctr
       ,fields-with-accessors
       ,@funcs
       (symbol-name ',classname))))

;; (defcppclass lol
;;     (public
;;      (properties
;;       (a 0)
;;       (b 3))))

;; (make-lol 2 3)
;; (q-type-of (make-lol 3 3))
;; (set-prop (make-lol 666 2) 'a 222)

  ;; (reduce #'+ (range 3) :initial-value 4)

(defun-export! q-type-of  (obj-alist)
  ;; (format t "obj-alist on ~a~%" obj-alist)
  (handler-case
      (lookup obj-alist 'type-name)
    (SIMPLE-TYPE-ERROR (ste)
      (format t "ste bongattu, obj-alist on ~a~%" obj-alist)
      (explode))
    (SIMPLE-ERROR (lol)
      (format t "Virhe: obj-alist on ~a~%" (prin1-to-string obj-alist))
      (explode))))

(defun-export! get-prop  (obj-alist key)
  (let* ((key (if (symbolp key)
		  key
		  (intern (string-upcase key))))
	 (real-alist (convert 'list obj-alist))
	 (result (cdr (assoc key real-alist :test #'string=))))
    (format t "result is ~a~%" result)
    (if (symbolp result)
	;; if not for the prin1, this'd return rubbish strings to c...
	(prin1-to-string (symbol-name result))
	result)))

(defun-export! set-prop  (obj-alist key val)
  (with obj-alist key val))

;; (delete-duplicates (concatenate 'list (range 10) (range 12)) :test #'equalp)

(defun-export! keys  (a)
  (delete-duplicates
   (mapcar #'car a) :test #'equalp))

(defun-export! keys-str  (a)
  (mapcar #'symbol-name (keys a)))

(defun-export! prop-list?  (alist k)
  (listp (get-prop alist k)))

(defun-export! prop-str?  (alist k)
  (stringp (get-prop alist k)))

(defun-export! prop-number?  (alist k)
  (numberp (get-prop alist k)))

(defun-export! prop-bool?  (alist k)
  (let ((v (get-prop alist k)))
    (or (equalp v  t)
	(equalp v nil))))

(defun-export! prop-float?  (alist k)
  (let ((v (get-prop alist k)))
    (floatp v)))

(defun-export! prop-sym?  (alist k)
  (symbolp (get-prop alist k)))

;; (tree-il->scheme
;;  (macroexpand 
;;   '(defcppclass Layer
;; 		 (public
;; 		  (properties
;; 		   (std__string name "")
;; 		   (unsigned_char opacity 255)
;; 		   (bool visible #t)
;; 		   (std__vector<std__vector<Tile*>>* tiles '())
;; 		   (Map* parent nil))
;; 		  (functions
;; 		   (width ()
;; 			  (length (first (Layer-tiles (this)))))
;; 		   (height ()
;; 			   (length (caar (Layer-tiles (this))))))))))



;; (layer- (make-Layer 'p '(1 2 3) #t 244 "Layer")

(defmacro-export! condp (bin-op param1 &rest rst)
  "This is supposed to work like clojure's condp. This evaluates param1 only once.
(condp equalp \"Lollero ooo\"
       \"Lol\" 3
       \"Lollero\" 4
       'else) => else
and 

(condp equalp \"Lollero\"
       \"Lol\" 3
       \"Lollero\" 4) => 4"
  (let ((p1 (gensym)))
    (labels ((emit (params)
	       (if (equalp (length params) 1)
		   (car params)
		   (if params
		       (let ((else (emit (cddr params))))
			 `(if (,bin-op ,p1 ,(car params))
			      ,(cadr params)
			      ,else))))))
      `(let ((,p1 ,param1))
	 ,(emit rst)))))

;; (condp equalp "Lollero"
       
;;        "Lol" 3
;;        "Lollero" 4
;;        'pröp) => 4

;; (condp equalp "Lollero"
;;        "Lol" 3
;;        "Lollero" 4) => 4

(defun-export! get-list-index  (l pred)
  (if (not l)
      -1
      (if (funcall pred (car l))
          0
          (let ((result (get-list-index (cdr l) pred)))
            (if (= result -1)
                -1
                (1+ result))))))

(defun-export! drop-plist-keys  (lst)
  (labels ((dropper (lst acc count)
	     (if (not lst)
		 acc
		 (if (oddp count)
		     (dropper (cdr lst)
			      (cons (car lst) acc)
			      (inc count))
		     (dropper (cdr lst)
			      acc
			      (inc count))))))
    (reverse (dropper lst '() 0))))

(defun-export! drop-alist-keys (lst)
  (labels ((dropper2 (lst acc)
	     (if (not lst)
		 acc
		 (dropper2 (cdr lst)
			   (cons (cdar lst) acc)))))
    (reverse (dropper2 lst '()))))

;; (drop-alist-keys `((AAA . 3) (BEE 2))) => '(3 2)

;; (drop-plist-keys '(:lol 1 :lollo 3 'asd 33))  => '(1 3 33)

(defun-export! distance  (x1 y1 x2 y2)
  (sqrt (+ (* (- x2 x1) (- x2 x1)) (* (- y2 y1) (- y2 y1)))))

(defun-export! distance-pairs  (xy1 xy2)
  (distance (car xy1) (cadr xy1)
	    (car xy2) (cadr xy2)))

(defun-export! sort-by  (keyfn coll)
  (sort coll (lambda (x y)
	       (< (funcall keyfn x) (funcall keyfn y)))))

;; (sort-by #'car (list (list 3 1 2 22) (list 1 2 3) (list 5 4 22)))


(defun-export! find-nearest  (x y lst)
  (->> lst
       (mapcar (lambda (sprite)
		 (list (distance (sprite-x sprite)
				 (sprite-y sprite)
				 x y)
		       sprite)))
       (sort-by #'first)
       (first)
       (last)))

;; (find-nearest 1 1 '((3 3) (50 2) (0 1)))  => (0 1)

(defun-export! sublist  (list max-ind)
  (if (<= max-ind 0)
      '()
      (cons (car list)
	    (sublist (cdr list)
		     (dec max-ind)))))

(defun-export! sublist-from  (list min-ind)
  (drop list (inc min-ind)))

(defun-export! drop-list-i  (lst index)
  (concatenate 'list
	       (sublist lst index)
	       (sublist-from lst index)))

;; (drop-list-i (range 10) 1) => (1 3 4 5 6 7 8 9 10)


;; (defun-export! set-timeout  (seconds fn)
;;   (let ((fut (future
;; 	      (begin
;; 		(thread-sleep! (seconds->time
;; 				(+ (time->seconds (current-time))
;; 				   seconds)))
;; 		(fn)))))
;;     (touch fut)))

;; (defun-export! qloop  (l)
;;   (future
;;    (begin
;;      (while #t
;;        (l)
;;        (thread-sleep! (seconds->time (+ (time->seconds (current-time)) 0.1)))))))

;; Testikoodi enginelle:

;; (add-event "2001100545" "Sprite" "X" (lambda (id)
;; 				       (let ((x (get-prop id "Sprite" "x")))
;; 					 (if (> x 200)
;; 					     (scm-puts "Voitit pelin")
;; 					     (begin
;; 					       (scm-puts "Lol äxä on: ")
;; 					       (scm-puts (object->string x)))))))


;; (qloop (lambda ()
;; 	 (when (key-down? 'Key_Right)
;; 	   (let ((x (get-prop "2001100545" "Sprite" "x")))
;; 	     (set-prop "2001100545" "Sprite" "x" (+ x 10))))))

(format t "~%qmapper-std loaded")

;; (export-all :qmapper.std)
