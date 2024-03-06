#ifndef UI_INCLUDE_UI_USER_INTERFACE
#define UI_INCLUDE_UI_USER_INTERFACE
#ifndef SRC_UI
#define SRC_UI

#include "DRVR/Driver.hpp"
#include "DOMN/Move.hpp"

#include <memory>
#include <chrono>

class UserInterface
{
public:
    void run(std::chrono::milliseconds refreshRate,
             std::weak_ptr<const Driver> driver);
    void terminate();

    move::Move readMove();

private:
    std::atomic<bool> m_running{};
    std::atomic<bool> m_paused{};
    std::thread m_background;
};

#endif // SRC_UI


#endif // UI_INCLUDE_UI_USER_INTERFACE
