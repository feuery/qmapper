(defpackage :qmapper.resize-test
  (:use :common-lisp
	:qmapper.test-framework
   :cl-arrows
	:qmapper.std
   :qmapper.map
	:qmapper.layer
   :qmapper.root)
  
  (:import-from :fset :size :convert :empty-map :with))

(in-package :qmapper.resize-test)


(defvar grow-w (random 99))
(defvar grow-h (random 99))

(deftest testing-layer-growth
  (let ((qmapper.root:*document* (make-map-with-layers *document* "Lol" 2 2 1)))
    (let* ((w (random 44))
	   (h (random 66))
	   (new-w (+ w (random 23)))
	   (new-h (+ h (random 22)))
	   (new-layer (resize-layer (make-layer "Lol" 254 t (make-tiles w h)) new-w new-h)))
      ; (format t "old dims ~a, new expected dims ~a, new dims ~a~%" (list w h) (list new-w new-h) (list (layer-width new-layer) (layer-height new-layer)))
      
      (is (layer-width new-layer) new-w)
      (is (layer-height new-layer) new-h))))

(deftest testing-layer-shrink
    (let ((qmapper.root:*document* (make-map-with-layers *document* "Lol" 2 2 1))
	  (w (+ 50 (random 44)))
	  (h (+ 50 (random 66))))
      (assert (> w 50))
      (assert (> h 50))
      (let* ((new-w 50)
	     (new-h 50)
	     (new-layer (assert-some? (resize-layer (make-layer "Lol" 222 t (make-tiles w h)) new-w new-h))))
	(is (layer-width new-layer) new-w)
	(is (layer-height new-layer) new-h))))

(deftest testing-vertical-layer-growth
    (let ((qmapper.root:*document* (make-map-with-layers *document* "Lol" 2 2 1)))
      (let* ((w (random 44))
	     (h (random 66))
	     (new-layer (grow-layer-vertically (make-layer "Lol" 254 t (make-tiles w h))))
	     (result (layer-width new-layer))
	     (new-h (layer-height new-layer)))
	;; (format t "(equalp ~a ~a)?~%" result (inc w))
	;; (format t "(equalp ~a ~a)?~%" new-h (inc h))
	(is (inc h) new-h))))

(deftest testing-vertical-layer-shrink
    (let ((qmapper.root:*document* (make-map-with-layers *document* "Lol" 2 2 1)))
      (let* ((w (random 44))
	     (h (random 66))
	     (new-layer (assert-some? (shrink-layer-vertically (make-layer "sdfsf" 222 t (make-tiles w h)))))
	     (new-h (layer-height new-layer)))
	(is (dec h) new-h))))

(deftest testing-horizontal-layer-growth
  (let* ((qmapper.root:*document* (make-map-with-layers *document* "lal" 2 2 1))
	 (w (random 66))
	 (h (random 55))
	 (new-layer (assert-some? (grow-layer-horizontally (make-layer "#sdfsdf" 255 t (make-tiles w h)))))
	 (result-w (layer-width new-layer)))
    (is (inc w) result-w)))

(deftest testing-horizontal-layer-shrink
  (let* ((qmapper.root:*document* (make-map-with-layers *document* "lal" 2 2 1))
	 (w (random 66))
	 (h (random 55))
	 (new-layer (assert-some? (shrink-layer-horizontally (make-layer "#sdfsdf" 255 t (make-tiles w h)))))
	 (result-w (layer-width new-layer)))
    (is (dec w) result-w)))

(deftest test-map-resize
  (let* ((qmapper.root:*document* (make-map-with-layers *document* "lal" 2 2 1))
	 (new-w (+ 3 (random 22)))
	 (new-h (+ 3 (random 22)))
	 (qmapper.root:*document* (resize-selected-map qmapper.root:*document* new-w new-h))
	 (selected-map (get-prop-in qmapper.root:*document* (list "MAPS" (root-chosenmap qmapper.root:*document*)))))
    (is (map-width selected-map) new-w)
    (is (map-height selected-map) new-h)))
	 

;; (deftest testing-resize
;;     (let* ((test-root (make-map-with-layers (init-root!) "Lol" 2 2 1))
;;     	   (qmapper.root:*document* test-root)
;;     	   (chosenmapid (root-chosenmap test-root))
;;     	   (map (assert-some? (get-prop-in test-root (list "MAPS" chosenmapid))))
;;     	   (old-w (map-width map))
;;     	   (old-h (map-height map))
;;     	   (test-root (assert-some? (resize-selected-map test-root grow-w grow-h)))
;;     	   (qmapper.root:*document* test-root)
;;     	   (map (assert-some? (get-prop-in test-root (list "MAPS" chosenmapid)))))
      
;;       (is (+ old-w grow-w) (map-width map))
;;       (is (+ old-h grow-h) (map-height map))))

;; (run-tests)
