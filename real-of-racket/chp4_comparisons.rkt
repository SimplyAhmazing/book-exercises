#lang racket

(require rackunit)  ; used for testing


(struct point (x y) #:transparent)

(define (distance-to-origin p)
  (sqrt (+ (sqr (point-x p)) (sqr (point-y p)))))

(check-eq? (add1 5) 5)

(check-equal? 5 6 "Numbers matter!!")