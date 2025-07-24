#include "ExampleImpl.hpp"

#include "LOGR/Trace.hpp"


void XMPL::ExampleImpl::set(double value)
{
    LOGR::Trace trace{m_value, "=>", value};
    m_value = value;
}

double XMPL::ExampleImpl::get()
{
    LOGR::Trace trace{m_value};
    m_breakLoop = true;
    return m_value;
}

void XMPL::ExampleImpl::calculate(double& square, double& cube)
{
    LOGR::Trace trace{m_value};
    square = m_value * m_value;
    cube   = m_value * m_value * m_value;
    trace.log(square, cube);
}

