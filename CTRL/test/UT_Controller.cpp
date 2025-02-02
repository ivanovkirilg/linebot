#include "CTRL/Controller.hpp"

#include "DRVR/DriverClient.hpp"
#include "LOGR/ILogger.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"


using ::testing::NiceMock;


class DriverMock : public DRVR::IDriverClient
{
public:
    MOCK_METHOD(double, position, () );
    MOCK_METHOD(double, velocity, () );
    MOCK_METHOD(void, loggingOn, () );
    MOCK_METHOD(void, loggingOff, () );
    MOCK_METHOD(void, run, (int refreshRate_ms) );
    MOCK_METHOD(void, terminate, () );
    MOCK_METHOD(void, accelerate, (double instantaneousAcceleration) );
};


TEST(TestController, ExecuteMoveTogglesLogging)
{
    auto logr = LOGR::ILogger::create("CTRL_UT", LOGR::StubSelection::STUB);
    auto driverMock = std::make_shared<NiceMock<DriverMock>>();

    Controller controller(driverMock);

    EXPECT_CALL(*driverMock, loggingOn());
    EXPECT_CALL(*driverMock, loggingOff());

    controller.executeMove({});
}
