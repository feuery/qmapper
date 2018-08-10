;; (define-module (qmapper-root)
;;   #:use-module (srfi srfi-1)
;;   :use-module (qmapper-std))

(defpackage :qmapper.root
  (:use :common-lisp
	:cl-arrows
	:qmapper.std))

(in-package :qmapper.root)

(defcppclass root
    (public   
     (fields
      ;; defcppclass macro should ignore the type specifier
      ;; and these are supposed to be (gensym) -> 'a plists 
      ;; (std__map<std__string___animatedsprite*>* animatedSprites  '())
      ;; (std__map<std__string___Sprite*>* sprites  '())
      (std__map<std__string___Layer*>* layers  '())
      (std__map<std__string___Map*>* maps  '())
      (std__map<std__string___Script*>* scripts  '())
      (std__map<std__string___Tile*>* tiles  '())
      (std__map<std__string___Tileset*>* tilesets  '())
      (SCM chosenMap '())
      (int chosenLayerInd -1)
      (SCM chosenTileset '())
      (SCM chosenTile '())
      (SCM StdVertexShader '())
      (SCM StdFragmentShader '())
      (SCM StdTileviewFragShader '()))
     (functions
      (contents-StdFragment ()
			    (Script-contents (root-StdFragmentShader *this*)))
      (contents-StdVertex ()
			  (Script-contents (root-StdVertexShader *this*)))
      (contents-TileviewFragShader ()
				   (Script-contents (root-StdTileviewFragShader *this*)))
      
      (registrySize ()
		    (+ ;; (length (root-animatedSprites *this*))
		     (length (root-layers *this*))
		     (length (root-maps *this*))
		     (length (root-Scripts *this*))
		     ;; (length (root-sprites *this*))
		     (length (root-tiles *this*))
		     (length (root-tilesets *this*))))
      (fetchRegister (Type id)
		     (plist-get 
		      (condp equalp Type
			     ;; TODO write a thing that catenates sprites from every map
			     ;; "AnimatedSprite" (root-animatedSprites *this*)
			     "Layer" (root-layers *this*)
			     "Map" (root-maps *this*)
			     "Script" (root-Scripts *this*)
			     ;; "Sprite" (root-sprites *this*)
			     "Tile" (root-tiles *this*)
			     "Tilesets" (root-tilesets *this*))
		      id))
      (findNs (ns)
	      (let ((scripts (root-scripts *this*)))
		(first (filter (lambda (script)
				 (equalp (Script-ns script) ns)) scripts))))
      (saveNs (ns content)
	      (let* ((scripts (root-scripts *this*))
		     (index (get-list-index (root-scripts *this*) (lambda (sc)
								    (equalp (Script-ns sc) ns))))
		     (new-script (-> (nth index scripts)
				     (set-Script-Contents! content)))
		     (scripts (assoc-to-ind scripts index new-script)))
		(set-root-scripts *this* scripts)))
      (containsNs (ns)
		  (not (filter (lambda (script)
				 (equalp (script-ns script) ns)) scripts)))
      (registryOf (type)
		  (let ((result 
			 (cond ;; ((equalp type "AnimatedSprite")
			   ;;  (root-animatedSprites *this*))
			   ((equalp type "Layer")
			    (root-layers *this*))
			   ((equalp type "Map") (root-maps *this*))
			   ((equalp type "Script") (root-scripts *this*))
			   ;; ((equalp type "Sprite") (root-sprites *this*))
			   ((equalp type "Tile") (root-tiles *this*))
			   ((equalp type "Tileset") (root-tilesets *this*))
			   (t
			    (scm-puts "registryOf got unrecognized type")
			    (scm-puts type)
			    nil))))
		    (if result
			(drop-plist-keys result)
			result)))
      
      (registryToList ()
		      (drop-plist-keys (concatenate 'list
						    ;; (root-animatedSprites *this*)
						    (root-layers *this*)
						    (root-maps *this*)
						    (root-scripts *this*)
						    ;; (root-sprites *this*)
						    (root-tiles *this*)
						    (root-tilesets *this*)))))))

      
(defun push-map (root m)
  (let* ((maps (alist-cons (gensym)
			  m
			  (root-maps root)))
	 (r (if (not (root-chosenMap root))
		(set-root-chosenMap! root (first maps))
		root)))    
    (set-root-maps! root maps)))

(defun push-script (root scr)
  (set-root-scripts! root (alist-cons (gensym)
				      scr
				      (root-scripts root))))

(defun find-ns (root ns)
  (let ((scr (filter
	      (lambda (scr)
		(equalp (Script-ns scr) ns))
	      (root-scripts root))))
    (if (not scr)
	'()
	(car scr))))
    
(defun is-ns-scheme? (root ns)
  (let ((scr (find-ns root ns)))
    (Script-is-scheme? (car scr))))

(defun is-ns-glsl? (root ns)
  (let ((scr (find-ns root ns)))
    (Script-is-glsl? (car scr))))

(defun push-sprite-to-chosen-map (root sprite)
  (let ((chosen-map (-> (root-chosenMap root)
			(push-sprite sprite))))
    (push-selected-map root chosen-map)))

(export-all :qmapper.root)

(format t "qmapper.root loaded!")

;; (scm-puts "root loaded")
