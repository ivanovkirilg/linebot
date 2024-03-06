#ifndef CTRL_INCLUDE_CTRL_CONTROLLER
#define CTRL_INCLUDE_CTRL_CONTROLLER
#ifndef SRC_WAFER_STAGE
#define SRC_WAFER_STAGE

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

#endif // SRC_WAFER_STAGE


#endif // CTRL_INCLUDE_CTRL_CONTROLLER
