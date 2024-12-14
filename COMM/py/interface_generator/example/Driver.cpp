// server-side hand-coded file

#include "Driver.hpp"

#include "LOGR/Exception.hpp"

#include <iostream>

using namespace DRVR;

void Driver::set(double speed)
{
    std::cout << "setting speed " << speed << '\n';
    m_speed = speed;
    if (m_speed > 100)
    {
        throw std::runtime_error("Speed too high");
    }
}

void Driver::get(double& speed)
{
    speed = m_speed;
    if (m_speed > 100)
    {
        throw LOGR::Exception("Speed too high");
    }
    std::cout << "returning speed " << speed << '\n';
}
