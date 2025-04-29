(use-modules (ice-9 match)
			 (ice-9 format))
(define (tree-insert t x)
  (match t
		 ('() `(() ,x () 0)) ; if tree is null
		 ((l y r h)
		  (if (< x y) (BT (tree-insert l x) y r)
					(BT l y (tree-insert r x))))))

(define (H t)
  (match t 
		 ('() 0)
		 ((l y r h) h)))



(define (BT l x r)
  (let ((lh (H l))
		(rh (H r)))
	(cond
	  ((= lh (+ rh 2))
	  		(match l
				((ll lx rl hl) 
			  		(if (> (H ll) (H rl)) (BT ll lx (BT rl x r))
						(match rl
					   		((lrl rlx rrl hrl) (BT (BT ll lx lrl) rlx (BT rrl x r))))))))
		((= rh (+ lh 2))
			(match r
				((lr rx rr hr)
				 	(if (> (H rr) (H lr)) (BT (BT l x lr) rx rr)
				 		(match lr
							((llr lrx rlr hlr) (BT (BT l x llr) lrx (BT rlr rx rr))))))))
		(else 
		  	`(,l ,x ,r ,(+ 1 (max lh rh)))))))


(define (print-tree t level) 
  (match t
		 ('() '())
		 ((l x r h)
		  	(print-tree l (+ 1 level))
			(format #t (format #f "~~~d/ ~~a (~~d) ~~%" level)  x (- (H l) (H r)))
			(print-tree r (+ 1 level)))))


(define tree '())
(set! tree (tree-insert tree 10))
(set! tree (tree-insert tree 1))
(set! tree (tree-insert tree 20))
(set! tree (tree-insert tree 30))
(set! tree (tree-insert tree 4))
(set! tree (tree-insert tree 12))

(define (make-tree n)
  (if (= n 0) '()
	(begin (set! tree (tree-insert tree n))
		   (make-tree (- n 1)))))

(make-tree 1000000)
(format #t "~d" (H tree))
;;(display tree)
;;(newline)
;;(print-tree tree 0)
(newline)

