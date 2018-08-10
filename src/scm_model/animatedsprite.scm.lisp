(defpackage :qmapper.animatedsprite
  (:use :common-lisp
	:cl-arrows
	:qmapper.std
	:qmapper.root))

(in-package :qmapper.animatedsprite)

(defcppclass animatedsprite
  (public
   (properties
    (std__string parentMapId "")
    (std__string name "")
    (int currentFrameId 0)
    (int msPerFrame 25)
    (bool animationPlaying true)
    (int lastUpdated 0)

    (int x 0)
    (int y 0)
    (float angle 0.0)
    
    (std__vector<Sprite*>* sprites '()))
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
