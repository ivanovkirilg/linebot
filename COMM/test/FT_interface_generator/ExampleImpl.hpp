#ifndef COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL
#define COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL

#include "ExampleServer.hpp"

namespace XMPL
{

class ExampleImpl : public ExampleServer
{
protected:
    virtual void set(double value) override;
    virtual double get() override;
    virtual void calculate(double& square, double& cube) override;

private:
    double m_value{};
};

} // XMPL

#endif // COMM_TEST_FT_INTERFACE_GENERATOR_EXAMPLE_IMPL
