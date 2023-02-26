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
std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, ssize_t> writeOne(char* out, size_t out_len, T t) {
    if (out_len < sizeof(t))
        return -1;
    *reinterpret_cast<T*>(out) = t;
    return sizeof(t);
}

ssize_t writeOne(char* out, size_t out_len, std::string_view sv) {
    uint64_t l = sv.size();
    if (out_len < sv.size() + sizeof(l))
        return -1;
    ssize_t len_len = writeOne(out, out_len, l);
    if (len_len < 0)
        return -1;
    out += len_len;
    out_len -= len_len;
    memcpy(out, (void*)sv.data(), sv.size());
    return len_len + sv.size();
}


template<typename T, typename ...Args>
const char* write(char* out, size_t out_len, const T& t, Args&& ...args) {
    ssize_t written_bytes = writeOne(out, out_len, t);
    if (written_bytes < 0)
        return nullptr;
    out += written_bytes;
    out_len -= written_bytes;
    if constexpr (sizeof...(args) != 0)
        return write(out, out_len, std::forward<Args>(args)...);
    return out;
}

int main() {
    char s[1024];
    const char* endp = write(s, sizeof(s), (uint32_t)5, (uint32_t)6, 15.5, "asd");
    if (!endp)
        std::cout << "not enough space\n";
    else {
        for (char* p = s; p != endp; ++p) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)*p << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
