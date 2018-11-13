(defpackage :qmapper.map
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.tileset
	:qmapper.script
	:qmapper.layer
	:qmapper.root
	:qmapper.export
	:qmapper.tile))

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
	(list-ref lst ind)
	(get-in (list-ref lst ind) path))))

(defun-export! get-tile-at (map layer x y)
  (-> map
      Map-layers
      (get-in (list layer x y))))

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

(defun-export! select-map (root map)
  (let ((old-index (position map (mapcar #'cdr (root-maps root)))))
    (set-root-chosenMap! root old-index)))

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
