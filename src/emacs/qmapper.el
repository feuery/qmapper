;; -*- lexical-binding: t -*-

(defun qmapper-fetch-ns (server port ns)
  (interactive "sServer: \nnPort: \nsNS: ")
  (let ((buffer-name (concat "QMAPPER: " ns)))
    (if (get-buffer buffer-name)
	(kill-buffer buffer-name))
    
    (let ((proc (open-network-stream buffer-name (get-buffer-create buffer-name)
				     server port
				     :nowait nil)))
      ;; (message (concat "Process coding system is " (prin1-to-string (process-coding-system proc))))
      (process-send-string proc (concat "NS:" ns "\n"))
      (set-process-sentinel proc (lambda (p e)
				   (switch-to-buffer (get-buffer-create buffer-name))
				   (scheme-mode)
				   (geiser-mode)
				   (rename-buffer buffer-name))))))

(defun qmapper-save-ns (server port ns)
  ;; TODO make the params buffer-locals or smthg
  (interactive "sServer: \nnPort: \nsNS: ")
  (let ((buffer-name (concat "QMAPPER: " ns)))
    (with-current-buffer buffer-name
      (let* ((buf-content (buffer-string))
	     (proc (open-network-stream "*qmapper-tmp*" (get-buffer-create "*qmapper-tmp*")
				     server port
				     :nowait nil)))
	(process-send-string proc (concat "SAVE-NS:" ns ":" buf-content))
	(set-process-sentinel proc (lambda (p e)))))))
				     
	

;; (qmapper-fetch-ns "localhost" 40359 "user")
;; (qmapper-save-ns  "localhost" 40359 "user")
