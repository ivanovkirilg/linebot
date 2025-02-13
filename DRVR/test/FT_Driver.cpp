#include "Driver.hpp"

#include "DOMN/Move.hpp"
#include "LOGR/ILogger.hpp"

#include <gtest/gtest.h>

#include <ctime>
#include <filesystem>


using namespace std::chrono_literals;


class TestDriverFunctional : public ::testing::Test
{
protected:
    static constexpr double STARTING_POSITION = 0.5;
    static constexpr int REFRESH_RATE_MS = 10;

    void SetUp() override
    {
        driver.run(REFRESH_RATE_MS);
    }

    void TearDown() override
    {
        driver.terminate();

        std::filesystem::remove(expectedFile);
    }
    
    const time_t creationTime = ::time(nullptr);
    const std::string expectedFile = "driver"
                                     + std::to_string(creationTime)
                                     + ".txt";
    DRVR::Driver driver{STARTING_POSITION};

    std::shared_ptr<LOGR::ILogger> log = LOGR::ILogger::create(
        "FT_Driver", LOGR::StubSelection::STUB);
};


TEST_F(TestDriverFunctional, CreatesLogFile)
{
    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}

TEST_F(TestDriverFunctional, Accelerates)
{
    constexpr double inputAcceleration = 0.2;

    driver.accelerate(inputAcceleration);
    driver.accelerate(inputAcceleration);
    driver.accelerate(inputAcceleration);

    ASSERT_NEAR(driver.velocity(), inputAcceleration * 3, .00001);

    driver.accelerate(-inputAcceleration);
    driver.accelerate(-inputAcceleration);
    driver.accelerate(-inputAcceleration);

    ASSERT_NEAR(driver.velocity(), 0, .00001);
}

TEST_F(TestDriverFunctional, Moves)
{
    constexpr double inputVelocity = 0.2;

    driver.accelerate(inputVelocity);

    EXPECT_NEAR(driver.velocity(), inputVelocity, .00001);

    constexpr int sleepTicks = 10;
    constexpr std::chrono::milliseconds sleepTime{REFRESH_RATE_MS * sleepTicks};
    constexpr double tickDistance = REFRESH_RATE_MS / 1000.0  * inputVelocity;
    constexpr double expectedDistance = tickDistance * sleepTicks;

    std::this_thread::sleep_for(sleepTime);

    EXPECT_NEAR(driver.position(), STARTING_POSITION + expectedDistance, tickDistance * 1.5);
}

TEST_F(TestDriverFunctional, StopsAtBounds)
{
    constexpr double inputVelocity = 10;

    constexpr std::chrono::milliseconds sleepTime{150};

    driver.accelerate(inputVelocity);

    std::this_thread::sleep_for(sleepTime);

    EXPECT_NEAR(driver.position(), move::MAX_POSITION, 0.00001);
    ASSERT_NEAR(driver.velocity(), 0.0, 0.00001);

    driver.accelerate(-inputVelocity);

    std::this_thread::sleep_for(sleepTime);

    EXPECT_NEAR(driver.position(), move::MIN_POSITION, 0.00001);
    ASSERT_NEAR(driver.velocity(), 0.0, 0.00001);
}