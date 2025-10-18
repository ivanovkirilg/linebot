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

    const int warmup     = 10000;
    const int iterations = 90000;
    auto start = steady_clock::now();

    for (int i = 0; i < warmup; ++i) {
        LOGR::Trace trace{argv[0]};
    }

    for (int i = 0; i < iterations; ++i) {
                                        // count
        LOGR::Trace trace{argv[0]};     // 1
        LOGR::Warning{argv[0]};         // 2
        LOGR::Exception exc{argv[0]};   // 3
        trace.log(argv[0]);             // 4
        exc.handle(argv[0]);            // 5
        // ~Trace()                     // 6
    }
    const int count = 6;

    auto end = steady_clock::now();
    duration<double> elapsed = end - start;

    std::cout << "Total time: " << elapsed << "\n";
    std::cout << "Average time per log: " << (duration_cast<nanoseconds>(elapsed) / iterations / count) << "\n";

    return 0;
}
