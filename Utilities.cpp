/**
 * @file utilities.cpp
 * @brief Implementation of Utility Functions and Polyfills
 *
 * This source file provides the implementation details for the utility functions and polyfills defined in utilities.hpp.
 * It includes the implementation of the TeeBuffer class, which facilitates output stream duplication, and the tee function,
 * which enables output to be written simultaneously to two different streambuf objects. This setup is particularly useful for
 * logging purposes where program output needs to be echoed to both the console and a log file.
 *
 * Features:
 * - TeeBuffer class: Handles the duplication of output streams by intercepting output operations and writing the output to 
 *   two streambuf objects. It is designed to work seamlessly with standard output streams like std::cout by redirecting the 
 *   stream buffer to an instance of TeeBuffer.
 * 
 * - tee function: Serves as a utility function to facilitate the use of TeeBuffer with standard C++ output streams. It configures
 *   a given std::ostream object to use a TeeBuffer for its underlying stream buffer, enabling the duplication of output.
 *
 *
 * Implementation Details:
 * - TeeBuffer::overflow(int c): Overrides the overflow function of std::streambuf. This function is called automatically 
 *   whenever the stream buffer is full or when a character is explicitly written to the stream. It writes the character to both 
 *   of its managed streambuf objects and handles the EOF condition.
 * 
 * - TeeBuffer::sync(): Synchronizes the state of the TeeBuffer with its underlying streambuf objects, ensuring that any buffered 
 *   output is flushed to the respective outputs.
 *
 * - tee(std::ostream& strm, std::ostream& strm1, std::ostream& strm2): Sets up the given std::ostream object (strm) to use a TeeBuffer
 *   that duplicates its output to strm1 and strm2. This allows for the same output to be directed to two different targets.
 */
#include "Utilities.hpp"

TeeBuffer::TeeBuffer(std::streambuf* sb1, std::streambuf* sb2) : sb1(sb1), sb2(sb2) {}

// This function is called when output is written to the stream
int TeeBuffer::overflow(int c) {
    if (c == EOF) {
        return !EOF;
    } else {
        int const r1 = sb1->sputc(c);
        int const r2 = sb2->sputc(c);
        return r1 == EOF || r2 == EOF ? EOF : c;
    }
}

    // Sync both buffers
int TeeBuffer::sync() {
    int const r1 = sb1->pubsync();
    int const r2 = sb2->pubsync();
    return r1 == 0 && r2 == 0 ? 0 : -1;
}


std::ostream& tee(std::ostream& strm, std::ostream& strm1, std::ostream& strm2) {
    static TeeBuffer teeBuffer(strm1.rdbuf(), strm2.rdbuf());
    strm.rdbuf(&teeBuffer);
    return strm;
}
