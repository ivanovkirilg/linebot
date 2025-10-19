#include "SYNC/Tick.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <numeric>

using namespace std::chrono;
using namespace SYNC;

class TestTick : public testing::Test
{
public:
    static constexpr size_t NR_TICKS = 50;
    static constexpr auto TICK_TIME = 8ms;

    Tick tick{TICK_TIME};
};

TEST_F(TestTick, takesExactTime)
{
    constexpr auto UPPER_LIMIT = Tick::MAX_DRIFT * NR_TICKS;

    // When
    auto start = steady_clock::now();
    for (size_t i = 0; i < NR_TICKS; i++)
    {
        tick();
    }
    auto end = steady_clock::now();

    // Then
    auto total = duration_cast<milliseconds>(end - start);

    EXPECT_GT(total, (NR_TICKS * TICK_TIME));
    EXPECT_LT(total, (NR_TICKS * TICK_TIME) + UPPER_LIMIT);
}

TEST_F(TestTick, reportsAccurateDeltaTime)
{
    constexpr auto MAX_DEVIATION_MS = NR_TICKS * TICK_TIME * 0.05;

    // Given
    std::vector<Tick::clock::duration> reportedDeltas;
    reportedDeltas.reserve(NR_TICKS);

    // When
    auto start = steady_clock::now();
    for (size_t i = 0; i < NR_TICKS; i++)
    {
        tick();
        reportedDeltas.push_back(tick.deltaTime());
    }
    auto end = steady_clock::now();

    // Then
    auto total = duration_cast<milliseconds>(end - start);
    auto reportedSum = duration_cast<milliseconds>(
        std::accumulate(reportedDeltas.begin(), reportedDeltas.end(), 0ns)
    );

    EXPECT_NEAR(reportedSum.count(), total.count(), MAX_DEVIATION_MS.count());
}

