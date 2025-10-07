#include "SYNC/Tick.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <numeric>

using namespace std::chrono;

class TestTick : public testing::Test
{
public:
    static constexpr size_t NR_TICKS = 50;
    static constexpr auto TICK_TIME = 8ms;

    SYNC::Tick tick{TICK_TIME};
};

TEST_F(TestTick, takesExactTime)
{
    constexpr auto UPPER_LIMIT = 0.5ms * NR_TICKS;

    // When
    auto start = system_clock::now();
    for (size_t i = 0; i < NR_TICKS; i++)
    {
        tick();
    }
    auto end = system_clock::now();

    // Then
    auto total = duration_cast<milliseconds>(end - start);

    EXPECT_GT(total, (NR_TICKS * TICK_TIME));
    EXPECT_LT(total, (NR_TICKS * TICK_TIME) + UPPER_LIMIT);
}

TEST_F(TestTick, reportsAccurateDeltaTime)
{
    constexpr auto ACCEPTABLE_DEVIATION_MS = NR_TICKS * TICK_TIME * 0.05;

    // Given
    std::vector<SYNC::Tick::duration> reportedDeltas;
    reportedDeltas.reserve(NR_TICKS);

    // When
    auto start = system_clock::now();
    for (size_t i = 0; i < NR_TICKS; i++)
    {
        tick();
        reportedDeltas.push_back(tick.deltaTime());
    }
    auto end = system_clock::now();

    // Then
    auto total = duration_cast<milliseconds>(end - start);
    auto reportedSum = duration_cast<milliseconds>(
        std::accumulate(reportedDeltas.begin(), reportedDeltas.end(), 0ns)
    );

    EXPECT_NEAR(reportedSum.count(), total.count(), ACCEPTABLE_DEVIATION_MS.count());
}

