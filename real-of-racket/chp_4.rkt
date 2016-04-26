#lang racket


(struct posn (x y))
(struct rectangle (width height))
(define (inside-of-rectangle? r p)
  (define x (posn-x p))
  (define y (posn-y p))
  (define width (rectangle-width r))
  (define height (rectangle-height r))
  (and (<= 0 x) (< x width) (<= 0 y) (<= y height)))


(define myp (posn 3 3))
(define rect (rectangle 4 4))
(inside-of-rectangle? rect myp)