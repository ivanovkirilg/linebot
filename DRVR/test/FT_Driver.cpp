#include "Driver.hpp"

#include <gtest/gtest.h>

#include <ctime>
#include <filesystem>


class TestDriverFunctional : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
        std::filesystem::remove(expectedFile);
    }

    const time_t creationTime = ::time(nullptr);
    const std::string expectedFile = "driver"
                                     + std::to_string(creationTime)
                                     + ".txt";
    DRVR::Driver driver{0.5};
};


TEST_F(TestDriverFunctional, CreatesLogFile)
{
    ASSERT_TRUE(std::filesystem::exists(expectedFile));
}
