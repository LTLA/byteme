#ifndef BYTEME_CHECK_BUFFER_SIZE_HPP
#define BYTEME_CHECK_BUFFER_SIZE_HPP

#include <vector>
#include <limits>

namespace byteme {

template<typename Type_>
constexpr typename std::make_unsigned<Type_>::type unsigned_max() {
    return std::numeric_limits<Type_>::max();
}

template<typename Cap_>
bool exceeds_cap(std::size_t buffer_size) {
    return buffer_size > unsigned_max<Cap_>();
}

template<typename Cap_>
std::size_t check_buffer_size(std::size_t buffer_size) {
    if (exceeds_cap<Cap_>(buffer_size)) {
        return unsigned_max<Cap_>();
    } else {
        return buffer_size;
    }
}

inline std::size_t check_buffer_size(std::size_t buffer_size) {
    // Usually this is a no-op as the size_type is a size_t.
    // But it doesn't hurt to confirm that is the case.
    return check_buffer_size<typename std::vector<unsigned char>::size_type>(buffer_size);
}

template<typename Cap_, bool non_zero_, typename Buffer_, typename BufSize_, class Func_>
void safe_write(const Buffer_* buffer, BufSize_ n, Func_ fun) {
    constexpr auto total = unsigned_max<Cap_>();
    while (n > total) {
        fun(buffer, total);
        n -= total;
        buffer += total;
    }

    if constexpr(non_zero_) {
        if (n == 0) { // not sure if we need this, but better safe than sorry.
            return;
        }
    }

    fun(buffer, n);
}

}

#endif
