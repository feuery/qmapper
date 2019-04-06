(defpackage :qmapper.root
  (:use :common-lisp
	:cl-arrows
	:rutils.abbr
	:qmapper.std
	:qmapper.export
   :qmapper.script)
  (:shadowing-import-from :fset :empty-map :with-first :with :seq :image :lookup ;; :filter
   :reduce :size :concat :convert :wb-map-from-list)
  (:shadowing-import-from :cl-strings :replace-all)
  (:import-from :qmapper.export :clear-lisp-drawingqueue :add-lambda-to-drawingqueue))

(in-package :qmapper.root)

(defun-export! set-image-x (dst img x)
  (let ((i (get-prop img "GL-KEY")))
    (funcall set-img-x (symbol-name dst) i x)))

(defun-export! set-image-y (dst img y)
  (funcall set-img-y (symbol-name dst) (get-prop img "GL-KEY") y))

(defun-export! set-image-rotation (dst img rotation)
  (funcall set-img-rotation (symbol-name dst) (get-prop img "GL-KEY") rotation))

(defun-export! set-image-opacity (dst img op)
  (funcall set-img-opacity (symbol-name dst) (get-prop img "GL-KEY") op))

(defun-export! load-img (path)
  (funcall load-image path))

(defun-export! img-width (img)
  (funcall image-w img))

(defun-export! img-height (img)
  (funcall image-h img))

(defun-export! copy-img (img x y w h)
  (funcall copy-image img x y w h))

(defun-export! add-to-drawqueue (img dst-key)
  (let ((i (get-prop img "GL-KEY")))
    (assert i)
    (funcall add-to-drawingqueue i (symbol-name dst-key))))

(defun-export! clear-draw-queue (dst-key)
  (funcall clear-drawingqueue (symbol-name dst-key)))

(defun-export! schedule-once (dst l)
  (format t "scheduling once to dst ~a~%" (symbol-name dst))
  (funcall do-schedule-lambda (symbol-name dst) l))

