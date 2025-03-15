#ifndef CTRL_INCLUDE_CTRL_CONTROLLER
#define CTRL_INCLUDE_CTRL_CONTROLLER

#include "DOMN/Move.hpp"
#include "DRVR/IDriver.hpp"

#include <memory>


class Controller
{
public:
    Controller(std::shared_ptr<IDriver> driver) : m_driver(driver) { }

    void executeMove(const DOMN::Move& move);

private:
    void executeMove(const DOMN::LinearMove& move);
    void executeMove(const DOMN::TriangularMove& move);

    double getDirection(double targetPosition) const;

    double getDistance(double targetPosition) const;

    std::shared_ptr<IDriver> m_driver;
};

#endif // CTRL_INCLUDE_CTRL_CONTROLLER
