(define fib (lambda  (n) 
			  (letrec ((fib' (lambda (a b k)
				(if (<  k n)
				  (fib' (+ a b)  a (+ k 1)) a))))
				; body
				(fib' 1 1 0))))


(display (fib 10))


(define (account initial-balance)
  (let ((balance initial-balance))
  	(cons 
	  	(lambda (withdraw) 
		  (if (< balance withdraw)
			(begin (display "cannot withdraw~n") balance)
			(begin (set! balance (- balance withdraw))
				   balance)))
		(lambda (deposit)
		  (set! balance (+ balance deposit))
		  balance))))


(define a1 (account 10))
(define a2 (account 0))
(display "Doing withdraw from a1\n")
(display ((car a1) 20))
(newline)
(display "doing deposit to a1\n")
(format #t "~a~%" ((cdr a1) 100))
(newline)
(display "doing withdraw from a1\n")
(format #t "~a~%" ((car a1) 80))


(display (map (lambda (x) (* x x)) '(1 2 3 4 5 6 7 8 10)))

(define (my-map f lst) 
  (cond ((null? lst) '())
		(else (cons (f (car lst)) (my-map f (cdr lst))))))

(newline)

(display (my-map (lambda (x) (* x x)) (iota 10)))
(newline)

(define lst (iota 100))

(display (list-ref lst 21))
(newline)



(format #t "~a = ~a ~%" (list-ref lst 3) (cadddr lst))


(newline)

				

				
