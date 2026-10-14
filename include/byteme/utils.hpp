#ifndef BYTEME_UTILS_HPP
#define BYTEME_UTILS_HPP

#include <vector>
#include <limits>
#include <type_traits>
#include <cstddef>
#include <exception>

#include "sanisizer/sanisizer.hpp"

namespace byteme {

template<typename Value_>
using I = std::remove_cv_t<std::remove_reference_t<Value_> >;

template<typename Cap_, class Func_>
std::size_t safe_read(unsigned char* buffer, std::size_t n, Func_ fun) {
    std::size_t remaining = n;
    constexpr auto total = std::numeric_limits<Cap_>::max();

    while (sanisizer::is_greater_than(remaining, total)) {
        const auto read = fun(buffer, total);
        static_assert(std::is_same<I<decltype(read)>, Cap_>::value);

        remaining -= read;
        buffer += read;
        if (read < total) {
            return n - remaining; 
        }
    }

    auto extra = fun(buffer, remaining);
    remaining -= extra;
    return n - remaining;
}

template<typename Cap_, bool non_zero_, class Func_>
void safe_write(const unsigned char* buffer, std::size_t n, Func_ fun) {
    constexpr auto total = std::numeric_limits<Cap_>::max();

    while (sanisizer::is_greater_than(n, total)) {
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

template<typename Overall_, typename Value_>
void safe_update_position(Overall_& overall, Value_ value, std::exception_ptr& ex) {
    try {
        overall = sanisizer::sum<Overall_>(overall, value);
    } catch (...) {
        ex = std::current_exception();
    }
}

template<typename Overall_, typename Value_>
Overall_ safe_report_position(Overall_ overall, Value_ value, const std::exception_ptr& ex) {
    if (ex) {
        std::rethrow_exception(ex);
    }
    return sanisizer::sum<Overall_>(overall, value);
}

}

#endif
