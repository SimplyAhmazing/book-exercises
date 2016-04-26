#lang racket
(require (prefix-in srfi: srfi/63))
(require racket/gui)

(define (make-board)
  (srfi:make-array '#() 10 10))

(define (set-initial-board-state board)
  (srfi:array-set! board 1 6 6)
  (srfi:array-set! board 1 6 7)
  (srfi:array-set! board 1 6 8)
  (srfi:array-set! board 1 5 8)
  (srfi:array-set! board 1 4 7)
  board)

(define (main)
  (let ([board (make-board)])
    (set-initial-board-state board)
    (print board)))



;;; GRAPHICS



(define frame (new frame%
                   [label "Example"]
                   [width 300]
                   [height 300]))
(new canvas% [parent frame]
             [paint-callback
              (lambda (canvas dc)
                ;(make-brush brush)
                ;(send dc draw-rectangle 0 0 10 10)
                (send dc draw-polygon (cons 0 (cons 0 empty)))
                )])
(send frame show #t)