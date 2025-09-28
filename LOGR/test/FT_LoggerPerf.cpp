#include "LOGR/ILogger.hpp"
#include "LOGR/Exception.hpp"
#include "LOGR/Trace.hpp"
#include "LOGR/Warning.hpp"

#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    using namespace std::chrono;
    auto logger = LOGR::ILogger::create("PERF");

    const int iterations = 100000;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        LOGR::Trace{argv[0]};
    }

    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;

    std::cout << "Total time: " << elapsed << "\n";
    std::cout << "Average time per log: " << (duration_cast<nanoseconds>(elapsed) / iterations) << "\n";

    return 0;
}
