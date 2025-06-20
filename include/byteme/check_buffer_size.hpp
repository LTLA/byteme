#ifndef BYTEME_CHECK_BUFFER_SIZE_HPP
#define BYTEME_CHECK_BUFFER_SIZE_HPP

#include <vector>
#include <limits>
#include <type_traits>
#include <cstddef>

/**
 * @file check_buffer_size.hpp
 * @brief Utilities to check the buffer sizes.
 */

namespace byteme {

/**
 * @cond
 */
template<typename Type_>
constexpr typename std::make_unsigned<Type_>::type unsigned_max() {
    return std::numeric_limits<Type_>::max();
}

template<typename Cap_>
bool exceeds_cap(std::size_t buffer_size) {
    constexpr auto cap = unsigned_max<Cap_>();
    if constexpr(std::numeric_limits<std::size_t>::max() > cap) {
        return buffer_size > cap;
    } else {
        return false;
    }
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
/**
 * @endcond
 */

/**
 * Cap an input integer at the largest value that can be represented by a specified output type, typically a `std::size_t`.
 * This avoids silent integer overflows, especially in the defaults of the various `*Options` classes.
 *
 * @tparam Output_ Unsigned integer type of the output.
 * @tparam Input_ Integer type of the input.
 *
 * @param size Non-negative integer specifying some kind of buffer size.
 *
 * @return `size` if it fits in an `Output_`, otherwise the largest value of an `Output_`.
 */
template<typename Output_, typename Size_>
constexpr Output_ cap(Size_ size) {
    constexpr auto cap = std::numeric_limits<Output_>::max();
    if (static_cast<typename std::make_unsigned<Size_>::type>(size) > cap) {
        return cap;
    } else {
        return size;
    }
}

}

#endif
