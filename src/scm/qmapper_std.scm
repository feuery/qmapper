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
