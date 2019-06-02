;;;; qmapper-loader.lisp

(in-package #:qmapper-loader)

(defparameter *asdf-path* nil)
(defparameter *asdf-contents* nil)
(defparameter *location* nil)
(setf compiler::*delete-files* nil)

(let ((quicklisp-init (merge-pathnames "quicklisp/setup.lisp"
                                       (user-homedir-pathname))))
  (when (probe-file quicklisp-init)
    (load quicklisp-init)))

;; (defun dump-me ()
;;   (push #P"./" asdf:*central-registry*)
;;   (asdf:make-build :qmapper-loader 
;; 		   :type :program
;; 		   :move-here #P"./"
;; 		   :epilogue-code '(progn
;; 				    (main #P"/home/feuer/Dropbox/qt-test/src/lisp/qmapper.asd")
;; 				    (si:exit)))
  
;;   ;; (compile-file "qmapper-loader.lisp" :system-p t)
;;   ;; (c:build-program "qmapper-loader"
;;   ;; 		   :lisp-files (list "qmapper-loader.o")
;;   ;; 		   :prologue-code '(progn
;;   ;; 				    (main #P"/home/feuer/Dropbox/qt-test/src/lisp/qmapper.asd")))
;;   )

(defun slurp (filename)
  (with-open-file (stream filename)
    (let ((contents (make-string (file-length stream))))
      (read-sequence contents stream)
      contents)))

(defun spit (filename data)
  (with-open-file (stream filename :direction :output
			  :if-exists :overwrite
			  :if-does-not-exist :create)
    (format stream data)))

(defun get-system-form (contents)
  (car (remove-if-not (lambda (l)
			(string= (symbol-name (car l)) "DEFSYSTEM"))
		      (cdr contents))))

(defun components (sysform)
  (getf sysform
	:components))

;; copypasted from https://rosettacode.org/wiki/Topological_sort#Common_Lisp
(defun topological-sort (graph &key (test 'eql))
  "Graph is an association list whose keys are objects and whose
values are lists of objects on which the corresponding key depends.
Test is used to compare elements, and should be a suitable test for
hash-tables.  Topological-sort returns two values.  The first is a
list of objects sorted toplogically.  The second is a boolean
indicating whether all of the objects in the input graph are present
in the topological ordering (i.e., the first value)."
  (let ((entries (make-hash-table :test test)))
    (flet ((entry (vertex)
             "Return the entry for vertex.  Each entry is a cons whose
              car is the number of outstanding dependencies of vertex
              and whose cdr is a list of dependants of vertex."
             (multiple-value-bind (entry presentp) (gethash vertex entries)
               (if presentp entry
                   (setf (gethash vertex entries) (cons 0 '()))))))
      ;; populate entries initially
      (dolist (vertex graph)
        (destructuring-bind (vertex &rest dependencies) vertex
          (let ((ventry (entry vertex)))
            (dolist (dependency dependencies)
              (let ((dentry (entry dependency)))
                (unless (funcall test dependency vertex)
                  (incf (car ventry))
                  (push vertex (cdr dentry))))))))
      ;; L is the list of sorted elements, and S the set of vertices
      ;; with no outstanding dependencies.
      (let ((L '())
            (S (loop for entry being each hash-value of entries
                  using (hash-key vertex)
                  when (zerop (car entry)) collect vertex)))
        ;; Until there are no vertices with no outstanding dependencies,
        ;; process vertices from S, adding them to L.
        (do* () ((endp S))
          (let* ((v (pop S)) (ventry (entry v)))
            (remhash v entries)
            (dolist (dependant (cdr ventry) (push v L))
              (when (zerop (decf (car (entry dependant))))
                (push dependant S)))))
        ;; return (1) the list of sorted items, (2) whether all items
        ;; were sorted, and (3) if there were unsorted vertices, the
        ;; hash table mapping these vertices to their dependants
        (let ((all-sorted-p (zerop (hash-table-count entries))))
          (values (nreverse L)
                  all-sorted-p
                  (unless all-sorted-p
                    entries)))))))

(defun get-files-to-load-in-topological-order (contents)
  (let* ((list (components (get-system-form contents)))
	 (alist (mapcar (lambda (plist)
			  (cons (getf plist :file) (getf plist :depends-on)))
			list)))
    (topological-sort alist :test #'equal)))

(defun path-directory-str (p)
  (format nil "~{/~a~}" (cdr (pathname-directory p))))

(defun filename (path)
  (format nil "~a.~a" (pathname-name path) (pathname-type path)))

(defun set-filename (path new-filename)
  (pathname (format nil "~a/~a" (path-directory-str path) new-filename)))

(defun main (p)
  (setf *asdf-path* p)
  (setf *asdf-contents* (read-from-string (format nil "(progn ~a)" (slurp p))))
  (setf *location* (path-directory-str p))
  (let* ((files-to-load (cons p (mapcar (lambda (x)
				  (format nil "~a/~a.lisp" *location* x))
					(get-files-to-load-in-topological-order *asdf-contents*))))
	 (script (reduce (lambda (script filename)
			   (format nil "~a ~a" script (slurp filename)))
			 files-to-load
			 :initial-value "")))
    (cl-fad:with-open-temporary-file (stream :keep t)
      (let ((filename (pathname stream))
	    (target-filename (set-filename *asdf-path* (format nil "loadable_~a.lisp"
							       (regex-replace-all ".asd"
							       (filename *asdf-path*) ""))))
	    ;; (object-filename (format nil (regex-replace-all "asd" (format nil "~a" *asdf-path*) "lisp")))
	    )
	;; (format t "filename on ~a~%" filename)
	;; (format t "obj-filename on ~a~%" object-filename)
	(format stream "~a" script)
	;; (load filename)
	;; (format t "Lolz1~%")
	;; (compile-file filename :system-p t)
	(format t "~a" target-filename)

	(asdf:run-shell-command (format nil "mv ~a ~a" filename target-filename))
	
	;; (c:build-static-library "qmapper_lisp"
	;; 			:lisp-files (list object-filename)
	;; 			:init-name "initialisaatio_funktio")
	;; (c:build-fasl "qmapper_lisp"
	;; 	      :lisp-files (list object-filename))
	;; (asdf:run-shell-command "pwd && ls")
	;; (format t "~%Done!~%")
	))))
    
(export 'main)

;; (main #P"/home/feuer/Dropbox/qt-test/src/lisp/qmapper.asd")
;; (path-directory-str #P"/home/feuer/Dropbox/qt-test/src/lisp/qmapper.asd")
