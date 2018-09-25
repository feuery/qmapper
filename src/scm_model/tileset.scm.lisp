(defpackage :qmapper.tileset
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.tileset)

(defcppclass Tileset
    (public
     (fields
      (name "")
      ;; (vertexShader nullptr)
      ;; (fragmentShader nullptr)
      (tiles '()))))
	       


;; (defun-export! load-img (path)
;;   (format t "Loading from ~a~%" path)
;;   (format t "TODO replace with a call to qt~%"))

;; (defun-export! img-width (img)
;;   (format t "TODO delegate to qt~%")
;;   100)

;; (defun-export! img-height (img)
;;   (format t "TODO delegate to qt~%")
;;   100)

;; (defun-export! copy-img (img x y w h)
;;   (format t "TODO delegate to qt~%")
;;   img)

(defun-export! load-texture-splitted (path)
  nil
  ;; (let* ((root-img (load-img path))
  ;; 	 (w (img-width root-img))
  ;; 	 (h (img-height root-img)))
  ;;   (mapcar (lambda (x)
  ;; 	      (mapcar (lambda (y)
  ;; 			(copy-img root-img
  ;; 				  x y
  ;; 				  (* x 50)
  ;; 				  (* y 50)))
  ;; 		      (range h)))
  ;; 	    (range w)))
  )

(defun-export! load-tileset (path)
  (let ((tiles (load-texture-splitted path)))
    (make-tileset "New tileset" tiles)))
    

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
