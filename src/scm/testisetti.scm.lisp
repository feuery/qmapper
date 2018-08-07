(add-event "1469834481" "Sprite" "X" (lambda (id)
				       (let ((x (get-prop id "Sprite" "x")))
					 (if (> x 400)
					     (scm-puts "Voitit pelin")
					     (begin
					       (scm-puts "Lol äxä on: ")
					       (scm-puts (object->string x)))))))


(qloop (lambda ()
	 (when (key-down? 'Key_Right)
	   (let ((x (get-prop "2001100545" "Sprite" "x")))
	     (set-prop "2001100545" "Sprite" "x" (+ x 10))))))
