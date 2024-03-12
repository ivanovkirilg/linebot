#include "CTRL/Controller.hpp"

#include "DRVR/IDriver.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"


using ::testing::NiceMock;


class DriverMock : public IDriver
{
public:
    MOCK_METHOD(void, loggingOn, ());
    MOCK_METHOD(void, loggingOff, ());
    MOCK_METHOD(double, position, (), (const));
    MOCK_METHOD(double, velocity, (), (const));
    MOCK_METHOD(void, run, (std::chrono::milliseconds refreshRate));
    MOCK_METHOD(void, terminate, ());
    MOCK_METHOD(void, accelerate, (double instantaneousAcceleration));
};


TEST(TestController, ExecuteMoveTogglesLogging)
{
    auto driverMock = std::make_shared<NiceMock<DriverMock>>();

    Controller controller(driverMock);

    EXPECT_CALL(*driverMock, loggingOn());
    EXPECT_CALL(*driverMock, loggingOff());

    controller.executeMove({});
}
