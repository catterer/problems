#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <assert.h>

using SL = std::list<size_t>;
class BrStack: SL {
public:
    const char sym_op{};
    const char sym_cl{};

    using SL::size;
    BrStack(const char sym_op, const char sym_cl): SL() {};
    void open(size_t pos) {
        push_back(pos);
    }
    int close() {
        if (empty())
            return -1;
        pop_back();
        return 0;
    }
    ssize_t last() const {
        if (empty())
            return -1;
        return *rbegin();
    }
};

using StackMap = std::map<char, std::shared_ptr<BrStack>>;
struct BrParser {
    StackMap    op{};
    StackMap    cl{};

    void add(const char sym_op, const char sym_cl) {
        assert(!op.count(sym_op));
        assert(!op.count(sym_cl));
        assert(!cl.count(sym_op));
        assert(!cl.count(sym_cl));

        auto br = std::make_shared<BrStack>(sym_op, sym_cl);
        op.emplace(sym_op, br);
        cl.emplace(sym_cl, br);
    }
    bool all_closed() const {
        for (const auto& i: op)
            if (i.second->size())
                return false;
        return true;
    }
};

using BrStackPair = std::pair<char, std::shared_ptr<BrStack>>;

static int check(const std::string& in) {
    BrParser pr{};
    pr.add('(', ')');
    pr.add('{', '}');
    pr.add('<', '>');
    pr.add('[', ']');

    for (auto i = in.begin(); i != in.end(); i++) {
        if (pr.op.count(*i))
            pr.op[*i]->open(i - in.begin());
        else if (pr.cl.count(*i)) {
            auto mi = pr.cl.find(*i);
            if (mi !=
                    std::max_element(pr.cl.begin(), pr.cl.end(),
                        [](const BrStackPair& a, const BrStackPair& b) {
                            return a.second->last() < b.second->last();
                        }))
            {
                std::cerr << "mix braces" << std::endl;
                return -1;
            }
            if (mi->second->close()) {
                std::cerr << "unopened" << std::endl;
                return -1;
            }
        }
    }
    if (!pr.all_closed()) {
        std::cerr << "unclosed" << std::endl;
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 2)
        return -1;
    std::string in{argv[1]};
    if (check(in))
        std::cout << "Fail" << std::endl;
    else
        std::cout << "OK" << std::endl;
    return 0;
}
