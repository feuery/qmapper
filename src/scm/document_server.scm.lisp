(define-module (document-server))

(use-modules (web server)) ; you probably did this already
(use-modules (web request)
             (web response)
             (web uri))

(use-modules (qmapper-std))

(define (request-path-components request)
  (split-and-decode-uri-path (uri-path (request-uri request))))

(str (write "lol") (write '(1 2 3))

(define (not-found request)
  (values (build-response #:code 404)
          (string-append "Resource not found: "
                         (uri->string (request-uri request)))))

(define (hello-hacker-handler request body)
  ;(values '((content-type . (text/plain)))

  (if (equal? (request-path-components request)
              '("hacker"))
      (values '((content-type . (text/plain)))
              "Hello hacker!")
      (not-found request))
  )

(define server-thread (call-with-new-thread (lambda ()
					      (run-server hello-hacker-handler))))


;; (paths
;;  (GET "/lol/:param/" (param)
;;       (str param " is param"))
;;  (POST "/lol/:lol" (lol)
;;        (* (string->number lol) 2)))
