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

(scm-puts "Loaded layer")
  
