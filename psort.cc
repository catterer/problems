#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <list>
#include <stdlib.h>
#include <time.h>

class RanVec: public std::vector<int> {
public:
    RanVec(int N) {
        reserve(N);
        for (auto i = 0; i < N; i++)
            push_back(rand() % 1000);
    }
    friend std::ostream& operator<<(std::ostream& out, const RanVec& v) {
        for (const auto& i: v)
            out << i << " ";
        return out;
    }
};

template<typename IterT>
static void psort(IterT b, IterT e) {
    const auto nthr = 8;
    auto l = e - b;
    if (l < 100) {
        std::sort(b, e);
        return;
    }

    std::list<std::future<void>> fs{};
    auto perthr = l/nthr;
    for (auto i = 0; i < nthr; i++)
        fs.push_back(
            std::async(
                [] (auto b, auto e) { std::sort(b, e); },
                b + perthr * i,
                i == nthr-1 ? e : (b + perthr * (i+1))
            ));

    for (auto&& f: fs)
        f.get();
}

int main(int argc, char** argv) {
    srand((int)time(NULL));
    if (argc < 2)
        throw std::runtime_error("arg");
    int N = strtoul(argv[1], NULL, 10);
    RanVec v{N};
    std::cout << v << std::endl;
    psort(v.begin(), v.end());
    std::cout << v << std::endl;

    return 0;
}
