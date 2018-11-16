(defpackage :qmapper.map
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.tileset
	:qmapper.script
	:qmapper.layer
	:qmapper.root
	:qmapper.export
	:qmapper.tile)
  (:import-from :qmapper.export :clear-lisp-drawingqueue :add-lambda-to-drawingqueue))

(in-package :qmapper.map)

(defcppclass Map
    (public 
     (properties
      (name "Map 1")
      (layers '())
      (spritesAndAnimatedsprites '())
      ;; (parent nil)
      )
     (functions
      (findNearest (x y)
		   ;; Let's search the nearest animatedsprite or sprite
		   (find-nearest x y (Map-spritesAndAnimatedsprites *this*)))
      (width ()
	     (let ((layers (first (Map-layers *this*))))
	       (Layer-width layers)))
      (height ()
	      (Layer-height (first (Map-layers *this*))))
      ;; this'll be fun
      (resize (w
	       h
	       vAnchor
	       hAnchor)
	      ;; TODO IMPLEMENT
	      (error "Don't call Map-resize yet!")))))

(defun-export! make-map-with-layers (name w h layer-count)
  (make-map name (repeatedly (lambda (i)
			       (let ((l 
				      (make-Layer (str (prin1-to-string i) "th layer")
						  255
						  t
						  (make-tiles w h))))
				 ;(format t "making layer ~a ~%" l)
				 l)) layer-count)
	    '()))

(defun-export! alist-update (alist k fn)
  (alist-cons k (funcall fn (cdr (assoc k alist))) alist))

;; (alist-update `((a . ,(* 3 3))
;; 		(b . ,(+ 3 2))) 'a (partial * 3))

(defun-export! alist-get (a k)
  (cdr (assoc k a)))

(defun-export! alist-update-in (alist path fn)
  (let ((key (car path))
	(path (cdr path)))
    (if (not path)
	(alist-update alist key fn)
	(alist-update-in (alist-get alist key) path fn))))

(defun-export! assoc-in (lst path val)
  (let ((ind (car path))
	(path (cdr path)))
    (if (not path)
	(assoc-to-ind lst ind val)
	(assoc-to-ind lst ind
		      (assoc-in (nth ind lst) path val)))))

(defun-export! get-in (lst path)
  (let ((ind (car path))
	(path (cdr path)))
    (if (not path)
	(nth ind lst)
	(get-in (nth ind lst) path))))

(defun-export! get-tile-at (map layer x y)
  (->> map
       Map-layers
       (nth layer)
       layer-tiles
       (nth x)
       (nth y)))


(defun filter (l s)
  (remove-if-not l s))

;; TODO testaa allekommentoidulla paskeella
(defun-export! find-layer-parent (layer root)
  (let* ((maps (mapcar #'cdr (root-maps root))))
    (remove-if-not (lambda (map)
		     (position layer (map-layers map)))
		   maps)))
	 

;; (let* ((root (-> (init-root!)
;; 		 (push-map (make-map-with-layers "Lolmap" 2 2 3))
;; 		 (push-map (make-map-with-layers "Lolmap" 2 3 3))))
;;        (map (cdar (root-maps root)))
;;        (layer (car (map-layers map))))
;;   (equalp (find-layer-parent layer root) map))
;; => T






(defun-export! push-sprite (map sprite)
  (let ((k (gensym)))
    (alist-update map 'spritesAndAnimatedsprites
		  (partial alist-cons k sprite))))
					   


(defun-export! index-of-chosen-map (root)
  (root-chosenMap root))

(defun-export! get-tile-at-chosen-map (root x y)
  (let ((m (root-chosenMap root))
	(l (root-chosenLayerInd root)))
    (get-tile-at m l x y)))
    

(defun-export! set-tile-inner (map layer x y tile)
  (set-Map-layers! map
		   (-> map
		       Map-layers
		       (assoc-in (list layer x y) tile))))

(defun-export! set-tile-at (root x y tile)
  (set-tile-inner (root-chosenMap root)
		  (root-chosenLayerInd root)
		  x
		  y
		  tile))

(defun-export! set-chosen-tile-at (root x y)
  (set-tile-at root x y (root-chosenTile root)))

(defun-export! set-tile-at-chosen-map (root x y tile)
  (assert nil)
  ;; (let* ((l (root-chosenLayerInd root))
  ;; 	 (m (set-tile-at root x y tile))	 
  ;; 	 (maps-index (find-index-of (root-maps root) m)))
  ;;   (-> root
  ;; 	(set-root-chosenMap! m)
  ;; 	(set-root-maps! (alist-cons maps-index m (root-maps root)))))
  )

(defun-export! clear-lisp-dq (dst)
  (funcall clear-lisp-drawingqueue (symbol-name dst)))

(defun-export! add-to-lisp-qd (dst fn)
  (funcall add-lambda-to-drawingqueue dst fn))

(defun-export! render-img (dst img)
  (funcall render dst img))

(defun fetch-tile-from-tileset (root tileset x y)
  (let ((tileset (->> root
		      (root-tilesets)
		      (mapcar #'cdr)
		      (nth tileset))))
    ;; (format t "(< ~a ~a)~%(< ~a ~a)~%"
    ;; 	    x (tileset-w tileset)
    ;; 	    y (tileset-h tileset))
    
    (assert (< x (tileset-w tileset)))
    (assert (< y (tileset-h tileset)))
    (let ((tile 
	   (->> tileset
		tileset-tiles
		;; v
		(nth x)
		;; v
		(nth y))))
      (if tile
	  tile
	  ;; (format t "didn't find a tile ~%")
	  ))))

(defun-export! select-map (root map)
  (let ((index (position map (mapcar #'cdr (root-maps root))))
	(name (map-name map)))
    (clear-lisp-dq :MAP)
    (add-to-lisp-qd :MAP (lambda ()
    			   (let* ((w (map-width map))
    				  (x-coords (mapcar #'dec (range w)))
    				  (h (map-width map))
    				  (y-coords (mapcar #'dec (range h)))
    				  (layers (length (map-layers map)))
    				  (l-coords (mapcar #'dec (range layers))))
			     ;(format t "rendering map~%")
    			     (mapcar (lambda (l)
    			     	       (mapcar (lambda (x)
    			     			 (mapcar (lambda (y)
    			     				   (let* ((tile (get-tile-at map l x y))
								  (tile (if (tile-gl-key tile)
									    tile
									    (fetch-tile-from-tileset root
													(tile-tileset tile)
													(tile-x tile)
													(tile-y tile))))
    			     					  (gl-key (tile-gl-key tile)))
							     (when gl-key
							       ; (format t "rendering gl-key ~a to ~a, ~a~%" gl-key (* 50 x) (* 50 y))
							       (set-image-x tile (* 50 x))
							       (set-image-y tile (* 50 y))
							       
    			     				       (render-img :MAP gl-key))
							     ;; (unless gl-key
							     ;;   (format t "gl-key is nil~%"))
							     ))
    			     				 y-coords))
    			     		       x-coords))
    			     	     l-coords)
			     ;; (format t "done rendering map ~%")
			     )))
    (set-root-chosenMap! root index)))

(defun-export! push-selected-map (root map)
  (let* (;; let's prepare the chosenmap
	 (root (select-map root map))
	 (index (root-chosenMap root))
	 ;; and let's cons the chosenmap to its old index
	 (new-map-consed (alist-cons (get-prop map "id") map
				
				     (root-maps root))))
    (set-root-maps! root new-map-consed)))

(defun-export! set-tile-rotation-at (root x y rotation)
  (format t "todo @set-tile-rotation-at~%")
  (assert nil)
  (let ((map (-> (root-chosenMap root)
		 (get-tile-at (root-chosenLayerInd root) x y)
		 (set-Tile-rotation! rotation))))
    ;; (push-selected-map root map)
    ))

(defun-export! add-layer (root map-index)
  (let ((maps (-> (root-maps root)
		  (alist-update map-index (lambda (m)
					    (let ((layers (Map-layers m))
						  (w (Map-width m))
						  (h (Map-height m)))
					      (set-Map-layers! m (cons (make-Layer (str (prin1-to-string (length layers)) "th layer")
										   255
										   t
										   (make-tiles w h)
										   nil) layers))))))))
    (set-root-maps! root maps)))

(defun-export! delete-layer (root map-index layer-index)
  (set-root-maps! root
		  (-> (root-maps root)
		      (alist-update map-index (lambda (m)
						(set-Map-layers! m (drop-list-i (Map-layers m) layer-index)))))))
