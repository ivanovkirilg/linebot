#ifndef UI_INCLUDE_UI_USER_INTERFACE
#define UI_INCLUDE_UI_USER_INTERFACE

#include "DRVR/IDriver.hpp"
#include "DOMN/Move.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <stdexcept>

namespace UI
{

class InvalidInputException : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class UserInterface
{
public:
    void run(std::chrono::milliseconds refreshRate,
             std::weak_ptr<const IDriver> driver);
    void terminate();

    std::optional<move::Move> readMove();

private:
    std::atomic<bool> m_running{};
    std::mutex m_outputMutex;
    std::thread m_background;
};

}

#endif // SRC_UI
