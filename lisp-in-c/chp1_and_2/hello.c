#include <stdio.h>


int add_together(int x, int y) {
    int result = x + y;
    return result;
}


typedef struct {
    float x;
    float y;
} point;


int main(int argc, char** argv) {
    int num = add_together(1,1);

    point p;
    p.x = add_together(1,1);
    p.y = add_together(1,2);

    printf("%d\n", (int) p.x);
    printf("%d\n", (int) p.y);
    puts("Hello, World!");

    int tot = 0;
    for (int i = 0; i < 10; i++) {
        tot += add_together(i, tot);
        printf("The total at %d is %d\n", i, tot);
    }
    return 0;
}
