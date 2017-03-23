#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2)
        exit(EXIT_FAILURE);
    char* s = strdup(argv[1]);
    char* rdr;
    char* wrr = s;
    for (rdr = s; *rdr; rdr++) {
        char* cur;
        int first = 1;
        for (cur = s; cur != wrr; cur++) {
            if (*cur == *rdr) {
                first = 0;
                break;
            }
        }
        if (first) {
            *wrr = *rdr;
            wrr++;
        }
    }
    *wrr = 0;
    printf("%s\n", s);
    free(s);
    return 0;
}

