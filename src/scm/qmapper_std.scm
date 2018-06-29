;; (define-module (qmapper-std)
;;   #:export (dec range str set-timeout))

(use-modules (srfi srfi-1))
(use-modules (srfi srfi-18))
(use-modules (ice-9 futures))
(use-modules (srfi srfi-9 gnu))

;; Threading macros copy-pasted from https://github.com/joshwalters/guile-pipe
;; thus making this GPLv3

(define-syntax ->
  (syntax-rules ()
                ((_) #f)
                ((_ x) x)
                ((_ x (f . (f-rest ...))) (f x f-rest ...))
                ((_ x f) (f x))
                ((_ x (f . (f-rest ...)) rest ...) (-> (f x f-rest ...) rest ...))
                ((_ x f rest ...) (-> (f x) rest ...))
                ))
(define-syntax ->>
  (syntax-rules ()
                ((_) #f)
                ((_ x) x)
                ((_ x (f ...)) (f ... x))
                ((_ x f) `(f x))
                ((_ x (f ...) rest ...) (->> (f ... x) rest ...))
		((_ x f rest ...) (->> (f x) rest ...))))

(define (dec n)
  (- n 1))

(define (range max)
  (define (-range max acc)
    (if (= 0 max)
	acc
	(-range (dec max) (cons max acc))))

  (-range max '()))

(define (str . strs)
  (string-join strs ""))

(define (partition-by pred lst)
  (fold (lambda (e acc)
	  (if (not (pred e))
	      (cons e (cons '() acc))
	      (let* ((s (car acc))
		     (lst (cadr acc))
		     (rst (drop acc 2)))
		(cons s (cons (cons e lst)
			      rst)))))
	'()
	lst
	))

(define (plist-get lst key)
  (car
   (drop (drop-while (lambda (e)
		      (not (equal? e key))) lst)
	 1)))

(define this (make-parameter #f))

(define-macro (defcppclass classname . rst)
  (let* ((visibility-stripped (apply append
				     (append-map cdr rst)))
  	 (partitioned (partition-by list? visibility-stripped))
	 (props (plist-get partitioned 'properties))
  	 (fields (->> (plist-get partitioned 'fields)
		      (list props)
		      concatenate
		      (map (lambda (field)
			     (second field)))))
	 (fields-with-accessors (->> fields
				     (map (lambda (field)
					    (let* ((getter (string->symbol (str (symbol->string classname)
										"-"
										(symbol->string field))))
						   (setter (string->symbol (str "set-"
										(symbol->string classname)
										"-"
										(symbol->string field)
										"!"))))
					      (list field getter setter))))))
  	 (funcs (->> (plist-get partitioned 'functions)
		     (map (lambda (fn)
			    (let ((name (first fn))
				  (param-list (second fn))
				  (body (drop fn 2))
				  (this (gensym)))
			      `(define (,name ,this ,@param-list)
				 (parameterize ((this ,this))
				   ,@body)))))))
	 (ctr-name (string->symbol (str "make-" (symbol->string classname))))
	 (pred-name (string->symbol (str (symbol->string classname) "?")))
	 (printer `(define (,(string->symbol (str "print-" (symbol->string classname))) o)
		     (str ,@(map (lambda (f-with-acc)
				   (let ((getter (second f-with-acc)))
				     `(str ,(symbol->string getter) ": "
					   ,(list getter 'o) "; "))) fields-with-accessors)))))
    `(begin
       (define-immutable-record-type ,classname
	 (,ctr-name ,@fields)
	 ,pred-name
	 ,@fields-with-accessors)
       ,@funcs
       ,printer)))

;; "DEMO defcppclassista: " 
;; (defcppclass Tileset
;;   (public
;;    (functions
;;     (findNearest (x y)
;; 		 (this))
;;     (parent (p) (+ 2 3))
;;     (width (a b)
;; 	   (* 2 2))
;;     (height (d) '())
;;     (resize (w
;; 	     h
;; 	     vAnchor
;; 	     hAnchor)
;; 	    (display "Lollero"))
;;     (parent (g p)
;; 	    'sdffd))
;;    (properties
;;     (int lollero-propsi "ksdfpo"))
;;    (fields
;;     (int name 0)
;;     (int vertexShader 0)
;;     (int fragmentShader 0)
;;     (int tiles 0))))

(define (set-timeout seconds fn)
  (let ((fut (future
	      (begin
		(thread-sleep! (seconds->time
				(+ (time->seconds (current-time))
				   seconds)))
		(fn)))))
    (touch fut)))

(define (qloop l)
  (future
   (begin
     (while #t
       (l)
       (thread-sleep! (seconds->time (+ (time->seconds (current-time)) 0.1)))))))

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
