(defpackage :qmapper.transitions
  (:use :common-lisp
	:qmapper.std
	:ppcre
	:qmapper.root 
	:cl-arrows
	:qmapper.export
	:bordeaux-threads))

(in-package :qmapper.transitions)

(defun-export! make-event (state-change-fn doc-path &rest rest-params)
  (fset:map ("STATE-CHANGE-FN" state-change-fn)
	    ("DOC-PATH" doc-path)
	    ("REST-PARAMS" rest-params)))

(defvar *message-queue* nil)

(defvar *dispatcher-running* t)

(defun dispatcher ()
  (while *dispatcher-running*
    (if-let (event (pop *message-queue*))
      (let ((fn (get-prop event "state-change-fn"))
	    (doc-path (get-prop event "doc-path"))
	    (rest-params (get-prop event "rest-params")))
	(set-engine-doc (update-prop-in *engine-document* doc-path (lambda (obj)
								     (format t "fn is ~a~%" fn)
								     (apply fn obj rest-params))))))
    (ms-sleep 2)))

(defun-export! start-dispatcher! ()
  (make-thread #'dispatcher :name "Dispatcher thread"))

(start-dispatcher!)
;; (setf *dispatcher-running* nil)
;; (push (make-event #'+
;; 		  (list "SPRITES" (caar (fset:convert 'list (root-sprites *document*))) "X")
;; 		  20)
;;       *message-queue*)
