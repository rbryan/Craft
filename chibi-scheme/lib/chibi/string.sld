
;;> A cursor-oriented string library.  Provides efficient string
;;> utilities for implementations with or without fast random-access
;;> strings.

(define-library (chibi string)
  (export
   string-cursor-start string-cursor-end string-cursor-ref
   string-cursor<? string-cursor<=? string-cursor>? string-cursor>=?
   string-cursor=? string-cursor-next string-cursor-prev substring-cursor
   string-null? string-every string-any
   string-join string-split string-count
   string-trim string-trim-left string-trim-right
   string-mismatch string-mismatch-right
   string-prefix? string-suffix?
   string-find string-find-right string-find? string-skip string-skip-right
   string-fold string-fold-right string-map string-for-each
   string-contains make-string-searcher
   string-downcase-ascii string-upcase-ascii)
  (cond-expand
   (chibi
    (import (chibi) (chibi ast) (chibi char-set base))
    (begin
      (define (string-for-each proc str . los)
        (if (null? los)
            (string-fold (lambda (ch a) (proc ch)) #f str)
            (let ((los (cons str los)))
              (let lp ((is (map string-cursor-start los)))
                (cond
                 ((any (lambda (str i)
                         (string-cursor>=? i (string-cursor-end str)))
                       los is))
                 (else
                  (apply proc (map string-cursor-ref los is))
                  (lp (map string-cursor-next los is))))))))
      (define (string-map proc str . los)
        (call-with-output-string
          (lambda (out)
            (apply string-for-each
                   (lambda args (write-char (apply proc args) out))
                   str los))))))
   (else
    (import (scheme base) (scheme char) (srfi 14)
            (except (srfi 1) make-list list-copy))
    (begin
      (define string-cursor<? <)
      (define string-cursor>? >)
      (define string-cursor=? =)
      (define string-cursor<=? <=)
      (define string-cursor>=? >=)
      (define string-cursor-ref string-ref)
      (define (string-cursor-start s) 0)
      (define string-cursor-end string-length)
      (define (string-cursor-next s i) (+ i 1))
      (define (string-cursor-prev s i) (- i 1))
      (define (substring-cursor s start . o)
        (substring s start (if (pair? o) (car o) (string-length s))))
      (define (string-concatenate ls) (apply string-append ls))
      (define string-size string-length)
      (define (call-with-output-string proc)
        (let ((out (open-output-string)))
          (proc out)
          (get-output-string out))))))
  (cond-expand
   (chibi)
   ((library (srfi 13))
    (import (only (srfi 13) string-contains)))
   (else
    (begin
      (define (string-contains a b)
       (let ((alen (string-length a))
             (blen (string-length b)))
         (let lp ((i 0))
           (and (<= (+ i blen) alen)
                (if (string=? b (substring a i (+ i blen)))
                    i
                    (lp (+ i 1))))))))))
  (include "string.scm"))
