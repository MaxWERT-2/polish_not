#include "print.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "func.h"

char *get_string() {
    char *s = (char *)malloc(sizeof(char));
    if (s != NULL) {
        int len = 0;
        int capacity = 1;
        char c = getchar();
        while (c != '\n') {
            s[len++] = c;
            if (len >= capacity) {
                capacity *= 2;
                char *sup = (char *)realloc(s, capacity * sizeof(char));
                if (sup == NULL) {
                    free(s);
                } else {{
                    s = sup;
                }}
            }
            c = getchar();
        }
        s[len] = '\0';
    }
    return s;
}

void printgraph(char *str) {
    double value_y = -1.0;
    double step_y = 1.0 / 12.0;  // 1 / 12
    double step_x = 4.0 * M_PI / 79.0;

    for (int y = 0; y < 25; y++) {
        double value_x = 0;
        for (int x = 0; x < 80; x++) {
            int mode = 1;
            double f_x = func(str, &mode, value_x);
            if (mode == 1) {
                if ((2 * value_y + step_y) / 2 > f_x && (2 * value_y - step_y) / 2 <= f_x) {
                    printf("*");
                } else {
                    printf(".");
                }
            } else {
                printf(".");
            }
            value_x += step_x;
        }
        value_y += step_y;
        printf("\n");
    }
}
