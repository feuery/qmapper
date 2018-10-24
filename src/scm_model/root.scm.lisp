;; (define-module (qmapper-root)
;;   #:use-module (srfi srfi-1)
;;   :use-module (qmapper-std))

(defpackage :qmapper.root
  (:use :common-lisp
	:cl-arrows
	:rutils.abbr
	:qmapper.std
	:qmapper.export
	:qmapper.script)
  (:shadowing-import-from :fset :empty-map :with :seq :image :lookup :filter :reduce :size :concat :convert))

(in-package :qmapper.root)

(defcppclass root
    (public   
     (fields
      ;; defcppclass macro should ignore the type specifier
      ;; and these are supposed to be (gensym) -> 'a plists 
      ;; (std__map<std__string___animatedsprite*>* animatedSprites  '())
      ;; (std__map<std__string___Sprite*>* sprites  '())
      (layers  '())
      (maps  '())
      (scripts  '())
      (tiles  '())
      (tilesets  '())
      (chosenMap '())
      (chosenLayerInd -1)
      (chosenTileset '())
      (chosenTile '())
      (StdVertexShader "defaultVertex")
      (StdFragmentShader "defaultFragment")
      (StdTileviewFragShader "default.tileView"))
     (functions
      (contents-StdFragment ()
			    (let* ((ns (root-stdfragmentshader *this*))
				   (result (->> (root-scripts *this*)
						(filter (lambda (b script)
		    					  (equalp (script-ns script) ns)))
						(convert 'list)
						cdar)))
			      (script-contents result)))
      (contents-StdVertex ()
			  (let* ((ns (root-stdvertexshader *this*))
				 (result (->> (root-scripts *this*)
					      (filter (lambda (b script)
		    					(equalp (script-ns script) ns)))
					      (convert 'list)
					      cdar)))
			    (script-contents result)))
      (contents-stdTileviewFragShader ()				      
				      (let* ((ns (root-stdtileviewfragshader *this*))
					     (result (->> (root-scripts *this*)
							  (filter (lambda (b script)
		    						    (equalp (script-ns script) ns)))
							  (convert 'list)
							  cdar)))
					(script-contents result)))
      
      (registrySize ()
		    (reduce #'+ (mapcar (lambda (col)
					  (length (convert 'list col)))
					(list
					 (root-layers *this*)
					 (root-maps *this*)
					 (root-Scripts *this*)
					 (root-tiles *this*)
					 (root-tilesets *this*)))))
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
		      (let ((concd (->> (reduce #'concat (seq
						    ;; (root-animatedSprites *this*)
						    (root-layers *this*)
						    (root-maps *this*)
						    (root-scripts *this*)
						    ;; (root-sprites *this*)
						    (root-tiles *this*)
						    (root-tilesets *this*)))
					(mapcar #'cdr)
					)))
			(format t "concd is ~a~%" concd)
			concd)))))

(defun-export! init-root! ()
  (make-root '() '() '() '() '() 0 0 0 nil "defaultVertex" "defaultFragment" "default.tileView"))
      
(defun-export! push-map (root m)
  (let* ((maps (cons (cons (gensym)
			   m)
		     (root-maps root)))
	 (r (if (not (root-chosenMap root))
		(set-root-chosenMap! root (first maps))
		root))
	 (final-root (set-root-maps! root maps)))
    ;; (format t "Final root at push-map is ~a~%" final-root)
    final-root))

(defun-export! find-by-id (root id)
  (let* ((result-set (->> root
			  root-registrytolist
			  (mapcat (lambda (r)
				    (if (string= (q-type-of r) "MAP")
					(cons r
					      (get-prop r "LAYERS"))
					(list r))))
			  (remove-if-not (lambda (r)
					   ;; (format t "(= ~a ~a)~%" (symbol-name (get-prop r "ID")) id)
					   (let* ((row-id (get-prop r "ID"))
						  (row-id (if (symbolp row-id)
							      (symbol-name row-id)
							      row-id)))
			 		     (string= row-id id))))))
	 (len (length result-set)))
    ;; (format t "result-set: ~a~%" result-set)
    ;; (format t "find-by-id found ~a elements~%" len)
    (when (> len 0)
	(when (> len 1)
	  (format t "find-by-id found ~a elements~%" len))
	(car result-set))))

(defun-export! push-script (root scr)
  ;; (declare (optimize (debug 3)))
  (assert root)
  (let ((result 
	 (set-root-scripts! root
			    (let ((result (with (or (root-scripts root) (empty-map))
						(gensym)
						scr)))
			      result))))
    (format t "Lol @ push-script~%")
    result))

;; (push-script *document* (make-script "Lollo" "name" "ns" "glsl"))

(defun-export! find-ns (root ns)
  (let ((scr (filter
	      (lambda (scr)
		(equalp (Script-ns scr) ns))
	      (root-scripts root))))
    (if (not scr)
	'()
	(car scr))))
    
(defun-export! is-ns-scheme? (root ns)
  (let ((scr (find-ns root ns)))
    (Script-is-scheme? (car scr))))

(defun-export! is-ns-glsl? (root ns)
  (let ((scr (find-ns root ns)))
    (Script-is-glsl? (car scr))))

(defun-export! push-sprite-to-chosen-map (root sprite)
  (let ((chosen-map (-> (root-chosenMap root)
			(push-sprite sprite))))
    (push-selected-map root chosen-map)))

(defvar *document* (init-root!))
(export '*document*)

(defun-export! set-doc (doc)
  (assert (not (functionp doc)))
  (assert doc)
  ;; TODO tee tästä joku c++:aan eventtikokoelmaan dispatchaava kikkare
  (format t "set doc called~%")
  (if explode
      (funcall explode))
  (setf *document* doc))
