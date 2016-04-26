#include <stdio.h>

#define PRINT_TOKEN(token) printf(#token " is %d ", token)
#define SWAP(a, b) { a ^= b; b ^= a; a ^= b; }

int main(int argc, char** argv) {
    int x = 1, y = 2;

    PRINT_TOKEN(2);
    SWAP(x, y);

    printf("\n\n(1,2) swapped is (%d,%d)\n", x, y);
    return 0;
}
