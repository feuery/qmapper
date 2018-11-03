(defpackage :qmapper.tileset
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root)
  (:import-from :qmapper.export
		:load-image :image-w :image-h :copy-image
		:add-to-drawingqueue  :clear-drawingqueue
		:set-img-x :set-img-y
		:do-schedule-lambda))		

(in-package :qmapper.tileset)

(defun-export! set-image-x (img x)
  (funcall set-img-x img x))

(defun-export! set-image-y (img y)
  (funcall set-img-y img y))

(defun-export! load-img (path)
  (funcall load-image path))

(defun-export! img-width (img)
  (funcall image-w img))

(defun-export! img-height (img)
  (funcall image-h img))

(defun-export! copy-img (img x y w h)
  (funcall copy-image img x y w h))

(defun-export! add-to-drawqueue (img dst_key)
  (funcall add-to-drawignqueue img (symbol-name dst_key)))

(defun clear-draw-queue (dst_key)
  (funcall clear-drawingqueue (symbol-name dst-key)))

(defun schedule-once (dst l)
  (format t "scheduling once to dst ~a~%" (symbol-name dst))
  (funcall do-schedule-lambda (symbol-name dst) l))

(defcppclass Tileset
    (public
     (fields
      (name "Tileset 1")
      ;; (vertexShader nullptr)
      ;; (fragmentShader nullptr)
      (tiles '())
      (w 0) 				; in tiles
      (h 0))))				; in tiles

(defun-export! select-tileset (*this*)
  (clear-draw-queue :TILESET)
  (let ((coords (pairs (range (Tileset-w *this*))
		       (range (Tileset-h *this*))))
	(tiles (Tileset-tiles *this*)))
    (dolist (c-pair coords)
      (let* ((x (car c-pair))
      	 (y (cadr c-pair))
      	 (img (->> tiles
      		  (nth x)
      		  (nth y))))
        (set-image-x img (* x 50.0))
        (set-image-y img (* y 50.0))
        (add-to-drawqueue img :TILESET)))))
				 
	      

(defun-export! load-texture-splitted (path)
  (let* ((root-img (load-img path))
	 (_ (format t "img loaded~%"))
  	 (w (/ (img-width root-img) 50))
  	 (h (/ (img-height root-img) 50))
	 (_ (format t "dimensions found~%"))
	 (textures (mapcar (lambda (x)
  			     (mapcar (lambda (y)
  				       (copy-img root-img
  						 x y
  						 (* x 50)
  						 (* y 50)))
  				     (range h)))
  			   (range w))))
    (format t "textures loaded!")
    (values
     textures
     w
     h)))

(defun-export! push-tileset (root tileset)
  (let* ((tilesets (cons (cons (gensym)
			      tileset)
			 (root-tilesets root)))
	 (_ (format t "Pushing tileset ~a~%" tilesets))
	 ;; (r (if (not (root-chosenMap root))
	 ;; 	(set-root-chosenMap! root (first maps))
	 ;; 	root))
	 (final-root (set-root-tilesets! root tilesets)))
    final-root))

(defun-export! load-tileset (path)
  (format t "Going into load-tileset~%")
  (schedule-once :TILESET (lambda ()
			    (format t "Prööt prööt ~%")
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
