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
  (:import-from :qmapper.export :clear-lisp-drawingqueue :add-lambda-to-drawingqueue)
  (:import-from :fset :convert))

(in-package :qmapper.map)

(defcppclass Map
    (public 
     (properties
      (name "Map 1")
      (layers '())
      (sprites '())
      (animatedSprites '())
      ;; (parent nil)
      )
     (functions
      (findNearest (x y)
		   ;; Let's search the nearest animatedsprite or sprite
		   (find-nearest x y (Map-spritesAndAnimatedsprites *this*)))
      (width ()
	     (let* ((layer-id (first (Map-layers *this*)))
		    (layers (get-prop (root-layers *document*) layer-id)))
	       (Layer-width layers)))
      (height ()
	      (let* ((layer-id (first (Map-layers *this*)))
		    (layers (get-prop (root-layers *document*) layer-id)))
		(Layer-height layers)))
      ;; this'll be fun
      (resize (w
	       h
	       vAnchor
	       hAnchor)
	      ;; TODO IMPLEMENT
	      (error "Don't call Map-resize yet!")))))

(defun-export! make-map-with-layers (doc name w h layer-count)
  (let* ((layers (repeatedly (lambda (i)
			       (let ((l 
				      (make-Layer (str (prin1-to-string i) "th layer")
						  255
						  t
						  (make-tiles w h))))
				 ;(format t "making layer ~a ~%" l)
				 l)) layer-count))
	 (ids (mapcar (lambda (l) (get-prop l "ID")) layers))
	 (map (make-map name ids '() '())))
    (-> (set-root-layers! doc
			  (reduce (lambda (all-layers layer)
				    (set-prop all-layers (get-prop layer "ID") layer)) layers :initial-value (root-layers doc)))
	(set-root-maps! (set-prop (root-maps doc) (get-prop map "ID") map)))))

;; (defun-export! alist-update (alist k fn)
;;   (alist-cons k (funcall fn (cdr (assoc k alist))) alist))

;; (alist-update `((a . ,(* 3 3))
;; 		(b . ,(+ 3 2))) 'a (partial * 3))

;; (defun-export! alist-get (a k)		
;;   (cdr (assoc k a)))

;; (defun-export! alist-update-in (alist path fn)
;;   (let ((key (car path))
;; 	(path (cdr path)))
;;     (if (not path)
;; 	(alist-update alist key fn)
;; 	(alist-update-in (alist-get alist key) path fn))))

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

(defun-export! get-tile-at2 (layer x y)
  (get-prop-in (root-layers *document*) (list layer 'tiles x y)))

(defun-export! get-tile-at (map layer x y)
  (let ((l (get-prop (root-layers *document*) (nth layer (map-layers map)))))
    (->> l
  	 layer-tiles
  	 (nth x)
  	 (nth y))))


(defun filter (l s)
  (remove-if-not l s))

(defun-export! find-layer-parent (layer root)
  (let* ((maps (mapcar #'cdr (convert 'list (root-maps root)))))
    (car (remove-if-not (lambda (map)
			  (format t "Löytyykö ~a (~a) layereistä ~a (listof<~a>)?~%" layer (type-of layer)
				  (map-layers map)
				  (type-of (car (map-layers map))))
			  (format t ": ~a~%" (position layer (map-layers map) :test #'string=))
			  (position layer (map-layers map) :test #'string=))
			maps))))
	 

;; (let* ((root (-> (init-root!)
;; 		 (make-map-with-layers "Lolmap" 2 3 3)
;; 		 (make-map-with-layers "Lolmap" 2 2 3)))
;;        (map (cdar (convert 'list (root-maps root))))
;;        (layer (car (convert 'list (map-layers map)))))
;; ;;   (find-layer-parent layer root)  )
;;   (equalp (find-layer-parent layer root) map))

;; => T






(defun-export! push-sprite (map sprite)
  (let ((k (get-prop sprite "ID")))
    (alist-update map 'spritesAndAnimatedsprites
		  (partial alist-cons k sprite))))
					   


(defun-export! index-of-chosen-map (root)
  (root-chosenMap root))

(defun-export! get-tile-at-chosen-map (root x y)
  (let ((m (root-chosenMap root))
	(l (root-chosenLayerInd root)))
    (get-tile-at m l x y)))

(defun set-tile-innest (tiles x y tile)
  (assoc-in tiles (list x y) tile))
    
(defun-export! set-tile-inner (root map layer x y tile)
  (let* ((layer-id layer)
	 (layers (map-layers map))
	 (layer (get-prop (root-layers root) layer-id))
	 (layer (set-layer-tiles! layer (set-tile-innest (layer-tiles layer) x y tile))))
    (set-root-layers! root (set-prop (root-layers root) layer-id layer))))

(defun-export! set-tile-at (root x y tile)
  (let* ((ind (root-chosenMap root))
	 (maps (root-maps root))
	 (map (get-prop maps ind)))
    (set-tile-inner root
		    map
		    (root-chosenLayerInd root)
		    x
		    y
		    tile)))

(defun-export! set-chosen-tile-at (root x y)
  (assert (root-chosenTile root))
  (format t "chosenTile is ~a~%" (root-chosenTile root))
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

(defun fetch-tile-from-tileset (root tileset x y)
  (let ((tileset (->> root
		      (root-tilesets)
		      (convert 'list)
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

(defun-export! set-tile-rotation-at (root x y rotation)
  (let* ((layer (get-prop (root-layers root) (root-chosenLayerInd root)))
	 (tile (-> layer
		   (get-prop-in (list 'tiles x y))
		   (set-prop 'rotation rotation)))
	 (layer (set-prop-in layer (list 'tiles x y) tile)))
    (set-root-layers! root
		      (set-prop (root-layers root) (root-chosenLayerInd root) layer))))

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



(defun-export! clear-lisp-dq (dst)
  (funcall clear-lisp-drawingqueue (symbol-name dst)))

(defun-export! add-to-lisp-qd (dst fn)
  (funcall add-lambda-to-drawingqueue dst fn))

(defun-export! render-img (dst img)
  (funcall render dst img))

(defun deg->rad (deg)
  (/ (* pi deg) 180))

(defun-export! select-map-layer (root map-id layer-id)
  (clear-lisp-dq :MAP)
  (add-to-lisp-qd :MAP (lambda ()

			 (setf map-id (root-chosenmap *document*))
			 (setf root-maps (root-maps *document*))

    			 (let* ((root *document*)
			 	(map (get-prop (root-maps root) (root-chosenmap root)))
			 	(_ (assert map))

			 	(w (map-width map))
				
    			 	(x-coords (mapcar #'dec (range w)))
    			 	(h (map-width map))
    			 	(y-coords (mapcar #'dec (range h)))
    			 	(layers (length (map-layers map)))
    			 	(l-coords (reverse (mapcar #'dec (range layers)))))
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
								(rotation (tile-rotation tile))
    			     					(gl-key (tile-gl-key tile)))
			 				   (when gl-key
			 				     (set-image-x :MAP tile (* 50 x))
			 				     (set-image-y :MAP tile (* 50 y))
							     (set-image-rotation :MAP tile (deg->rad rotation))
							     
    			     				     (render-img :MAP gl-key))
			 				   ;; (unless gl-key
			 				   ;;   (format t "gl-key is nil~%"))
			 				   ))
    			     			       y-coords))
    			     		     x-coords))
    			     	   l-coords)
			   ;; (format t "done rendering map ~%")
			   )
			 ))
  
  (-> root
      (set-root-chosenlayerind! layer-id)
      (set-root-chosenmap! map-id)))
