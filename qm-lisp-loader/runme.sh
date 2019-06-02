#!/usr/bin/ecl --shell
;; -*- mode: Lisp; fill-column: 75; comment-column: 50; -*-
(load #P"./package.lisp")
(load #P"./qmapper-loader.lisp")

(qmapper-loader:main (format nil "/home/~a/Dropbox/qt-test/src/lisp/qmapper.asd" (uiop:run-program "whoami" :output '(:string :stripped t))))

		     

;; #P
