#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

bool regex_chk(char* s, char* r) {
    printf("'%s'\t'%s'\n", s, r);
    if (!*s)
        return !*r || !strcmp(r, ".*");
    if (!*r)
        return *s == 0;
    if (*r == '.') {
        if (*(r+1) != '*')
            return regex_chk(s+1, r+1);

        bool res = false;
        char* i;
        for (i = s; *i; i++)
            res = res || regex_chk(i, r+2);
        return res || regex_chk(i, r+2);
    } else {
        if (*(r+1) != '*')
            return (*s == *r) && regex_chk(s+1, r+1);

        bool res = false;
        char* i;
        for (i = s; *i == *s; i++)
            res = res || regex_chk(i, r+2);
        return res || regex_chk(i, r+2);
    }
}

int main(int argc, char** argv) {
    if (argc != 3)
        return -1;
    printf("%d\n", regex_chk(argv[1], argv[2]));
    return 0;
}
