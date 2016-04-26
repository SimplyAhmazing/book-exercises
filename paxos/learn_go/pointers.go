package main

import "fmt"


func main() {
    i, j := 42, 2701

    p := &i

    fmt.Println(*p)
    *p = 21  // set i through pointer
    fmt.Println(i)

    p = &j // point to jo
    *p = *p / 37
    fmt.Println(j)
}
