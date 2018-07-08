(defcppclass root
  (public   
   (properties
    ;; defcppclass macro should ignore the type specifier
    ;; and these are supposed to be (gensym) -> 'a plists 
    ;; (std__map<std__string___animatedsprite*>* animatedSprites  '())
    ;; (std__map<std__string___Sprite*>* sprites  '())
    (std__map<std__string___Layer*>* layers  '())
    (std__map<std__string___Map*>* maps  '())
    (std__map<std__string___Script*>* scripts  '())
    (std__map<std__string___Tile*>* tiles  '())
    (std__map<std__string___Tileset*>* tilesets  '()))
   (functions
    (registrySize ()
	 (+ ;; (length (root-animatedSprites (this)))
	    (length (root-layers (this)))
	    (length (root-maps (this)))
	    (length (root-scripts (this)))
	    ;; (length (root-sprites (this)))
	    (length (root-tiles (this)))
	    (length (root-tilesets (this)))))
    (fetchRegister (Type id)
		   (plist-get 
		    (condp equal? Type
			   ;; TODO write a thing that catenates sprites from every map
			   ;; "AnimatedSprite" (root-animatedSprites (this))
			   "Layer" (root-layers (this))
			   "Map" (root-maps (this))
			   "Script" (root-scripts (this))
			   ;; "Sprite" (root-sprites (this))
			   "Tile" (root-tiles (this))
			   "Tilesets" (root-tilesets (this)))
		    id))		     
		   
    (findNs (ns)
	    (let ((scripts (root-scripts (this))))
	      (first (filter (lambda (script)
			       (equal? (script-ns script))) scripts))))
    (saveNs (ns content)
	    (let* ((scripts (root-scripts (this)))
		   (index (get-list-index (root-scripts (this)) (lambda (sc)
								  (equal? (script-ns sc) ns))))
		   (new-script (-> (list-ref scripts index)
				   (set-script-Contents! content)))
		   (scripts (assoc-to-ind scripts index new-script)))
	      (set-root-scripts (this) scripts)))
    (containsNs (ns)
		(not 
		 (null?
		  (filter (lambda (script)
			    (equal? (script-ns script))) scripts))))
    (registryOf (type)
		(let ((result 
		       (cond ;; ((equal? type "AnimatedSprite")
			     ;;  (root-animatedSprites (this)))
			     ((equal? type "Layer")
			      (root-layers (this)))
			     ((equal? type "Map") (root-maps (this)))
			     ((equal? type "Script") (root-scripts (this)))
			     ;; ((equal? type "Sprite") (root-sprites (this)))
			     ((equal? type "Tile") (root-tiles (this)))
			     ((equal? type "Tileset") (root-tilesets (this)))
			     (#t
			      (scm-puts "registryOf got unrecognized type")
			      (scm-puts type)
			      #f))))
		  (if result
		      (drop-plist-keys result)
		      result)))
		      
    (registryToList ()
		    (drop-plist-keys (concatenate (list
					 ;; (root-animatedSprites (this))
					 (root-layers (this))
					 (root-maps (this))
					 (root-scripts (this))
					 ;; (root-sprites (this))
					 (root-tiles (this))
					 (root-tilesets (this)))))))))
(scm-puts "root loaded")
