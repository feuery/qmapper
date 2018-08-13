(defpackage :qmapper.animatedsprite
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.animatedsprite)

(defcppclass animatedsprite
  (public
   (properties
    (parentMapId "")
    (name "")
    (currentFrameId 0)
    (msPerFrame 25)
    (animationPlaying true)
    (lastUpdated 0)
    
    (x 0)
    (y 0)
    (angle 0.0)
    
    (sprites '()))
   (functions
    (maxFrames ()
	       (length (animatedsprite-sprites *this*)))
    (advanceFrame! ()
		   (let* ((tt (animatedsprite-advanceFrameIfNeeded! *this*))
			  (frameid (animatedsprite-currentFrameId tt))
			 (frameid (modulo frameid (animatedsprite-maxFrames tt))))
		    (set-animatedsprite-currentFrameId! tt frameid)))
    (render ()
    	    (let* ((sprites (animatedsprite-sprites *this*))
    		   (current-sprite (list-ref sprites (animatedsprite-currentFrameId *this*))))
	      (render-object! current-sprite)))
    (advanceFrameIfNeeded! ()
    			   (if (and (animatedsprite-animationPlaying *this*)
    				    (> (MsTime) (+ (animatedsprite-lastUpdated *this*)
						   (animatedsprite-msPerFrame *this*))))
			       (-> (animatedsprite-advanceFrame! *this*)
				   (set-animatedsprite-lastUpdated! (MsTime)))
			       *this*)))))

(export-all :qmapper.animatedsprite)
