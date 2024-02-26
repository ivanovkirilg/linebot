#ifndef SRC_UI
#define SRC_UI

#include "Driver.hpp"
#include "Move.hpp"

#include <memory>
#include <chrono>

class UserInterface
{
public:
    void run(std::chrono::milliseconds refreshRate,
             std::weak_ptr<const Driver> driver);
    void join();

    static Move readMove();

private:
    std::atomic<bool> m_running{};
    std::thread m_background;
};

#endif // SRC_UI
