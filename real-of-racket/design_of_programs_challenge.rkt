#lang racket

; width of river
; speed of boat perpindicular to the river
; speed of river
; c^2 = b^2  + a^2

; boat_velocity = boat_speed_x & boat_speed_y
; boat velocity
; pi rad = 180 deg
; deg_num / 180 / pi = degrees

(define (dist-traveled d_w v_r v_b)
  (cond [(<= v_r 0) d_w]
        [(<= v_b 0) 0]
        [#t (/ d_w (sin (atan (/ v_r v_b))))]))