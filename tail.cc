#include <iostream>
#include <fstream>
#include <string>
#include <list>

int main(int argc, char** argv) {
    if (argc < 3)
        return EXIT_FAILURE;
    size_t nstrs = (size_t)atoi(argv[2]);
    std::list<std::string> res{};

    std::ifstream f;
    f.open(argv[1]);
    while (!f.eof()) {
        std::string l;
        std::getline(f, l);
        res.push_back(l);
        if (res.size() > nstrs)
            res.pop_front();
    }

    for (const auto& i: res)
        std::cout << i << std::endl;
    return 0;
}
