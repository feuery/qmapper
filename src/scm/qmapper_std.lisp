;; (let ((quicklisp-init (merge-pathnames "quicklisp/setup.lisp"
;;                                        (user-homedir-pathname))))
;;   (when (probe-file quicklisp-init)
;;     (load quicklisp-init)))

;; (ql:quickload "cl-arrows")
;; (ql:quickload "split-sequence")

(defpackage :qmapper.std
  (:use :common-lisp
	:cl-arrows))

;; (use 'cl-arrows)

(in-package :qmapper.std)

(defun export-all (pkg)
  (let ((pack (find-package pkg)))
    (do-all-symbols (sym pack)
      (if (and (eql (symbol-package sym) pack)
	       (not (equalp (symbol-name sym) "PUBLIC")))
	  (export sym)))))

;; (define-macro (define-and-reg nameparams . body)
;;   (let ((name (car nameparams))
;; 	(params (cdr nameparams)))
;;     `(begin
;;        (define-public (,name ,@params)
;; 	 ,@body)
;;        (register-fn! ,(symbol->string name) ,name)
;;        ,name)))

(defun dec (n)
  (- n 1))
(defun inc (n)
  (+ n 1))

(defun range (max)
  (defun -range (max acc)
    (if (= 0 max)
	acc
	(-range (dec max) (cons max acc))))

  (-range max '()))

(defun partial (f &rest args)
  (lambda (&rest rst-args)
    (apply f (concatenate 'list args rst-args))))

(defun filter (f coll)
  (remove-if-not f coll))

(defun repeatedly (fn n)
  (if (equalp n 0)
      '()
      (cons (funcall fn n) (repeatedly fn (dec n)))))

(defun str (&rest strs)
  (apply (partial #'concatenate 'string)
	 (mapcar (lambda (o)
		   (write-to-string o :escape nil))
		   strs)))

(defun drop (list n)
  (if (> n 0)
      (drop (cdr list)
	    (dec n))
      list))

(defun partition-by2 (pred lst)
  (reduce (lambda (acc e)
	    (if (funcall pred e)
		(let* ((s (car acc))
		       (lst (cadr acc))
		       (rst (drop acc 2)))
		  (cons s (cons (cons e lst)
				rst)))
		(cons e (cons acc '()))))
	  lst :initial-value '()))

(defun take (list n)
  (defun -take (list n acc)
    (if (> n 0)
	(-take (cdr list)
	       (dec n)
	       (cons (car list) acc))
	acc))
  (reverse (-take list n '())))

(defun take-while (pred coll)
  (defun -take-while (list acc)
    (if (or (not list)
	    (not (funcall pred (car list))))
	acc
	(-take-while (cdr list)
		     (cons (car list) acc))))
  (reverse (-take-while coll '())))

(defun partition-by (f coll)
  (declare (optimize (debug 3)))
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

(defun drop-while (fn lst)
  (if (or (not (funcall fn (car lst)))
	  (not lst))
      lst
      (drop-while fn (cdr lst))))

(defun plist-get (lst key)
  (if (not lst)
      '()
      (let ((list (drop-while (lambda (e)
				(not (equalp e key))) lst)))
	(if (not list)
	    '()
	    (car
	     (drop list
		   1))))))


(defun slice (l offset n)
  (take (drop l offset) n))

(defun assoc-to-ind (lst ind value)
  (if (< ind (length lst))
      (let ((beginning (slice lst 0 ind))
	    (end (drop lst (inc ind))))
	(concatenate 'list beginning (list value) end))
      (error  "Overflow in assoc-to-ind")))

(defvar *this* nil)

(defun alist-cons (k v list)
  (cons (cons k v) list))

(defun sym (str)
  (let ((*read-eval* nil))
    (read-from-string str)))

(defvar *tulos-lista* `(FIELDS ((STD__MAP<STD__STRING___LAYER*>* LAYERS 'NIL)
				(STD__MAP<STD__STRING___MAP*>* MAPS 'NIL)
				(STD__MAP<STD__STRING___SCRIPT*>* SCRIPTS 'NIL)
				(STD__MAP<STD__STRING___TILE*>* TILES 'NIL)
				(STD__MAP<STD__STRING___TILESET*>* TILESETS 'NIL)
				(SCM CHOSENMAP 'NIL) (INT CHOSENLAYERIND -1)
				(SCM CHOSENTILESET 'NIL) (SCM CHOSENTILE 'NIL)
				(SCM STDVERTEXSHADER 'NIL) (SCM STDFRAGMENTSHADER 'NIL)
				(SCM STDTILEVIEWFRAGSHADER 'NIL))
			       FUNCTIONS
			       ((CONTENTS-STDFRAGMENT NIL
						      (SCRIPT-CONTENTS (ROOT-STDFRAGMENTSHADER *THIS*)))
				(CONTENTS-STDVERTEX NIL
						    (SCRIPT-CONTENTS (ROOT-STDVERTEXSHADER *THIS*)))
				(CONTENTS-TILEVIEWFRAGSHADER NIL
							     (SCRIPT-CONTENTS (ROOT-STDTILEVIEWFRAGSHADER *THIS*)))
				(REGISTRYSIZE NIL
					      (+ (LENGTH (ROOT-LAYERS *THIS*))
						 (LENGTH (ROOT-MAPS *THIS*))
						 (LENGTH (ROOT-SCRIPTS *THIS*))
						 (LENGTH (ROOT-TILES *THIS*))
						 (LENGTH (ROOT-TILESETS *THIS*))))
				(FETCHREGISTER (TYPE ID)
					       (PLIST-GET
						(CONDP EQUALP TYPE Layer (ROOT-LAYERS *THIS*) Map
						       (ROOT-MAPS *THIS*) Script (ROOT-SCRIPTS *THIS*)
						       Tile (ROOT-TILES *THIS*) Tilesets
						       (ROOT-TILESETS *THIS*))
						ID))
				(FINDNS (NS)
					(LET ((SCRIPTS (ROOT-SCRIPTS *THIS*)))
					  (FIRST
					   (FILTER (LAMBDA (SCRIPT) (EQUALP (SCRIPT-NS SCRIPT)))
						   SCRIPTS))))
				(SAVENS (NS CONTENT)
					(LET* ((SCRIPTS (ROOT-SCRIPTS *THIS*))
					       (INDEX
						(GET-LIST-INDEX (ROOT-SCRIPTS *THIS*)
								(LAMBDA (SC)
								  (EQUALP (SCRIPT-NS SC) NS))))
					       (NEW-SCRIPT
						(-> (NTH INDEX SCRIPTS)
						    (SET-SCRIPT-CONTENTS! CONTENT)))
					       (SCRIPTS (ASSOC-TO-IND SCRIPTS INDEX NEW-SCRIPT)))
					  (SET-ROOT-SCRIPTS *THIS* SCRIPTS)))
				(CONTAINSNS (NS)
					    (NOT
					     (FILTER (LAMBDA (SCRIPT) (EQUALP (SCRIPT-NS SCRIPT)))
						     SCRIPTS)))
				(REGISTRYOF (TYPE)
					    (LET ((RESULT
						   (COND ((EQUALP TYPE Layer) (ROOT-LAYERS *THIS*))
							 ((EQUALP TYPE Map) (ROOT-MAPS *THIS*))
							 ((EQUALP TYPE Script)
							  (ROOT-SCRIPTS *THIS*))
							 ((EQUALP TYPE Tile) (ROOT-TILES *THIS*))
							 ((EQUALP TYPE Tileset)
							  (ROOT-TILESETS *THIS*))
							 (T
							  (SCM-PUTS
							   registryOf got unrecognized type)
							  (SCM-PUTS TYPE) NIL))))
					      (IF RESULT
						  (DROP-PLIST-KEYS RESULT)
						  RESULT)))
				(REGISTRYTOLIST NIL
						(DROP-PLIST-KEYS
						 (CONCATENATE 'LIST (ROOT-LAYERS *THIS*)
							      (ROOT-MAPS *THIS*) (ROOT-SCRIPTS *THIS*)
							      (ROOT-TILES *THIS*)
							      (ROOT-TILESETS *THIS*)))))))

(defvar *testilista* `(FIELDS (STD__MAP<STD__STRING___LAYER*>* LAYERS 'NIL)
			       (STD__MAP<STD__STRING___MAP*>* MAPS 'NIL)
			       (STD__MAP<STD__STRING___SCRIPT*>* SCRIPTS 'NIL)
			       (STD__MAP<STD__STRING___TILE*>* TILES 'NIL)
			       (STD__MAP<STD__STRING___TILESET*>* TILESETS 'NIL)
			       (SCM CHOSENMAP 'NIL) (INT CHOSENLAYERIND -1)
			       (SCM CHOSENTILESET 'NIL) (SCM CHOSENTILE 'NIL)
			       (SCM STDVERTEXSHADER 'NIL) (SCM STDFRAGMENTSHADER 'NIL)
			       (SCM STDTILEVIEWFRAGSHADER 'NIL)

			       FUNCTIONS
			       (CONTENTS-STDFRAGMENT NIL
						     (SCRIPT-CONTENTS (ROOT-STDFRAGMENTSHADER *THIS*)))
			       (CONTENTS-STDVERTEX NIL
						   (SCRIPT-CONTENTS (ROOT-STDVERTEXSHADER *THIS*)))
			       (CONTENTS-TILEVIEWFRAGSHADER NIL
							    (SCRIPT-CONTENTS (ROOT-STDTILEVIEWFRAGSHADER *THIS*)))
			       (REGISTRYSIZE NIL
					     (+ (LENGTH (ROOT-LAYERS *THIS*))
						(LENGTH (ROOT-MAPS *THIS*))
						(LENGTH (ROOT-SCRIPTS *THIS*))
						(LENGTH (ROOT-TILES *THIS*))
						(LENGTH (ROOT-TILESETS *THIS*))))
			       (FETCHREGISTER (TYPE ID)
					      (PLIST-GET
					       (CONDP EQUALP TYPE Layer (ROOT-LAYERS *THIS*) Map
						      (ROOT-MAPS *THIS*) Script (ROOT-SCRIPTS *THIS*)
						      Tile (ROOT-TILES *THIS*) Tilesets
						      (ROOT-TILESETS *THIS*))
					       ID))
			       (FINDNS (NS)
				       (LET ((SCRIPTS (ROOT-SCRIPTS *THIS*)))
					 (FIRST
					  (FILTER (LAMBDA (SCRIPT) (EQUALP (SCRIPT-NS SCRIPT)))
						  SCRIPTS))))
			       (SAVENS (NS CONTENT)
				       (LET* ((SCRIPTS (ROOT-SCRIPTS *THIS*))
					      (INDEX
					       (GET-LIST-INDEX (ROOT-SCRIPTS *THIS*)
							       (LAMBDA (SC)
								 (EQUALP (SCRIPT-NS SC) NS))))
					      (NEW-SCRIPT
					       (-> (NTH INDEX SCRIPTS)
						   (SET-SCRIPT-CONTENTS! CONTENT)))
					      (SCRIPTS (ASSOC-TO-IND SCRIPTS INDEX NEW-SCRIPT)))
					 (SET-ROOT-SCRIPTS *THIS* SCRIPTS)))
			       (CONTAINSNS (NS)
					   (NOT
					    (FILTER (LAMBDA (SCRIPT) (EQUALP (SCRIPT-NS SCRIPT)))
						    SCRIPTS)))
			       (REGISTRYOF (TYPE)
					   (LET ((RESULT
						  (COND ((EQUALP TYPE Layer) (ROOT-LAYERS *THIS*))
							((EQUALP TYPE Map) (ROOT-MAPS *THIS*))
							((EQUALP TYPE Script)
							 (ROOT-SCRIPTS *THIS*))
							((EQUALP TYPE Tile) (ROOT-TILES *THIS*))
							((EQUALP TYPE Tileset)
							 (ROOT-TILESETS *THIS*))
							(T
							 (SCM-PUTS
							  registryOf got unrecognized type)
							 (SCM-PUTS TYPE) NIL))))
					     (IF RESULT
						 (DROP-PLIST-KEYS RESULT)
						 RESULT)))
			       (REGISTRYTOLIST NIL
					       (DROP-PLIST-KEYS
						(CONCATENATE 'LIST (ROOT-LAYERS *THIS*)
							     (ROOT-MAPS *THIS*) (ROOT-SCRIPTS *THIS*)
							     (ROOT-TILES *THIS*)
							     (ROOT-TILESETS *THIS*))))))
 
(defmacro defcppclass (classname &rest rst)
  (declare (optimize (debug 3)))
  (let* ((visibility-stripped (apply #'append
				     (mapcan #'cdr rst)))
	 ;; (removeMe (format t "visibility-stripped: ~a~%" visibility-stripped))
  	 (partitioned (partition-by #'listp visibility-stripped))
	 ;; (_ (prin1 partitioned))
	 (props (plist-get partitioned 'properties))
	 (fields (plist-get partitioned 'fields))
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
										 (list `(defun ,getter (this)
											  (cdr (assoc ',field this)))
										       `(defun ,setter (this val)
											  (alist-cons ',field val this))))))))))
  	 (funcs (->> (plist-get partitioned 'functions)
		     (mapcar (lambda (fn)
			       (let ((name ;; (first fn)
				      
				      (sym (str (symbol-name classname)
						   	"-"
						   	(symbol-name (first fn)))))
				     (param-list (second fn))
				     (body (drop fn 2))
				     (this (gensym)))
				 `(defun ,name (,this ,@param-list)
				    (let ((*this* ,this))
				      ,@body)))))))
	 (ctr-name (sym (str "make-" (symbol-name classname))))
	 (ctr `(defun ,ctr-name (,@(reverse fields))
		 (defun doIt (fields values acc)
		   (if fields
		       (let ((f (car fields))
			     (v (car values)))
			 (doIt (cdr fields)
			       (cdr values)
			       (cons `(,f . ,v) acc)))
		       acc))
		 (doIt (list ,@(mapcar (lambda (f)
					 (list 'quote f))
				       fields))
		       (list ,@fields)
		       `((type-name . ,,(symbol-name classname))))))
	 ;; (pred-name (sym (str (symbol-name classname) "?")))
	 )
    `(progn
       ,ctr
       ,fields-with-accessors
       ,@funcs
       (symbol-name ',classname))))

(defun q-type-of  (obj-alist)
  (handler-case
      (cdr (assoc 'type-name obj-alist))
    (SIMPLE-TYPE-ERROR (ste)
      (format t "ste bongattu, obj-alist on ~a~%" obj-alist)
      (explode))
    (SIMPLE-ERROR (lol)
      (format t "Virhe: obj-alsit on ~a~%" (prin1-to-string obj-alist)))))

(defun get-prop  (obj-alist key)
  (cdr (assoc (intern (string-upcase key)) obj-alist)))

(defun set-prop  (obj-alist key val)
  (alist-cons key val obj-alist))

(delete-duplicates (concatenate 'list (range 10) (range 12)) :test #'equalp)

(defun keys  (a)
  (delete-duplicates
   (mapcar #'car a) :test #'equalp))

(defun keys-str  (a)
  (mapcar #'symbol-name (keys a)))

(defun prop-list?  (alist k)
  (listp (get-prop alist k)))

(defun prop-str?  (alist k)
  (stringp (get-prop alist k)))

(defun prop-number?  (alist k)
  (numberp (get-prop alist k)))

(defun prop-bool?  (alist k)
  (let ((v (get-prop alist k)))
    (or (equalp v  t)
	(equalp v nil))))

(defun prop-float?  (alist k)
  (let ((v (get-prop alist k)))
    (floatp v)))

(defun prop-sym?  (alist k)
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

(defmacro condp (bin-op param1 &rest rst)
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
    (defun emit (params)
      (if (equalp (length params) 1)
	  (car params)
	  (if params
	      (let ((else (emit (cddr params))))
		`(if (,bin-op ,p1 ,(car params))
		     ,(cadr params)
		     ,else)))))
    `(let ((,p1 ,param1))
       ,(emit rst))))

;; (condp equalp "Lollero"
       
;;        "Lol" 3
;;        "Lollero" 4
;;        'pröp) => 4

;; (condp equalp "Lollero"
;;        "Lol" 3
;;        "Lollero" 4) => 4

(defun get-list-index  (l pred)
  (if (not l)
      -1
      (if (funcall pred (car l))
          0
          (let ((result (get-list-index (cdr l) pred)))
            (if (= result -1)
                -1
                (1+ result))))))

(defun drop-plist-keys  (lst)
  (defun dropper (lst acc count)
    (if (not lst)
	acc
	(if (oddp count)
	    (dropper (cdr lst)
		     (cons (car lst) acc)
		     (inc count))
	    (dropper (cdr lst)
		     acc
		     (inc count)))))
  (reverse (dropper lst '() 0)))

(defun drop-alist-keys (lst)
  (defun dropper2 (lst acc)
    (if (not lst)
	acc
	(dropper2 (cdr lst)
		 (cons (cdar lst) acc))))
  (reverse (dropper2 lst '())))

;; (drop-alist-keys `((AAA . 3) (BEE 2))) => '(3 2)

;; (drop-plist-keys '(:lol 1 :lollo 3 'asd 33))  => '(1 3 33)

(defun distance  (x1 y1 x2 y2)
  (sqrt (+ (* (- x2 x1) (- x2 x1)) (* (- y2 y1) (- y2 y1)))))

(defun distance-pairs  (xy1 xy2)
  (distance (car xy1) (cadr xy1)
	    (car xy2) (cadr xy2)))

(defun sort-by  (keyfn coll)
  (sort coll (lambda (x y)
	       (< (funcall keyfn x) (funcall keyfn y)))))

;; (sort-by #'car (list (list 3 1 2 22) (list 1 2 3) (list 5 4 22)))


(defun find-nearest  (x y lst)
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

(defun sublist  (list max-ind)
  (if (<= max-ind 0)
      '()
      (cons (car list)
	    (sublist (cdr list)
		     (dec max-ind)))))

(defun sublist-from  (list min-ind)
  (drop list (inc min-ind)))

(defun drop-list-i  (lst index)
  (concatenate 'list
	       (sublist lst index)
	       (sublist-from lst index)))

;; (drop-list-i (range 10) 1) => (1 3 4 5 6 7 8 9 10)


;; (defun set-timeout  (seconds fn)
;;   (let ((fut (future
;; 	      (begin
;; 		(thread-sleep! (seconds->time
;; 				(+ (time->seconds (current-time))
;; 				   seconds)))
;; 		(fn)))))
;;     (touch fut)))

;; (defun qloop  (l)
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

(export-all :qmapper.std)
