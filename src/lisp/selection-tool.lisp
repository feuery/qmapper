(defpackage :qmapper.selection-tool
  (:use :common-lisp
	:qmapper.editor_events
	:qmapper.map
	:qmapper.tile
	:cl-arrows
	:qmapper.export
	:qmapper.root
	:qmapper.std)
  (:shadowing-import-from :fset :empty-map :with-first :with :seq :image :lookup ;; :filter
			  :reduce :size :concat :convert :wb-map-from-list))

(in-package :qmapper.selection-tool)

(defun get-points-in-selection (left top right bottom)
  (loop for x in (drop-last (minmax-range left right))
     append (loop for y in (drop-last (minmax-range top bottom))
	       collect (list x y))))

(defvar *clipboard* nil)



(add-key-lambda "C-X"
		(lambda ()
		  (let* ((selection (root-selected-coordinates *document*))
		  	 (left (fset:lookup selection 0))
		  	 (top (fset:lookup selection 1))
		  	 (right (fset:lookup selection 2))
		  	 (bottom  (fset:lookup selection 3))
		  	 (selected-tile-coord-pairs (get-points-in-selection left top right bottom))
		  	 (tiles (mapcar (lambda (coord-pair)
					  (get-tile-at2 (root-chosenLayerInd *document*) (car coord-pair) (cadr coord-pair)))
					selected-tile-coord-pairs)))
		    (format t "Eating tiles from ~a to ~a~%" (list left top) (list right bottom))
		    (setf *clipboard* (fset:map ("x" left)
						("y" right)
						("tiles" tiles)))
		    (set-doc (reduce (lambda (doc tile-coord-pair)
				       (let ((x (car tile-coord-pair))
					     (y (cadr tile-coord-pair)))
					 (set-tile-at-chosen-map doc x y (make-tile 0 0 0 0 nil))))
				     selected-tile-coord-pairs :initial-value *document*)))))
