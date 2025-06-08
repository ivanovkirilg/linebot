#ifndef UI_INCLUDE_UI_USER_INTERFACE
#define UI_INCLUDE_UI_USER_INTERFACE

#include "DRVR/IDriver.hpp"
#include "DOMN/Move.hpp"
#include "LOGR/Exception.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>


namespace UI
{

class InvalidInputException : public LOGR::Exception
{
    using LOGR::Exception::Exception;
};

class UserInterface
{
public:
    void run(std::chrono::milliseconds refreshRate,
             std::weak_ptr<IDriver> driver);
    void terminate();

    std::optional<DOMN::Move> readMove();

private:
    std::atomic<bool> m_running{};
    std::mutex m_outputMutex;
    std::thread m_background;
};

}

#endif // SRC_UI
