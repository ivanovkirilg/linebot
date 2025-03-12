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

    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

};

class ExampleClient : public IExampleClient
{
public:
    ExampleClient(const std::string& localAddress);
    ExampleClient(int serverPort);

    void set(double speed) override;
    void get(double& speed) override;

private:
    COMM::Connection m_serverConnection;
    std::mutex m_mutex;
};

} // XMPL

#endif // _TMP_EXAMPLE_CLIENT
