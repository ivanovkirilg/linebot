#ifndef UI_INCLUDE_UI_USER_INTERFACE
#define UI_INCLUDE_UI_USER_INTERFACE

#include "DRVR/IDriver.hpp"
#include "DOMN/Move.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>


class UserInterface
{
public:
    void run(std::chrono::milliseconds refreshRate,
             std::weak_ptr<const IDriver> driver);
    void terminate();

    move::Move readMove();

private:
    std::atomic<bool> m_running{};
    std::atomic<bool> m_paused{};
    std::thread m_background;
};

#endif // SRC_UI
