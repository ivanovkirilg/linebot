#ifndef CTRL_INCLUDE_CTRL_CONTROLLER
#define CTRL_INCLUDE_CTRL_CONTROLLER

#include "DOMN/Move.hpp"
#include "DriverClient.hpp"

#include <memory>


class Controller
{
public:
    Controller(std::shared_ptr<DRVR::DriverClient> driver) : m_driver(driver) { }

    void executeMove(const move::Move& move);

private:
    std::shared_ptr<DRVR::DriverClient> m_driver;
};

#endif // CTRL_INCLUDE_CTRL_CONTROLLER
