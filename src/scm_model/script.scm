(defcppclass Script
  (public
   (properties
    (std__string contents "")
    (std__string name "")
    ;; TODO redo validators in pure scheme
    (std__string ns "user" ;; (doesntContainNs)
		 )
    (scriptTypes script_type :scheme))))

(scm-puts "Loaded script")
