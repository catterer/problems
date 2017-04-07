#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <string>
#include <string.h>

namespace Module {
    using Name = std::string;
    using Mods = std::unordered_set<Name>;
    class Order: public std::list<Mods> {
    public:
        friend std::ostream& operator<<(std::ostream& out, const Order& a);
    };

    class App: public std::unordered_map<Name, Mods> {
    public:
        App(std::istream& in);
        friend std::ostream& operator<<(std::ostream& out, const App& a);
        Order buildOrder() const;
    };

    App::App(std::istream& in) {
        std::string l{};
        while (getline(in, l)) {
            char* tok;
            char* modname;
            Mods deps{};
            for (modname = tok = strtok((char*)l.c_str(), ":"); tok; tok = strtok(NULL, " ")) {
                if (tok != modname)
                    deps.emplace(tok);
            }
            emplace(std::string(modname), std::move(deps));
        }
    }

    std::ostream& operator<<(std::ostream& out, const App& a) {
        for (const auto& p: a) {
            out << p.first << ": ";
            for (const auto& m: p.second)
                out << m << " ";
            out << std::endl;
        }
        return out;
    }

    Order App::buildOrder() const {
        Mods built{};
        Order order{};
        App left = *this;
        while (left.size()) {
            Mods nodeps{};
            for (const auto& i: left) {
                if (i.second.size())
                    continue;
                nodeps.emplace(i.first);
            }
            if (!nodeps.size())
                throw std::runtime_error("Unmet dependencies!");

            for (const auto& nodep: nodeps)
                left.erase(nodep);

            for (auto& i: left)
                for (const auto& nodep: nodeps)
                    i.second.erase(nodep);

            order.emplace_back(std::move(nodeps));
        }
        return std::move(order);
    }

    std::ostream& operator<<(std::ostream& out, const Order& o) {
        for (const auto& s: o) {
            for (const auto& m: s)
                out << m << " ";
            out << std::endl;
        }
        return out;
    }
}

int main() {
    Module::App app{std::cin};
    auto o = app.buildOrder();
    std::cout << o << std::endl;
    return 0;
}
