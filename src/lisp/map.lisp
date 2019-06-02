(defpackage :qmapper.map
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
        :qmapper.tileset
	:qmapper.animatedsprite
	:qmapper.script
	:qmapper.sprite
	:qmapper.layer
	:qmapper.root
	:qmapper.export
	:qmapper.tile)
  (:import-from :qmapper.export :clear-lisp-drawingqueue :add-lambda-to-drawingqueue)
  (:import-from :fset :size :convert))

(in-package :qmapper.map)

(defun-export! find-nearest  (x y lst)
  (->> lst
       (mapcar (lambda (sprite)
		 (list (distance (sprite-x sprite)
				 (sprite-y sprite)
				 x y)
		       sprite)))
       (sort-by #'first)
       (first)
       (last)))

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
		   (let* ((lst  (->> (map-sprites *this*)
				     (convert 'list)
				     (concatenate 'list 
						  (convert 'list (map-animatedSprites *this*)))
				     (mapcar (lambda (sprite-id)
					       (or 
						(get-prop (root-sprites *document*) sprite-id)
						(get-prop (root-animatedSprites *document*) sprite-id)))))))
		     (car (find-nearest x y lst))))
      (width ()
	     (let* ((layer-id (fset-first (Map-layers *this*)))
		    (layers (get-prop (root-layers *document*) layer-id)))
	       (Layer-width layers)))
      (height ()
	      (let* ((layer-id (fset-first (Map-layers *this*)))
		     (layer (get-prop (root-layers *document*) layer-id)))
		(Layer-height layer)))
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
	(set-root-maps! (set-prop (root-maps doc) (get-prop map "ID") map))
	(set-root-chosenmap! (get-prop map "ID")))))

