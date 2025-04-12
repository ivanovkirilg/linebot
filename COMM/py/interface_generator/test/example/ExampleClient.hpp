// client-side GENERATED file

#ifndef _TMP_EXAMPLE_CLIENT
#define _TMP_EXAMPLE_CLIENT

#include "COMM/Connection.hpp"

#include <mutex>
#include <string>


namespace XMPL
{

class IExampleClient
{
public:
    virtual ~IExampleClient() = default;

    virtual void set(double value) = 0;
    virtual double get() = 0;
    virtual void calculate(double& square, double& cube) = 0;

};

class ExampleClient : public IExampleClient
{
public:
    ExampleClient(const std::string& localAddress);
    ExampleClient(int serverPort);
    ~ExampleClient() override;

    void set(double value) override;
    double get() override;
    void calculate(double& square, double& cube) override;

private:
    COMM::Connection m_serverConnection;
    std::mutex m_mutex;
};

} // XMPL

#endif // _TMP_EXAMPLE_CLIENT
