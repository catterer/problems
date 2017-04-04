#include <iostream>
#include <memory>
#include <list>

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
        for (size_t i = 0; i < off; i++)
            out << "  ";
        out << val;
        out << std::endl;
        if (r)
            r->depthPrint(out, off+1);
        if (l)
            l->depthPrint(out, off+1);
    }
    void horPrint(std::ostream& out, size_t off = 0) const {
        std::list<const Node<T>*> que{};
        que.push_back(this);
        while (!que.empty()) {
            const auto& curs = **que.begin();
            out << curs.val << " ";
            if (curs.r)
                que.push_back(&(*curs.r));
            if (curs.l)
                que.push_back(&(*curs.l));
            que.pop_front();
        }
        out << std::endl;
    }
};


int main() {
    std::unique_ptr<Node<unsigned long>> root;
    std::string l{};
    for (std::getline(std::cin, l); l != "q"; std::getline(std::cin, l)) {
        auto d = strtoul(l.c_str(), NULL, 10);
        if (root)
            root->insert(d);
        else
            root = std::make_unique<Node<unsigned long>>(d);
    }
    root->depthPrint(std::cout);
    std::cout << std::endl;
    root->horPrint(std::cout);
}
