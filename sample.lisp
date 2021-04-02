(+ (* 3 5) (* 47 (- 20 6.8))) 12)
;                           ~~~~~
(+ (* 3 5)
   (* 47
      (- 20 6.8))
   12)

(defun fib (n)
  (loop for f1 = 0 then f2
        and f2 = 1 then (+ f1 f2)
        repeat n finally (return f1))))
;                                     ~
(defparameter num 10)
(fib num)
(let ((n 40)) (fib n))

(defun revapp (x y)
  (if (null x) y
      (revapp
        (cdr x)
        (cons (car x) y)))))
;                          ~
(revapp (revapp '(a b c) nil) '(x y z))

