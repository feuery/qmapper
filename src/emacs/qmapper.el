(defconst proc-name "*qmapper-client*")
;; (defsubst client-process ()
;;   (get-process proc-name))

(defun qmapper-fetch-ns (server port ns)
  (interactive "sServer: \nnPort: \nsNS: ")
  (let ((proc (open-network-stream proc-name (get-buffer-create proc-name)
				   server port
				   :nowait nil)))
    (process-send-string proc (concat ns "\n"))

    (switch-to-buffer (get-buffer-create proc-name))
    (erase-buffer)
    (scheme-mode)
    (geiser-mode)
    (end-of-buffer)
    (backward-delete-char-untabify 1)
    (move-beginning-of-line nil)
    (kill-line)))

(qmapper-fetch-ns "localhost" 33791 "user")
