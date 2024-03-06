#ifndef CTRL_INCLUDE_CTRL_CONTROLLER
#define CTRL_INCLUDE_CTRL_CONTROLLER

#include "DOMN/Move.hpp"
#include "DRVR/Driver.hpp"

#include <memory>


class Controller
{
public:
    Controller(std::shared_ptr<Driver> driver) : m_driver(driver) { }

    void executeMove(const move::Move& move);

private:
    std::shared_ptr<Driver> m_driver;
};

#endif // CTRL_INCLUDE_CTRL_CONTROLLER
