// client-side GENERATED file

#ifndef COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
#define COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT

#include "COMM/Connection.hpp"

#include <mutex>
#include <string>


namespace DRVR
{

class IDriverClient
{
public:
    virtual ~IDriverClient() = default;

    virtual void set(double speed) = 0;
    virtual void get(double& speed) = 0;

};

class DriverClient : public IDriverClient
{
public:
    DriverClient(const std::string& localAddress);
    DriverClient(int serverPort);

    void set(double speed) override;
    void get(double& speed) override;

private:
    COMM::Connection m_serverConnection;
    std::mutex m_mutex;
};

} // DRVR

#endif // COMM_PY_INTERFACE_GENERATOR_EXAMPLE_DRIVER_CLIENT
