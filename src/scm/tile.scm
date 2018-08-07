;; (define-module (qmapper-tile)
;;   #:use-module (srfi srfi-1)
;;   :use-module (qmapper-std))

(defcppclass Tile 
  (public
   (properties
    (int x 0)
    (int y 0)
    (std__string tileset) 
    (int rotation 0))))

;; (scm-puts "Loaded tile")
