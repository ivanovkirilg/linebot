// server-side hand-coded file

#include "Driver.hpp"

#include <iostream>

void Driver::set(double speed)
{
    std::cout << "setting speed " << speed << '\n';
    m_speed = speed;
}

void Driver::get(double& speed)
{
    speed = m_speed;
    std::cout << "returning speed " << speed << '\n';
}