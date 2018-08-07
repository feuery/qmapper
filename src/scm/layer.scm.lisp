;; (define-module (qmapper-layer)
;;   #:use-module (srfi srfi-1)
;;   #:use-module (qmapper-tile)
;;   #:use-module (qmapper-std))

(defcppclass Layer
  (public
   (properties
    (std__string name "")
    (unsigned_char opacity 255)
    (bool visible #t)
    (std__vector<std__vector<Tile*>>* tiles '())
    (Map* parent nil))
   (functions
    (width ()
	   (length (first (Layer-tiles (this)))))
    (height ()
	    (length (caar (Layer-tiles (this))))))))

(define-and-reg (make-tiles w h)
  (repeatedly (lambda (x)
		(repeatedly (lambda (y)
			      (make-Tile 0 0 #f 0)) h)) w))

;; (scm-puts "Loaded layer")
