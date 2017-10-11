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
      (process-send-string proc (concat ns "\n"))
      (set-process-sentinel proc (lambda (p e)
				   (switch-to-buffer (get-buffer-create buffer-name))
				   (scheme-mode)
				   (geiser-mode)
				   (rename-buffer buffer-name))))))

;; (qmapper-fetch-ns "localhost" 34983 "user")
