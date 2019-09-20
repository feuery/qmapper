(defpackage :qmapper.tileset
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.export
	:qmapper.root
	:qmapper.tile)
  (:import-from :fset :lookup :with :empty-map)
  (:shadowing-import-from :fset :convert)
  (:import-from :qmapper.export
		:explode :set-img-rotation
		:load-image :image-w :image-h :copy-image
		:add-to-drawingqueue  :clear-drawingqueue
		:set-img-x :set-img-y
		:do-schedule-lambda))		

(in-package :qmapper.tileset)


(defcppclass Tileset
    (public
     (fields
      (name "Tileset 1")
      ;; (vertexShader nullptr)
      ;; (fragmentShader nullptr)
      (tiles '())
      (w 0) 				; in tiles
      (h 0))))				; in tiles

(defun-export! get-tile (tileset x y)
  (get-prop-in (tileset-tiles tileset) (list x y)))

(defun-export! select-tileset-to-draw (*this*)
  (clear-draw-queue :TILESET)
  (let ((coords (pairs (mapcar #'dec (range (Tileset-w *this*)))
		       (mapcar #'dec (range (Tileset-h *this*)))))
	(tiles (Tileset-tiles *this*)))
    ;; (format t "coord-pairs are ~a~%w and h are ~a & ~a ~%" coords
    ;; 	    (Tileset-w *this*)
    ;; 	    (Tileset-h *this*))
    (dolist (c-pair coords)
      (let* ((x (car c-pair))
      	     (y (cadr c-pair))
      	     (img (get-prop-in tiles (list x y))))
	(if img
	    (progn
              (set-image-x :TILESET img (truncate (* x 50.0)))
              (set-image-y :TILESET img (truncate (* y 50.0)))
              (add-to-drawqueue img :TILESET))
	    (progn
	      (format t "x and y are [~a ~a]~%" x y)))))
    (format t "dolist is done~%")))

(defun-export! select-tileset (tileset)
  (format t "Selecting tileset~%")
  (select-tileset-to-draw tileset)
  (assert (not (consp tileset))))
  				 
(defun-export! load-tilesetless-texture-splitted (path &key (tile-width 50) (tile-height 50))
  (let* ((root-img (load-img path))
	 (_ (format t "img loaded~%"))
  	 (w (floor (/ (img-width root-img) tile-width)))
  	 (h (floor (/ (img-height root-img) tile-height)))
	 (_ (format t "dimensions ~a found~%" (list w h)))
	 (textures (mapcar (lambda (x)
  			     (mapcar (lambda (y)  
  				       (copy-img root-img
  						 (* x tile-width) (* y tile-height)
  						 tile-width tile-height))
  				     (mapcar #'dec (range h))))
  			   (mapcar #'dec (range w)))))
    (format t "textures ~a loaded!~%" textures)
    (values
     textures
     w
     h)))	      

(defun-export! load-texture-splitted (path)
  (let* ((root-img (load-img path))
	 (_ (format t "img loaded~%"))
  	 (w (/ (img-width root-img) 50))
  	 (h (/ (img-height root-img) 50))
	 (_ (format t "dimensions found~%"))
	 (textures (mapcar (lambda (x)
  			     (mapcar (lambda (y)
				       (make-tile :x x :y y :tileset (tileset-count!) :rotation 0
						  :gl-key
  						  (copy-img root-img
  							    (* x 50) (* y 50)
  							    50 50)))
  				     (mapcar #'dec (range h))))
  			   (mapcar #'dec (range w)))))
    (format t "textures loaded!")
    (values
     textures
     w
     h)))

(defun-export! push-tileset (root tileset)
  (let* ((tilesets (set-prop (root-tilesets root)
			     (get-prop tileset "ID")
			      tileset))
	 (final-root (set-root-tilesets! root tilesets)))
    final-root))

(defun-export! load-tileset (path)
  (format t "Going into load-tileset~%")
  (schedule-once :TILESET (lambda ()
			    (multiple-value-bind (tiles w h) (load-texture-splitted path)
			      (format t "Loaded tile textures, going to make tileset ~%")
			      (let ((tileset (make-tileset "New tileset" tiles w h)))
				(set-doc (push-tileset *document* tileset)))))))
				
    

;; void Tilesetcontainer::load_texture_splitted(Renderer *parent, const char *filename)
;; {
;;   QImage root_img;
;;   root_img.load(QString(filename));

;;   tiles_w = root_img.width() / 50;
;;   tiles_h = root_img.height() / 50;

;;   // qDebug() << "Tiles_w: " << tiles_w;

;;   // QVector<QVector<obj*>> results(array_w);
;;   // tiles = new std::string*[tiles_w];
;;   tiles.resize(tiles_w, std::vector<std::string>(tiles_h, ""));
  
;;   for(int x = 0; x < tiles_w; x++) {
;;     std::vector<Tile*> tileRow;
    
;;     for(int y = 0; y < tiles_h; y++) {
;;       QImage copy = root_img.copy(x * 50,
;; 				  y * 50, 50, 50);
;;       obj *o = obj::make(parent, copy);
;;       o->position = glm::vec2(x * 50.0f, y * 50.0f);
;;       // qDebug() << "Loading tiletexture at " << o;
;;       tiles[x][y] = o->getRenderId();

;;       Tile *t = new Tile;
;;       t->setX(x);
;;       t->setY(y);
;;       t->setTileset(getId());
;;       tileRow.push_back(t);
;;     }
;;     try {
;;     getTiles()->push_back(tileRow);
;;     }
;;     catch(...) {
;;       puts("(Lol");
;;       throw;
;;     }
;;   }
;; }


;; Tilesetcontainer::Tilesetcontainer():Tileset() {
;;   setTiles(new std::vector<std::vector<Tile*>>);
;; }

;;   load_texture_splitted(r, tilesetPath);
;;   r->owned_objects[getId()] = this;
;;   if(!editorController::instance->firstLoadedTileset) {
;;     for(int i = 0; i < editorController::instance->tiles->size(); i++) {
;;       Tile *t = editorController::instance->tiles->at(i);
;;       t->setTileset(getId());
;;     }
    
;;     editorController::instance->firstLoadedTileset = this;
;;     delete editorController::instance->tiles;
;;     editorController::instance->tiles = nullptr;
;;   }
;; }

;(export-all :qmapper.tileset)
