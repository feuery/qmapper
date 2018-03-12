;; (define-module (qmapper-std)
;;   #:export (dec range str set-timeout))

(use-modules (srfi srfi-1))
(use-modules (srfi srfi-18))
(use-modules (ice-9 futures))

(define (dec n)
  (- n 1))

(define (range max)
  (define (-range max acc)
    (if (= 0 max)
	acc
	(-range (dec max) (cons max acc))))

  (-range max '()))

(define (str . strs)
  (string-join strs))

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
