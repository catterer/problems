#include <thread>
#include <condition_variable>
#include <vector>
#include <functional>
#include <queue>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_set>
#include <iomanip>
#include <inttypes.h>
#include <cstring>

template<typename T>
class Vector {
public:
    Vector() = default;

    template<typename P>
    std::enable_if_t<std::is_same<P, T>::value> push_back(P&& t) {
        if (size_ == allocd_) {
            allocd_ += std::min((size_t)8, allocd_ / 8);
            T* new_data = new T[allocd_];
            try {
                new_data[size_] = std::forward<P>(t);

                for (size_t i = 0; i < size_; ++i)
                    new_data[i] = std::move(data_[i]);
            } catch(...) {
                delete[] new_data;
                throw;
            }
            delete[] data_;
            data_ = new_data;
            ++size_;
        } else {
            data_[size_] = std::forward<P>(t);
            ++size_;
        }
    }

    T& operator[](size_t idx) {
        return data_[idx];
    }

    size_t size() const { return size_; }

    ~Vector() {
        delete[] data_;
    }
private:

    T* data_ = nullptr;
    size_t size_ = 0;
    size_t allocd_ = 0;
};

struct S {
    S() = default;
    S& operator=(S&&) {
        if (rand() % 10 == 0)
            throw std::runtime_error("assign!");
        return *this;
    }
};

int main() {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    for (size_t i = 0 ; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";

    Vector<S> vs;
    try {
        for (int i = 0; i < 100; ++i)
            vs.push_back(S{});
    } catch (const std::exception& x) {
        std::cout << x.what() << "\n";
    }
    return 0;
}
