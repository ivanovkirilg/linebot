#include "CTRL/Controller.hpp"

#include "DOMN/Move.hpp"
#include "DRVR/IDriver.hpp"
#include "LOGR/ILogger.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"


using ::testing::NiceMock;
using ::testing::Test;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRoundRobin;


class IDriverMock : public IDriver
{
public:
    MOCK_METHOD(void, loggingOn, ());
    MOCK_METHOD(void, loggingOff, ());
    MOCK_METHOD(double, position, (), (const));
    MOCK_METHOD(double, velocity, (), (const));
    MOCK_METHOD(void, run, (std::chrono::milliseconds refreshRate));
    MOCK_METHOD(void, terminate, ());
    MOCK_METHOD(void, setVelocity, (double velocity));
    MOCK_METHOD(void, setAcceleration, (double acceleration));
};

using DriverMock = NiceMock<IDriverMock>;

class TestController : public Test
{
public:
    std::shared_ptr<LOGR::ILogger> m_logr = LOGR::ILogger::create(
        "CTRL_UT", LOGR::StubSelection::STUB);

    std::shared_ptr<DriverMock> m_driverMock = std::make_shared<DriverMock>();
    Controller m_controller{m_driverMock};
};

TEST_F(TestController, ExecuteMoveTogglesLogging)
{
    EXPECT_CALL(*m_driverMock, loggingOn());
    EXPECT_CALL(*m_driverMock, loggingOff());

    m_controller.executeMove(DOMN::Move{});
}

TEST_F(TestController, LinearMoveSetsDriverVelocity)
{
    DOMN::Move linearMove {
        DOMN::MoveType::LINEAR,
        DOMN::LinearMove{ .targetPosition = 0.1, .speed = 0.9 } };

    {
        InSequence sequence;
        // TODO Controller issues a defensive 'stop'; do we want to keep this?
        EXPECT_CALL(*m_driverMock, setVelocity(0.0));

        EXPECT_CALL(*m_driverMock, setVelocity(0.9));
        EXPECT_CALL(*m_driverMock, setVelocity(0.0));
    }

    m_controller.executeMove(linearMove);
}

TEST_F(TestController, TriangularMoveSetsDriverAcceleration)
{
    DOMN::Move triangularMove {
        DOMN::MoveType::TRIANGULAR,
        DOMN::TriangularMove{ .targetPosition = 0.1, .acceleration = 0.2 } };

    ON_CALL(*m_driverMock, position()).WillByDefault(ReturnRoundRobin(
        { .5, .4, .3, .2, .1 }
    ));

    ON_CALL(*m_driverMock, velocity()).WillByDefault(Return(0.2));

    {
        InSequence sequence;
        EXPECT_CALL(*m_driverMock, setAcceleration(-0.2));
        EXPECT_CALL(*m_driverMock, setAcceleration(+0.2));
        EXPECT_CALL(*m_driverMock, setVelocity(0.0));
    }

    m_controller.executeMove(triangularMove);
}
