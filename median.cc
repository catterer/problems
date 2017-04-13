#include <iostream>
#include <set>
#include <algorithm>
#include <cassert>
using namespace std;

int readnum() {
    std::string l;
    std::getline(std::cin, l);
    return strtol(l.c_str(), NULL, 10);
}

int main() {
    std::multiset<int> l{}, r{};
    
    auto N = readnum();
    for (auto i = 0; i < N; i++) {
        auto x = readnum();
        
        if (!l.size() && !r.size()) {
            r.emplace(x);
        } else {
            assert(r.size());
            if (x < *r.begin())
                l.emplace(x);
            else
                r.emplace(x);
        }
        
        if (l.size()+1 < r.size()) {
            l.emplace_hint(l.end(), *r.begin());
            r.erase(r.begin());
        } else if (l.size() > r.size()+1) {
            r.emplace_hint(r.begin(), *l.rbegin());
            auto e = l.end();
            e--;
            l.erase(e);
        }
        
        int median;
        if (l.size() > r.size())
            median = *l.rbegin();
        else if (r.size() > l.size())
            median = *r.begin();
        else
            median = (*l.rbegin() + *r.begin())/2;
        std::cout << median << std::endl;
    }
    return 0;
}
