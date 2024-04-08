#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "print.h"

int main() {
    char *str = get_string();
    printgraph(str);
    free(str);
    return 0;
}
