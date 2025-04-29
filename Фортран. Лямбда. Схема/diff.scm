;'(+ x 3)
;(D '(+ x 3)) 
;=> 1
;'(* 2 (+ x 3))
;D(..) => 2

(use-modules (ice-9 match))

(define (diff expr var)
  (let ((D (lambda (e) (diff e var))))
  (match expr
		 (('+ exp1 exp2) `(+ ,(D exp1) ,(D exp2)))
		 (('- exp1 exp2) `(- ,(D exp1) ,(D exp2)))
		 (('* exp1 exp2) `(+ (* ,(D exp1) ,exp2) (* ,(D exp2) ,exp1)))
		 (('/ exp1 exp2) `(/ (- (* ,(D exp1) ,exp2) (* ,(D exp2) exp1)) (* ,exp2 ,exp2)))
		 ((? number?  x) 0)
		 (var 1)
		 ((? symbol? v) 1))))

(define (fold-const expr var)
  (let* ((F (lambda (e) (fold-const e var)))
		 (res (match expr 
		 		((binop exp1 exp2) `(,binop ,(F exp1) ,(F exp2)))
				(('- exp1) `(- ,(F exp1)))
				((? number? x) x)
				(var var))))
	(match res
		   (('+ 0 exp1) exp1)
		   (('+ exp1 0) exp1)
		   (('- exp1 0) exp1)
		   (('- 0 exp1) `(- ,exp1))
		   (('* 1 exp1) exp1)
		   (('* exp1 1) exp1)
		   (('/ exp1 1) exp1)
		   (('* exp1 0) 0)
		   (('* 0 exp1) 0)
		   (x x))))

; x * (3 * (x+9)) 
(display (fold-const (diff '(* x (* 3 (+ x 9)))  'x) 'x))
(newline)

(display (match 3 ((? number? x) 'ok) ('x 'isX') (else 'false)))
  (newline)




