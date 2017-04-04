#include <iostream>
#include <memory>

template<typename T>
class Node {
private:
    size_t nch{};
    T   val;
    std::unique_ptr<Node> l{};
    std::unique_ptr<Node> r{};

    void branch_insert(std::unique_ptr<Node>& br, const T& nv) {
        if (br)
            return br->insert(nv);
        br = std::make_unique<Node>(nv);
    }

public:
    Node(const T& val): val{val} {}

    void insert(const T& nv) {
        if (nv == val)
            return;
        if (nv > val)
            return branch_insert(r, nv);
        else
            return branch_insert(l, nv);
    }

    void depthPrint(std::ostream& out, size_t off = 0) const {
        for (auto i = 0; i < off; i++)
            out << "  ";
        out << val;
        if (r)
            r->depthPrint(out, off+1);
        if (l)
            l->depthPrint(out, off+1);
    }
};


int main() {
    Node root{};
    std::string l{};
    for (std::string l{}; l != "q"; std::getline(std::cin, l)) {
        auto d = strtoul(l.c_str(), NULL, 10);
        root.insert(d);
    }
}
