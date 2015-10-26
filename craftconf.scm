(define-syntax loopvar
  (syntax-rules ()
		((_ var a b exp ...) (let ((inc (if (> a b) - +)))
					     (do ((var a (inc var 1))) ((= var b) exp ...) exp ...)))))

(define (display-message thing)
  (add-message
    (let ((output-port (open-output-string)))
      (display thing output-port)
      (get-output-string output-port))))

(define (print . stuff)
  (map display-message stuff)
  #t)
		  

(define (repl)
  (let loop ((input (read)))
    (protect (exn (else (print-exception exn))) (eval input))
    (loop (read))))

(repl)
