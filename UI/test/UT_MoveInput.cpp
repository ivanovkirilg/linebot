#include "MoveInput.hpp"

#include "LOGR/ILogger.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <sstream>

#include <memory>

using namespace UI;
using testing::HasSubstr;


class TestMoveInput : public ::testing::Test
{
protected:
    std::stringstream input;
    std::unique_ptr<MoveInput> moveInput;
    std::shared_ptr<LOGR::ILogger> logger;

    void SetUp() override
    {
        logger = LOGR::ILogger::create("UI_UT", LOGR::StubSelection::STUB);
        moveInput = std::make_unique<MoveInput>(input);
    }

    void expect_linear_move()
    {
        EXPECT_TRUE(moveInput->valid());
        EXPECT_EQ(moveInput->move.type, DOMN::MoveType::LINEAR);
        EXPECT_THAT(moveInput->profilePrompt(), HasSubstr("speed"));
    }

    void expect_linear_profile(double position, double speed)
    {
        const auto& profile = std::get<DOMN::LinearMove>(moveInput->move.profile);

        EXPECT_EQ(profile.targetPosition, position);
        EXPECT_EQ(profile.speed, speed);
    }

    void expect_triangular_move()
    {
        EXPECT_TRUE(moveInput->valid());
        EXPECT_EQ(moveInput->move.type, DOMN::MoveType::TRIANGULAR);
        EXPECT_THAT(moveInput->profilePrompt(), HasSubstr("acceleration"));
    }

    void expect_triangular_profile(double position, double acceleration)
    {
        const auto& profile = std::get<DOMN::TriangularMove>(moveInput->move.profile);

        EXPECT_EQ(profile.targetPosition, position);
        EXPECT_EQ(profile.acceleration, acceleration);
    }
};


TEST_F(TestMoveInput, ReadsAbbreviatedLinearMoveType)
{
    input.str("l\n");

    moveInput->tryReadMoveType();

    expect_linear_move();
}

TEST_F(TestMoveInput, ReadsFullLinearMoveType)
{
    input.str("linear\n");

    moveInput->tryReadMoveType();

    expect_linear_move();
}

TEST_F(TestMoveInput, ReadsAbbreviatedTriangularMoveType)
{
    input.str("t\n");

    moveInput->tryReadMoveType();

    expect_triangular_move();
}

TEST_F(TestMoveInput, ReadsFullTriangularMoveType)
{
    input.str("triangular\n");

    moveInput->tryReadMoveType();

    expect_triangular_move();
}

TEST_F(TestMoveInput, RejectsInvalidMoveType)
{
    input.str("invalid\n");

    moveInput->tryReadMoveType();

    EXPECT_FALSE(moveInput->valid());
}

TEST_F(TestMoveInput, ReadsLinearMoveProfile)
{
    input.str("0.5 0.1\n");

    moveInput->move.type = DOMN::MoveType::LINEAR;
    moveInput->tryReadMoveProfile();

    expect_linear_profile(0.5, 0.1);
}

TEST_F(TestMoveInput, ReadsTriangularMoveProfile)
{
    input.str("0.5 0.1\n");
    moveInput->move.type = DOMN::MoveType::TRIANGULAR;

    moveInput->tryReadMoveProfile();

    expect_triangular_profile(0.5, 0.1);
}

TEST_F(TestMoveInput, RetriesOnInvalidInput)
{
    input.str("invalid\ninvalid\nl\n");

    EXPECT_NO_THROW(moveInput->retry<&MoveInput::tryReadMoveType>(2));

    expect_linear_move();
}

TEST_F(TestMoveInput, ThrowsOnTooManyInvalidInputs)
{
    input.str("invalid\ninvalid\ninvalid\n");

    EXPECT_THROW(moveInput->retry<&MoveInput::tryReadMoveType>(2),
                 UI::InvalidInputException);
}

TEST_F(TestMoveInput, RejectsEmptyInput)
{
    input.str("");

    moveInput->tryReadMoveType();

    // FIXME make clearer that end of input is different from invalid input!!
    EXPECT_FALSE(moveInput->valid());
}

TEST_F(TestMoveInput, RejectsWholeInvalidLines)
{
    input.str("a 0.8 0.9\n0.5 0.1\n");
    moveInput->move.type = DOMN::MoveType::LINEAR;

    moveInput->tryReadMoveProfile();
    ASSERT_FALSE(moveInput->valid());

    moveInput->tryReadMoveProfile();
    ASSERT_TRUE(moveInput->valid());

    expect_linear_profile(0.5, 0.1);
}
