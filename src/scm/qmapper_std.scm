;; This isn't currently loaded by default
;; Eval these with geiser if you need them

(define (dec n)
  (- n 1))

(define (range max)
  (define (-range max acc)
    (if (= 0 max)
	acc
	(-range (dec max) (cons max acc))))

  (-range max '()))