#include <iostream>
#include <string>
#include <memory>

static void sasha(char* accum, char* pos, int nb, int opened) {
    if (!nb) {
        *pos = 0;
        std::cout << accum << std::endl;
        return;
    }
    if (nb > opened) {
        *pos = '(';
        sasha(accum, pos+1, nb-1, opened+1);
    }
    if (opened) {
        *pos = ')';
        sasha(accum, pos+1, nb-1, opened-1);
    }
}

int main(int argc, char** argv) {
    if (argc != 2)
        exit(EXIT_FAILURE);
    int ns = atoi(argv[1]);
    ns -= ns % 2;
    auto accum = std::shared_ptr<char>((char*)malloc(ns + 1), [] (char* p) { free(p); } );

    for (int i = 0; i < ns; i += 2)
        sasha(accum.get(), accum.get(), i+2, 0);
    return 0;
}
