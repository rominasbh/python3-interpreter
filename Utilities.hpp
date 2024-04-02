/**
 * @file utilities.hpp
 * @brief Utility Functions and Polyfills
 * 
 * This header file provides utility functions and polyfills to support features not available in older C++ standards.
 * Specifically, it includes an implementation of std::make_unique for compilers that do not support C++14 or newer.
 * std::make_unique is a safer, more concise way to create std::unique_ptr instances compared to directly using new.
 * 
 * Usage:
 * Instead of manually managing memory with new, use std::make_unique to create unique_ptr instances.
 * 
 * Example:
 * auto myObject = std::make_unique<MyClass>(constructor, arguments);
 * 
 * Note: This polyfill is only provided if the compiler does not already support C++14 or newer (__cplusplus < 201402L).
 */
 
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>
#include <utility> 


#if __cplusplus < 201402L

namespace std {
    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif // __cplusplus < 201402L

#endif // UTILITIES_HPP
