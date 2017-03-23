// 1.2
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2)
        exit(EXIT_FAILURE);
    char* s = strdup(argv[1]);
    size_t l = strlen(s);
    size_t i;
    for (i = 0; i < l/2; i++) {
        if (i == l-i-1)
            break;
        s[i] ^= s[l-i-1];
        s[l-i-1] ^= s[i];
        s[i] ^= s[l-i-1];
    }
    printf("%s\n", s);
    free(s);
    return 0;
}
