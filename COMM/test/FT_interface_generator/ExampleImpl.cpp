#include "ExampleImpl.hpp"

#include "LOGR/Trace.hpp"


void XMPL::ExampleImpl::set(double speed)
{
    LOGR::Trace trace{m_speed, "=>", speed};
    m_speed = speed;
}

void XMPL::ExampleImpl::get(double& speed)
{
    LOGR::Trace trace{m_speed};
    speed = m_speed;
    m_breakLoop = true;
}
