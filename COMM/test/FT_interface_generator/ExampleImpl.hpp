#ifndef COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL
#define COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL

#include "ExampleServer.hpp"

namespace XMPL
{

class ExampleImpl : public ExampleServer
{
protected:
    virtual void set(double speed) override;
    virtual void get(double& speed) override;

private:
    double m_speed{};
};

} // XMPL

#endif // COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL
