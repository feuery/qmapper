(defpackage :qmapper.doc-server
  (:use :common-lisp
	:qmapper.std
	:ppcre
	:qmapper.script
	:qmapper.root 
	:cl-arrows
	:qmapper.export
	:usocket
	:bordeaux-threads))

(in-package :qmapper.doc-server)

(defparameter *server-running?* t)

(defmacro while (condition &body body)
  `(loop while ,condition
      do (progn ,@body)))

;; pohja lainattu https://gist.github.com/traut/6bf71d0da54493e6f22eb3d00671f2a9

(defun send-text-to-socket (text socket)
  (let ((socket-stream (usocket:socket-stream socket)))
    (format
      socket-stream
      (format nil "~a~%" text))  ; adding a line break at the end for prettiness
    (force-output socket-stream)))

(defun logger (text &rest args)
  "Simple wrapper around format func to simplify logging"
  (apply 'format (append (list t (concatenate 'string text "~%")) args)))


(defun close-socket (socket)
  "Close a socket without raising an error if something goes wrong"
  (handler-case
      (usocket:socket-close socket)
    (error (e)
      (logger "ignoring the error that happened while trying to close socket: ~a" e)))
  (logger "socket closed"))


(defun run-tcp-server (port)
  "Run TCP server in a loop, listening to incoming connections.
  This is single-threaded version. Do NOT call this in REPL, in qmapper this'll
  be wrapped inside a QThread"
  (format t "port in run-tcp-server: ~a~%" port)
  (let* ((host "127.0.0.1")
	 (master-socket (usocket:socket-listen host port :backlog 256))
         (all-sockets `(,master-socket)))
    (format t "let's wait-for-input~%")
    (loop 
       (loop for sock in (usocket:wait-for-input all-sockets :ready-only t)
          do (progn
	       (format t "jej :D ~%")
	       (if (eq sock master-socket)
                   (let ((client-socket
                          (usocket:socket-accept master-socket :element-type 'character)))
                     (push client-socket all-sockets)
                     (logger "new socket initiated: ~a" client-socket)
		     (process-client-socket client-socket))
					; client socket activity
                   ;; (handler-case
		   ;;     (progn
		   (logger "processing client socket~%")
                   (process-client-socket sock);; )
                   ;; (t (e)
                   ;;   (logger "error during processing ~a" e)
                   ;;   (setf all-sockets (delete sock all-sockets))
		   ;;   (close-socket sock))
		   ))))));)

(defun-export! run-tcp-server-threaded (port)
  (make-thread (lambda ()
		 (run-tcp-server port)) :name "doc-server-thread"))

;; protocol parser

(defun eval-protocol-row (row)
  (format t "evalling row ~a~%" row)
  (let ((split-row (cl-strings:split row ":")))
    (when (pos? (length split-row))
      (let ((protocol (car split-row)))
	(values protocol
		(condp string= protocol
		       "NS" (let* ((ns (nth 1 split-row))
				   (_ (format t "finding ns ~a~%" ns))
				   (script (root-findns *document* ns)))
			      (if script
				  (let* ((contents (get-prop script "CONTENTS"))
					 (_ (format t "script is ~a~%" script))
					 (firstLine (car (cl-strings:split contents (format nil "~%"))))
					 (is-lisp? (script-is-lisp? script))
					 (modeline-scanner (create-scanner "mode: (lisp|glsl)"))
					 (contains-modeline? (scan modeline-scanner contents)))
				    (if contains-modeline?
					contents
					(if is-lisp?
					    (format nil "; -*- mode: lisp; -*-~%~a" contents)
					    (format nil "; -*- mode: glsl; -*-~%~a" contents))))
				  nil))
		       "SAVE-NS" (let ((ns (nth 1 split-row))
				       (contents (cl-strings:replace-all row (format nil "~a:~a:"
										     (car split-row)
										     (cadr split-row)) "")))
				   (root-savens *document* ns contents))))))))

(defun process-client-socket (client-socket)
  "Process client socket that got some activity"
  (let ((message (read-line (usocket:socket-stream client-socket))))
    (logger "got a message: ~a" message)
    (multiple-value-bind (protocol result) (eval-protocol-row message)
      (if (string= protocol "NS")
	  (format (socket-stream client-socket) "~a" result)
	  (if (string protocol "SAVE-NS")
	      (set-doc result))))))
