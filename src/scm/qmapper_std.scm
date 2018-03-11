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


;; Testikoodi enginelle:

;; (define (keyloop)
;;   (when (key-down? 'Key_Right)
;;     (let ((x (get-prop "2001100545" "Sprite" "x")))
;;       (set-prop "2001100545" "Sprite" "x" (+ x 10))))
;;   (set-timeout 1 keyloop))

;; (add-event "2001100545" "Sprite" "x" (lambda (id)
;; 				       (let ((x (get-prop id "Sprite" "x")))
;; 					 (if (> x 200)
;; 					     (display "Voitit pelin")))))

;; (keyloop)
