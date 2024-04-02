/**
 * @file utilities.hpp
 * @brief Utility Functions and Polyfills
 * 
 * This header file provides utility functions and polyfills to support features not available in older C++ standards, 
 * enhancing compatibility and adding convenient functionalities for modern C++ development practices.
 * 
 * Features:
 * - std::make_unique polyfill: Provides an implementation of std::make_unique for compilers that do not support C++14 or newer. 
 *   This function offers a safer, more concise way to create std::unique_ptr instances, avoiding direct use of new.
 * 
 * - TeeBuffer and tee function: Implements output stream duplication, allowing output to be written simultaneously 
 *   to two streambuf objects. This is particularly useful for logging purposes, where output needs to be echoed to both 
 *   the console and a log file.
 * 
 * Usage:
 * - For creating unique_ptr instances:
 *   auto myObject = std::make_unique<MyClass>(constructor_arguments...);
 * 
 * - For duplicating output streams:
 *   std::ofstream logFile("log.txt");
 *   tee(std::cout, std::cout, logFile); // Echoes output to console and logFile
 * 
 * Note:
 * - The std::make_unique polyfill is only provided if the compiler does not already support C++14 or newer (__cplusplus < 201402L).
 * - The TeeBuffer and tee function can be utilized regardless of the C++ standard version.
 */
 
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>
#include <utility> 
#include <streambuf>
#include <iostream>
#include <fstream>

// Polyfill for std::make_unique in C++11

#if __cplusplus < 201402L

namespace std {
    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}


// TeeBuffer: Duplicates output stream to two streambufs
class TeeBuffer : public std::streambuf {
public:
    TeeBuffer(std::streambuf* sb1, std::streambuf* sb2);
protected:
    virtual int overflow(int c) override;
    virtual int sync() override;
private:
    std::streambuf* sb1; // First streambuf
    std::streambuf* sb2; // Second streambuf

};


// tee: Utility function to connect an ostream to a TeeBuffer// tee: Utility function to connect an ostream to a TeeBuffer
std::ostream& tee(std::ostream& strm, std::ostream& strm1, std::ostream& strm2);


#endif // __cplusplus < 201402L

#endif // UTILITIES_HPP
