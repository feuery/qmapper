;; -*- lexical-binding: t -*-

(defvar qmapper-server ""
  "QMapper server's location/IP")
(defvar qmapper-port nil
  "QMapper server's port")
(defvar qmapper-ns ""
  "NS current qmapper-buffer is visiting")

(make-variable-buffer-local 'qmapper-server)
(make-variable-buffer-local 'qmapper-port)
(make-variable-buffer-local 'qmapper-ns)

(defun qmapper-fetch-ns (server port ns)
  (let ((buffer-name (concat "QMAPPER: " ns)))
    (if (get-buffer buffer-name)
	(kill-buffer buffer-name))
    
    (let ((proc (open-network-stream buffer-name (get-buffer-create buffer-name)
				     server port
				     :nowait nil)))
      ;; (message (concat "Process coding system is " (prin1-to-string (process-coding-system proc))))
      (process-send-string proc (concat "NS:" ns "\n"))
      (set-process-sentinel proc (lambda (p e)
				   (with-current-buffer (get-buffer-create buffer-name)
				     (switch-to-buffer (get-buffer-create buffer-name))

				     (goto-char 0)

				     (cond
				      ((save-excursion
					 (search-forward "-*- mode: lisp; -*-" nil t))
				       (lisp-mode))
				      ((save-excursion
					 (search-forward "-*- mode: glsl" nil t))
				       (glsl-mode)))
				     
				     (qmapper-editor-mode)
				     (rename-buffer buffer-name)

				     
				     (setq qmapper-server server)
				     (setq qmapper-port port)
				     (setq qmapper-ns ns)))))))

(defun qmapper-save-ns ()
  (interactive)
  (let* ((server qmapper-server)
	 (port qmapper-port)
	 (ns qmapper-ns)
	 (buffer-name (concat "QMAPPER: " ns)))
    (if (not (equal ns ""))
	(with-current-buffer buffer-name
	  (let* ((buf-content (buffer-string))
		 (n-of-rows (length (split-string (buffer-substring-no-properties (point-min) (point-max)) "\n")))
		 (proc (open-network-stream "*qmapper-tmp*" (get-buffer-create "*qmapper-tmp*")
					    server port
					    :nowait nil)))
	    (process-send-string proc (concat "SAVE-NS:" ns ":" (int-to-string n-of-rows) "\n" buf-content "\n"))
	    (set-process-sentinel proc (lambda (p e)
					 (set-buffer-modified-p nil)
					 (message (concat "Saved " ns " to " server ":" (int-to-string port)))))))
      (save-buffer))))

(defun qmapper-find-file (find-file &rest args)
  (message (concat "Searching " (prin1-to-string (car args))))
  (let* ((path (car args)))
      (cond
       ((string-match "^QM:\\(\\w+\\):\\([0-9]+\\):\\(.+\\)" path)
	(let ((server (match-string 1 path))
	      (port (string-to-number (match-string 2 path)))
	      (ns (match-string 3 path)))
	  (qmapper-fetch-ns server port ns)))
       (t (apply find-file args)))))
  

(defun to-kill-buffer ()
  (interactive)
  (if (or (not (buffer-modified-p))
  	       (and (buffer-modified-p)
  		    (y-or-n-p (concat "Buffer " (prin1-to-string (current-buffer)) " modified. Kill anyway?"))))
      (kill-buffer (current-buffer))))

(define-minor-mode qmapper-editor-mode
  "QMapper doc-server client"
  :lighter " qmapper"
  :keymap (let ((map (make-sparse-keymap)))
	    (define-key map (kbd "C-x C-s") #'qmapper-save-ns)
	    (define-key map (kbd "C-x k") 'to-kill-buffer)
	    (advice-add 'find-file :around #'qmapper-find-file)
						   
	    map)  
  :global nil)
				     
	

;; (qmapper-fetch-ns "localhost" 40359 "user")
;; (qmapper-save-ns  "localhost" 40359 "user")t
