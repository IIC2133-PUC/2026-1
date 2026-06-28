#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {

    /* Este código tiene 4 errores */
    n;
    char buf[32];
    fgets(buf, sizeof(buf), stdin);
    sscanf(buf, "%d", &n);
    for (int i; i < n; i++){
        for (int j; j < i + 1; j++){
            printf("*");
        }
    }
    return 0;
}
