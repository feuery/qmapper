;; (define-module (qmapper-map)
;;   #:use-module (srfi srfi-1)
;;   #:use-module (qmapper-std)
;;   #:use-module (qmapper-root)
;;   #:use-module (qmapper-tile)
;;   #:use-module (qmapper-layer))

(defcppclass Map
  (public 
   (properties
    (std__string name "Map 1")
    (std__vector<Layer*>* layers '())
    (list spritesAndAnimatedsprites '())
    (root parent nil))
   (functions
    (findNearest (x y)
		 ;; Let's search the nearest animatedsprite or sprite
		 (find-nearest x y (Map-spritesAndAnimatedsprites (this))))
    (width ()
	   (Layer-width (first (Map-layers (this)))))
    (height ()
	    (Layer-height (first (Map-layers (this)))))
    ;; this'll be fun
    (resize (w
	     h
	     vAnchor
	     hAnchor)
	    ;; TODO IMPLEMENT
	    (error "Don't call Map-resize yet!")))))

(define-and-reg (make-map-with-layers name w h layer-count)
  (make-Map name (repeatedly (lambda (i)
			       (make-Layer (str (number->string i) "th layer")
					   255
					   #t
					   (make-tiles w h)
					   #f)) layer-count)
	    '()
	    #f))

(define-and-reg (alist-update alist k fn)
  (alist-cons k (fn (cdr (assoc k alist))) alist))

;; (alist-update `((a . ,(* 3 3))
;; 		(b . ,(+ 3 2))) 'a (partial * 3))

(define-and-reg (alist-get a k)
  (cdr (assoc k a)))

(define-and-reg (alist-update-in alist path fn)
  (let ((key (car path))
	(path (cdr path)))
    (if (null? path)
	(alist-update alist key fn)
	(alist-update-in (alist-get alist key) path fn))))

(define-and-reg (assoc-in lst path val)
  (let ((ind (car path))
	(path (cdr path)))
    (if (null? path)
	(assoc-to-ind lst ind val)
	(assoc-to-ind lst ind
		      (assoc-in (list-ref lst ind) path val)))))

(define-and-reg (get-in lst path)
  (let ((ind (car path))
	(path (cdr path)))
    (if (null? path)
	(list-ref lst ind)
	(get-in (list-ref lst ind) path))))

(define-and-reg (get-tile-at map layer x y)
  (-> map
      Map-layers
      (get-in (list layer x y))))

(define-and-reg (find-index-of alist el)
  (let ((result 
	 (filter
	  (lambda (m)
	    (equal? (cdr m)
		    el))
	  alist)))
    (if (null? result)
	#f
	(caar result))))

;; (let* ((s (gensym))
;;        (alist `((,s . 3))))
;;   (equal? (find-index-of alist 3) s))


(define-and-reg (push-sprite map sprite)
  (let ((k (gensym)))
    (alist-update map 'spritesAndAnimatedsprites
		  (partial alist-cons k sprite))))
					   


(define-and-reg (index-of-chosen-map root)
  (find-index-of (root-maps root)
		 (root-chosenMap root)))

(define-and-reg (get-tile-at-chosen-map root x y)
  (let ((m (root-chosenMap root))
	(l (root-chosenLayerInd root)))
    (get-tile-at m l x y)))
    

(define-and-reg (set-tile-inner map layer x y tile)
  (set-Map-layers! map
		   (-> map
		       Map-layers
		       (assoc-in (list layer x y) tile))))

(define-and-reg (set-tile-at root x y tile)
  (set-tile-inner (root-chosenMap root)
		  (root-chosenLayerInd root)
		  x
		  y
		  tile))

(define-and-reg (set-chosen-tile-at root x y)
  (set-tile-at root x y (root-chosenTile root)))

(define-and-reg (set-tile-at-chosen-map root x y tile)
  (let* ((l (root-chosenLayerInd root))
	 (m (set-tile-at root x y tile))	 
	 (maps-index (find-index-of (root-maps root) m)))
    (-> root
	(set-root-chosenMap! m)
	(set-root-maps! (alist-cons maps-index m (root-maps root))))))

(define-and-reg (push-selected-map root map)
  (let* ((index (index-of-chosen-map root))
	 ;; let's prepare the chosenmap
	 (root (set-root-chosenMap! root map)))
    (set-root-maps! root
		    ;; and let's cons the chosenmap to its old index
		    (alist-cons index map
				
				root))))

(define-and-reg (set-tile-rotation-at root x y rotation)
  (let ((map (-> (root-chosenMap root)
		 (get-tile-at (root-chosenLayerInd root) x y)
		 (set-Tile-rotation! rotation))))
    (push-selected-map root map)))

(define-and-reg (add-layer root map-index)
  (let ((maps (-> (root-maps root)
		  (alist-update map-index (lambda (m)
					    (let ((layers (Map-layers m))
						  (w (Map-width m))
						  (h (Map-height m)))
					      (set-Map-layers! m (cons (make-Layer (str (number->string (length layers)) "th layer")
										   255
										   #t
										   (make-tiles w h)
										   #f) layers))))))))
    (set-root-maps! root maps)))

(define-and-reg (delete-layer root map-index layer-index)
  (set-root-maps! root
		  (-> (root-maps root)
		      (alist-update map-index (lambda (m)
						(set-Map-layers! m (drop-list-i (Map-layers m) layer-index)))))))
	 

;; (scm-puts "map ladattu")
