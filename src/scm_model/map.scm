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
	   (layer-width (first (Map-layers (this)))))
    (height ()
	    (layer-height (first (Map-layers (this)))))
    ;; this'll be fun
    (resize (w
	     h
	     vAnchor
	     hAnchor)
	    ;; TODO IMPLEMENT
	    (error "Don't call Map-resize yet!")))))


(scm-puts "map ladattu")