(defun-export! get-tile-at2 (layer x y)
  (get-prop-in (root-layers *document*) (list layer 'tiles x y)))

(defun-export! get-tile-at (map layer x y)
  (let ((l (get-prop (root-layers *document*) (get-prop (map-layers map) layer))))
    (get-prop-in l (list "tiles" x y))))


;; (mapcar (lambda (a) (format t "lol ~a~%")) (fset:seq 1 2 3 4 5))

;; The value
;;   #[ 1 2 3 4 5 ]
;; is not of type
;;   LIST

(defun-export! find-layer-parent (layer root)
  (let* ((maps (mapcar #'cdr (convert 'list (root-maps root)))))
    (car (remove-if-not (lambda (map)
			  (let ((layers (convert 'list (map-layers map))))
			    (position layer layers :test #'string=)))
			maps))))
	 
(defun-export! index-of-chosen-map (root)
  (root-chosenMap root))

(defun-export! get-tile-at-chosen-map (root x y)
  (let ((m (root-chosenMap root))
	(l (root-chosenLayerInd root)))
    (get-tile-at m l x y)))

(defun set-tile-innest (tiles x y tile)
  (set-prop-in tiles (list x y) tile))
    
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

    (when tileset
      
      (assert (< x (tileset-w tileset)))
      (assert (< y (tileset-h tileset)))
      (get-prop-in tileset (list "tiles" x y)))))

(defun-export! set-tile-rotation-at (root x y rotation)
  (let* ((layer (get-prop (root-layers root) (root-chosenLayerInd root)))
	 (tile (-> layer
		   (get-prop-in (list 'tiles x y))
		   (set-prop 'rotation rotation)))
	 (layer (set-prop-in layer (list 'tiles x y) tile)))
    (set-root-layers! root
		      (set-prop (root-layers root) (root-chosenLayerInd root) layer))))

(defun-export! add-layer (root map-index)
  (let ((l nil))
    (-> root
	(update-prop-in (list "MAPS" map-index) (lambda (m)
						 (let* ((layers (Map-layers m))
							(w (Map-width m))
							(h (Map-height m))
							(ll (make-Layer (str (prin1-to-string (size layers)) "th layer")
												      255
												      t
												      (make-tiles w h))))
						   ;; ei
						   (setf l ll)
						   (update-prop m "LAYERS" (lambda (layers)
									     (fset:insert layers 0 (get-prop l "ID")))))))
	(update-prop "LAYERS" (lambda (layers)
				(set-prop layers (get-prop l "ID") l))))))

(defun-export! add-layer-to-selected-map (root)
  (add-layer root (root-chosenMap root)))

(defun-export! delete-layer (root map-index layer-index)
  (set-root-maps! root
		  (-> (root-maps root)
		      (alist-update map-index (lambda (m)
						(set-Map-layers! m (drop-list-i (Map-layers m) layer-index)))))))

;; I hope putting those lambdas here too will keep them from being collected
(defvar *lisp-dq-buffer* (list))

(defun-export! clear-lisp-dq (dst)
  (setf *lisp-dq-buffer* (list))
  (funcall clear-lisp-drawingqueue (symbol-name dst)))

(defun-export! add-to-lisp-qd (dst fn)
  ;; doesn't work on sbcl
  ;; (ext:set-finalizer fn (lambda (x)
  ;; 			  (format t "Finalizing lisp-dq-element ~a~%" x)))
  (setf *lisp-dq-buffer* (cons fn *lisp-dq-buffer*))
  (funcall add-lambda-to-drawingqueue dst fn))

(defun deg->rad (deg)
  (/ (* pi deg) 180))

(defun get-layer (doc map index)
  (let ((layer-list (map-layers map)))
    (get-prop (root-layers doc) (get-prop layer-list index))))

;; epicly copypasted from https://groups.google.com/d/msg/comp.lang.lisp/QWk0uDw2Fd0/UZ1nKv9hd0QJ
(defun mapcar-with-index (function list &rest more-lists)
  (let ((index -1))
    (apply #'mapcar (lambda (&rest args)
                      (incf index)
                      (apply function (append args (list index))))
           (append (list list) more-lists))))

(defun set-image-subobject (dst tile subtile)
  (funcall set-img-subobj dst (tile-gl-key tile) (tile-gl-key subtile)))

(defun-export! set-map-renderer-fn (dst map-id)
  (add-to-lisp-qd dst (lambda ()
			(setf map-id (root-chosenmap *document*))
			;; (setf root-maps (root-maps *document*))
		        (let ((*local-document* (if (equalp dst :ENGINE)
						    *engine-document*
						    qmapper.root:*document*)))
			  (when *local-document*
			    (let* (
				   (root *local-document*)
				   (map (get-prop (root-maps root) (root-chosenmap root)))
				   (_ (assert map))

				   (w (map-width map))
				   
    				   (x-coords (mapcar #'dec (range w)))
    				   (h (map-width map))
    				   (y-coords (mapcar #'dec (range h)))
				   (layer-list (map-layers map))
    				   (layers (size layer-list))
    				   (l-coords (reverse (mapcar #'dec (range layers))))
				   (sprites (map-sprites map))
				   (root-sprites (root-sprites *local-document*))
				   (animations (map-animatedsprites map))
				   (root-animations (root-animatedsprites *local-document*))
				   (final-l-coords (->> l-coords
							(remove-if-not (lambda (l-index)
									 (let ((layer (get-layer root map l-index)))
									   (layer-visible layer)))))))
			      (->> final-l-coords
    				   (mapcar-with-index (lambda (l index)
							(let ((layer (get-prop (root-layers root) (get-prop layer-list l))))
    			     				  (mapcar (lambda (x)
    			     					    (mapcar (lambda (y)
									      (let* ((tile (get-tile-at map l x y)))
										(if tile 
			   							    (let ((tile (if (valid-gl-key? (tile-gl-key tile))
			   									    tile
			   									    (fetch-tile-from-tileset root
			   												     (tile-tileset tile)
			   												     (tile-x tile)
			   												     (tile-y tile))))
											  (subtile (if (not (zerop index))
												       (let ((subtile (get-tile-at map (nth (dec index) final-l-coords) x y)))
													 (if (valid-gl-key? (tile-gl-key subtile))
													     subtile
													     (fetch-tile-from-tileset root
			   													      (tile-tileset subtile)
			   													      (tile-x subtile)
			   													      (tile-y subtile)))))))
										      
										      (when tile
											(let ((rotation (tile-rotation tile))
    			     								      (gl-key (tile-gl-key tile)))
			   								  (if (valid-gl-key? gl-key)
											      (progn
			   									(set-image-x :MAP tile (* 50 x))
			   									(set-image-y :MAP tile (* 50 y))
												(set-image-opacity :MAP tile (get-prop layer "opacity"))
			   									(set-image-rotation :MAP tile (deg->rad rotation))


												(when subtile
												  (set-image-subobject :MAP tile subtile))
												
    			     									(render-img :MAP gl-key)))))))))
    			     						    y-coords))
    			     					  x-coords)))))

			      (dolist (sprite-id (convert 'list sprites))
				(let ((sprite (get-prop root-sprites sprite-id)))
				  (sprite-render sprite)))

			      (dolist (animation-id (convert 'list animations))
				(let ((anim (-> (get-prop root-animations animation-id)
						animatedsprite-advanceframeifneeded!)))
				  ;; a surprising skip of the set-doc, to prevent DoSsing dom tree element
				  (if (equalp dst :ENGINE)
				      (setf *engine-document* (set-prop-in *local-document* (list 'animatedSprites (get-prop anim "ID")) anim))
				      (setf qmapper.root:*document* (set-prop-in *local-document* (list 'animatedSprites (get-prop anim "ID")) anim)))
				  (animatedsprite-render anim)))))))))

(defun-export! select-map-layer (root map-id layer-id)
  (clear-lisp-dq :MAP)
  (set-map-renderer-fn :MAP map-id)
  
  (-> root
      (set-root-chosenlayerind! layer-id)
      (set-root-chosenmap! map-id)))

(defun-export! resize-selected-map (root new-w new-h)
  (let* ((selected-map-id (root-chosenmap root))
	 (selected-map (get-prop-in root (list "MAPS" selected-map-id))) 
	 (layer-ids (map-layers selected-map)))
    (fset:reduce (lambda (root layer-id)
		   (update-prop-in root (list "LAYERS" layer-id) (lambda (layer)
								   (resize-layer layer new-w new-h))))
		 layer-ids :initial-value root)))
