(defcppclass Sprite
  (public
   (properties
    (int x 0)
    (int y 0)
    (float angle 0.0)
    (std__string parentMapId "")
    (std__string name "")
    (obj* obj_ptr nil)
    (bool loadingDone #f))
   (functions
    (render ()
	    (render-object! (Sprite-obj_ptr (this)))))))

(scm-puts "Loaded sprite")
