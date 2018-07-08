;; (define-module (qmapper-std)
;;   #:export (dec range str set-timeout))

(use-modules (srfi srfi-1))
(use-modules (srfi srfi-18))
(use-modules (ice-9 futures))
(use-modules (srfi srfi-9 gnu))
(use-modules (language tree-il))

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
(define (inc n)
  (+ n 1))

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
  (if (null? lst)
      '()
      (let ((lst (drop-while (lambda (e)
			       (not (equal? e key))) lst)))
	(if (null? lst)
	    '()
	(car
	 (drop lst
	       1))))))

(define (slice l offset n)
  (take (drop l offset) n))

(define (assoc-to-ind lst ind value)
  (if (< ind (length lst))
      (let ((beginning (slice lst 0 ind))
	    (end (drop lst (inc ind))))
	(concatenate `(,beginning (,value) ,end)))
      (error "Overflow in assoc-to-ind")))

(define this (make-parameter #f))

(define-macro (defcppclass classname . rst)
  (let* ((visibility-stripped (apply append
				     (append-map cdr rst)))
  	 (partitioned (partition-by list? visibility-stripped))
	 (props (plist-get partitioned 'properties))
	 (fields (plist-get partitioned 'fields))
  	 (fields (->> (list fields
	 		    props)
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
			    (let ((name ;; (first fn)
				   
					(string->symbol (str (symbol->string classname)
						   	     "-"
						   	     (symbol->string (first fn))))
					)
				  (param-list (second fn))
				  (body (drop fn 2))
				  (this (gensym)))
			      `(define (,name ,this ,@param-list)
				 (parameterize ((this ,this))
				   ,@body))))))) ;) `(display (list ,funcs))))
	 (ctr-name (string->symbol (str "make-" (symbol->string classname))))
	 (pred-name (string->symbol (str (symbol->string classname) "?")))
	 (printer `(define (,(string->symbol (str "print-" (symbol->string classname))) o)
	 	     (str ,@(map (lambda (f-with-acc)
	 			   (let ((getter (second f-with-acc)))
	 			     `(str ,(symbol->string getter) ": "
	 				   ,(list getter 'o) "; "))) fields-with-accessors))))
	 )
    `(begin
       (define-immutable-record-type ,classname
	 (,ctr-name ,@fields)
	 ,pred-name
	 ,@fields-with-accessors)
       ,printer
       ,@funcs
       ,classname)))

(define-macro (condp bin-op param1 . rst)
  "This is supposed to work like clojure's condp. This evaluates param1 only once.
(condp equal? \"Lollero ooo\"
       \"Lol\" 3
       \"Lollero\" 4
       'else) => else
and 

(condp equal? \"Lollero\"
       \"Lol\" 3
       \"Lollero\" 4) => 4"
  (let ((p1 (gensym)))
    (define (emit params)
      (if (equal? (length params) 1)
	  (car params)
	  (if (not (null? params))
	      (let ((else (emit (cddr params))))
		`(if (,bin-op ,p1 ,(car params))
		     ,(cadr params)
		     ,else)))))
    `(let ((,p1 ,param1))
       ,(emit rst))))

;; (condp equal? (begin
;; 		(scm-puts "Evalled!")
;; 		"Lollero")
       
;;        "Lol" 3
;;        "Lollero" 4
;;        'pröp) => pröp

;; (condp equal? "Lollero"
;;        "Lol" 3
;;        "Lollero" 4) => 4

(define (get-list-index l pred)
  (if (null? l)
      -1
      (if (pred (car l))
          0
          (let ((result (get-list-index (cdr l) pred)))
            (if (= result -1)
                -1
                (1+ result))))))

(define (drop-plist-keys lst)
  (define (dropper lst acc count)
    (if (null? lst)
	acc
	(if (odd? count)
	    (dropper (cdr lst)
		     (cons (car lst) acc)
		     (inc count))
	    (dropper (cdr lst)
		     acc
		     (inc count)))))
  (reverse (dropper lst '() 0)))

;; (drop-plist-keys '(:lol 1 :lollo 3 'asd 33))  => '(1 3 33)

(define (distance x1 y1 x2 y2)
  (sqrt (+ (* (- x2 x1) (- x2 x1)) (* (- y2 y1) (- y2 y1)))))

(define (distance-pairs xy1 xy2)
  (distance (car xy1) (cadr xy1)
	    (car xy2) (cadr xy2)))

(define (partial f . args)
  (lambda (. rst-args)
    (apply f (concatenate (list args rst-args)))))

(define cmp (make-parameter <))

(define (sort-by keyfn coll)
  (sort coll (lambda (x y)
	       ((cmp) (keyfn x) (keyfn y)))))

;; (sort-by car (list (list 3 1 2 22) (list 1 2 3) (list 5 4 22))))


(define (find-nearest x y lst)
  (->> lst
       (map (lambda (sprite)
	      (list (distance (sprite-x sprite)
			      (sprite-y sprite)
			      x y)
		    sprite)))
       (sort-by first)
       (first)
       (last)))

;; (find-nearest 1 1 '((3 3) (50 2) (0 1)))  => (0 1)


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