(defun script-ns->id (root ns)
  (clean-key 
   (get-prop (->> root
		  root-scripts 
		  (convert 'list)
		  (mapcar #'cdr)
		  (filter (lambda (s)
			    (format t "s is ~s" s)
			    (string= (script-ns s)
				     ns)))
		  car) "ID")))

(defcppclass root
    (public   
     (fields
      ;; defcppclass macro should ignore the type specifier
      ;; and these are supposed to be (gensym) -> 'a plists 
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
      (StdTileviewFragShader "default.tileView")
      (animatedSprites '())
      (sprites  '()))
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
	      (let* ((list-scripts (convert 'list (root-scripts *this*)))
		     (scripts (mapcar #'cdr list-scripts)))
		(let ((result (car (filter (lambda (script)
					     (equalp (Script-ns script) ns)) scripts))))
		  result)))
      (saveNs (ns content)
	      (let* ((scripts (root-scripts *this*))
		     (id (script-ns->id *this* ns))
		     (new-script (-> (get-prop scripts id)
				     (set-Script-Contents! content)))
		     (scripts (set-prop scripts id new-script)))
		(set-root-scripts! *this* scripts)))
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
		      (let* ((s (->> (concatenate 'list
		      				  (convert 'list (root-animatedSprites *this*))
		      				  (convert 'list (root-layers *this*))
		      				  (convert 'list (root-maps *this*))
		      				  (convert 'list (root-scripts *this*))
		      				  (convert 'list (root-sprites *this*))
		      				  (convert 'list (root-tiles *this*))
		      				  (convert 'list (root-tilesets *this*)))
				     (mapcar (lambda (l)
					       (if (symbolp (car l))
						   (cons (replace-all (symbol-name (car l)) "\"" "")
							 (cdr l))
						   (cons (replace-all (car l) "\"" "")
							 (cdr l)))))))
			     (alist (->>  s
					  seq
					  (reduce #'concat)))
			     (concd (alist->map alist)))
		      	concd)))))

(defun-export! get-selected-tileset (root)
  (let ((selected-ind (root-chosenTileset root)))
    (format t "selected tileset is ~a~%" selected-ind)
    (cdr (nth selected-ind (convert 'list (root-tilesets root))))))

(defun-export! init-root! ()
  (make-root (empty-map) (empty-map) (empty-map) (empty-map) (empty-map) 0 0 0 nil "defaultVertex" "defaultFragment" "default.tileView" (empty-map) (empty-map)))

;; test code for events
;; (-> (init-root!)
;;     (set-prop "maps" 22)
;;     (add-event!  "maps" (lambda (this x)
;; 			  (format t "Sijoitettiin x-propsuun ~a~%" x)
;; 			  this))
;;     (add-event! "maps" (lambda (this m)
;; 			 (set-prop this "lolz" (* m m))))
;;     (update-prop "maps" #'inc)
;;     (update-prop "maps" #'inc)
;;     (get-prop "lolz"))
(defun-export! push-map (root m)
  (let* ((maps (set-prop (root-maps root)
			 (get-prop m "ID")
			 m))
	 (r (if (not (root-chosenMap root))
		(set-root-chosenMap! root (get-prop m "ID"))
		root))
	 (final-root (set-root-maps! root maps)))
    ;; (format t "Final root at push-map is ~a~%" final-root)
    final-root))

;; (get-prop-in *document* '(maps))
;; (assoc 'G1468 '((G1468 . 2323)))

(defun-export! find-path-of (root obj)
  (let ((type-sym (q-type-of obj)))
    (unless type-sym
      (format t "didn't recognize type ~a~%" (q-type-of obj))
      (format t "obj is ~a~%" obj))
    (let ((key (get-prop obj "ID")))
      (list type-sym (clean-key key)))))
		    

(defun-export! find-by-id (root id)
;  (declaim (optimize (debug 3)))
  (let* ((result-set (->> root
			  root-registrytolist
			  (convert 'list)
			  (mapcar #'cdr)
			  (remove-if-not (lambda (r)
					   (let* ((row-id (get-prop r "ID"))
						  (row-id (replace-all (if (symbolp row-id)
									   (symbol-name row-id)
									   row-id)
								       "\"" ""))				    
						  (result (string= row-id id)))
					     (format t "(string= ~a ~a) => ~a~%" (prin1-to-string row-id) (prin1-to-string id) result)
					     result)))))
	 (len (length result-set)))
    ;; (format t "result-set: ~a members~%" len)
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
			    (let* ((root-scripts (or (root-scripts root) (empty-map)))
				   (_ (format t "root-scripts: ~a~%" root-scripts))
				   (result (set-prop root-scripts
						 (get-prop scr "ID")
						 scr)))
			      result))))
    result))

    
(defun-export! is-ns-lisp? (root ns)
  (let ((scr (root-findNs root ns)))
    (format t "found script ~a~%" scr)
    (Script-is-lisp? scr)))

(defun-export! is-ns-glsl? (root ns)
  (let ((scr (root-findNs root ns)))
    (Script-is-glsl? scr)))

(defun-export! push-sprite2 (root mapInd sprite-id sprite)
  (-> root
      (update-prop 'sprites (lambda (sprites)
			      (format t "updating sprites~%")
			      (let ((res
				      (-> sprites
					  (set-prop sprite-id 
						    (set-prop sprite 'id (clean-key sprite-id))))))
				(format t "updated sprites~%")
				res)))
      (update-prop-in (list "maps" mapInd "sprites") (lambda (sprites)
						       (with-first sprites (clean-key sprite-id))))))

(defun-export! push-animation (root mapInd animation)
  (let ((animation-id (get-prop animation "ID")))
    (-> root
	(update-prop 'animatedSprites (lambda (animatedSprites)
					(format t "updating animatedSprites~%")
					(let ((res
						(-> animatedSprites
						    (set-prop animation-id 
							      (set-prop animation 'id animation-id)))))
					  (format t "updated animatedSprites~%")
					  res)))
	(update-prop-in (list "maps" mapInd "animatedSprites") (lambda (animatedSprites)
							       (with-first animatedSprites animation-id))))))

(defvar *document* (init-root!))
(defvar *engine-document* nil)
(export '*engine-document*)
(export '*document*)

(defvar *document-hooks* '())

(defun-export! set-doc (doc)
  (assert (not (functionp doc)))
  (assert doc)

  ;; (dolist (c (convert 'list doc))
  ;;   (assert (not (symbolp (car c)))))
  
  ;; TODO tee tästä joku c++:aan eventtikokoelmaan dispatchaava kikkare
  (setf *document* doc)
  (dolist (l *document-hooks*)
    (if (and l
	     (functionp l))
	(funcall l doc))))

(defun-export! register-doc-hook (l)
  (if (and l
	   (functionp l))
      (push l *document-hooks*)
      (format t "hook ~a can't be registered~%" l)))

(defun-export! clear-engine-document! ()
  (setf *engine-document* *document*)
  *engine-document*)

(defun-export! tileset-count! ()
  (size (root-tilesets *document*)))

(defun-export! load-root (data)
  (-> data
      read-from-string
      eval))
