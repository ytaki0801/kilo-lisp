(+ (* 3 5) (* 47 (- 20 6.8))) 12)
;                           ~~~~~
(+ (* 3 5)
   (* 47
      (- 20 6.8))
   12)

(define (fib n)
  (let loop ((n n) (f1 0) (f2 1))
    (if (zero? n) f1
        (loop (- n 1) f2 (+ f1 f2))))))
;                                     ~
(define num 10)
(fib num)
(let ((n 40)) (fib n))

(define (revapp x y)
  (if (null? x) y
      (revapp
        (cdr x)
        (cons (car x) y)))))
;                          ~
(revapp
  (revapp '(a b c) '())
  '(x y z))

