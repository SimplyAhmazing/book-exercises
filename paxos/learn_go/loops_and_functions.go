package main

import (
    "fmt"
    "math"
)

func Sqrt(x float64) float64 {
    z := float64(x)

    z_delta := 1.0

    for z_delta > .1e-5 {
        z_prev := z
        z = z - (math.Pow(z, 2) - float64(x))/ (2 * z)
        z_delta = z_prev - z
    }
    return z
}

func main() {
    fmt.Println(Sqrt(4))
}
