#ifndef CTRL_INCLUDE_CTRL_CONTROLLER
#define CTRL_INCLUDE_CTRL_CONTROLLER

#include "DOMN/Move.hpp"
#include "DRVR/DriverClient.hpp"

#include <memory>


///
/// \brief Calculate & send commands for the Driver to execute a specified move.
///
class Controller
{
public:
    Controller(std::shared_ptr<DRVR::IDriverClient> driver)
    :   m_driver(driver)
    { }

    /// \brief Move as specified, blocking the caller and enabling
    /// the driver's logging for the needed duration.
    /// \pre The Driver has been `run()`.
    /// \post The Driver is stationary.
    /// \note The exact `targetPosition` is not guaranteed to be reached.
    void executeMove(const DOMN::Move& move);

private:
    void executeMove(const DOMN::LinearMove& move);
    void executeMove(const DOMN::TriangularMove& move);

    double getDirection(double targetPosition) const;

    double getDistance(double targetPosition) const;

    std::shared_ptr<DRVR::IDriverClient> m_driver;
};

#endif // CTRL_INCLUDE_CTRL_CONTROLLER
