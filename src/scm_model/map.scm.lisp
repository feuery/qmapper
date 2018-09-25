(defpackage :qmapper.map
  (:use :common-lisp
	:cl-arrows
	:qmapper.treecache
	:qmapper.std
	:qmapper.tileset
	:qmapper.script
	:qmapper.layer
	:qmapper.root
	:qmapper.tile))

(in-package :qmapper.map)

;; (macroexpand-1 '
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
	     (Layer-width (first (Map-layers *this*))))
      (height ()
	      (Layer-height (first (Map-layers *this*))))
      (lol ()
	   "Lollo")
      ;; this'll be fun
      (resize (w
	       h
	       vAnchor
	       hAnchor)
	      ;; TODO IMPLEMENT
	      (error "Don't call Map-resize yet!"))))) ;)

;; (format t "Ladataan qmapper.mapia ~%")

(defun-export! make-map-with-layers (name w h layer-count)
  (make-map name (repeatedly (lambda (i)
			       (make-Layer (str (prin1-to-string i) "th layer")
					   255
					   t
					   (make-tiles w h))) layer-count)
	    '()))

;; (cadr (assoc 'layers (make-map-with-layers "NAME" 3 3 2)))

; (make-map-with-layers "lol" 2 2 2)

;; (map-layers (make-map-with-layers "Lollo" 2 2 2))
;; (((NAME . "2th layer") (OPACITY . 255) (VISIBLE . T)
;;   (TILES
;;    (((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))
;;     ((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE")))
;;    (((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))
;;     ((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))))
;;   (TYPE-NAME . "LAYER"))
;;  ((NAME . "1th layer") (OPACITY . 255) (VISIBLE . T)
;;   (TILES
;;    (((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))
;;     ((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE")))
;;    (((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))
;;     ((X . 0) (Y . 0) (TILESET) (ROTATION . 0) (TYPE-NAME . "TILE"))))
;;   (TYPE-NAME . "LAYER")))

; (get-prop (make-map-with-layers "Lollo" 2 2 2) "namee")

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

(defun-export! find-index-of (alist el)
  (let ((result 
	 (filter
	  (lambda (m)
	    (equal? (cdr m)
		    el))
	  alist)))
    (if (not result)
	nil
	(caar result))))

;; TODO testaa allekommentoidulla paskeella
(defun-export! find-layer-parent (layer root)
  (let* ((maps (mapcar #'cdr (root-maps root))))
    (car
     (remove-if-not (lambda (map)
		      (position layer (map-layers map)))
		    maps))))
	 

;; (let* ((root (-> (init-root!)
;; 		 (push-map (make-map-with-layers "Lolmap" 2 2 3))
;; 		 (push-map (make-map-with-layers "Lolmap" 2 3 3))))
;;        (map (cdar (root-maps root)))
;;        (layer (car (map-layers map))))
;;   (equalp (find-layer-parent layer root) map)) ;; => T






(defun-export! push-sprite (map sprite)
  (let ((k (gensym)))
    (alist-update map 'spritesAndAnimatedsprites
		  (partial alist-cons k sprite))))
					   


(defun-export! index-of-chosen-map (root)
  (find-index-of (root-maps root)
		 (root-chosenMap root)))

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
  (let* ((l (root-chosenLayerInd root))
	 (m (set-tile-at root x y tile))	 
	 (maps-index (find-index-of (root-maps root) m)))
    (-> root
	(set-root-chosenMap! m)
	(set-root-maps! (alist-cons maps-index m (root-maps root))))))

(defun-export! push-selected-map (root map)
  (let* ((index (index-of-chosen-map root))
	 ;; let's prepare the chosenmap
	 (root (set-root-chosenMap! root map)))
    (set-root-maps! root
		    ;; and let's cons the chosenmap to its old index
		    (alist-cons index map
				
				root))))

(defun-export! set-tile-rotation-at (root x y rotation)
  (let ((map (-> (root-chosenMap root)
		 (get-tile-at (root-chosenLayerInd root) x y)
		 (set-Tile-rotation! rotation))))
    (push-selected-map root map)))

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

; (export-all :qmapper.map)
	 

;; (scm-puts "map ladattu")
