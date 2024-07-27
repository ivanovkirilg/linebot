#ifndef LOGR_INCLUDE_LOGR_EXCEPTION
#define LOGR_INCLUDE_LOGR_EXCEPTION

#include <atomic>
#include <source_location>
#include <stdexcept>


namespace LOGR
{

class Exception : public std::runtime_error
{
public:
    Exception(const std::string& message,
        const std::source_location& loc = std::source_location::current());

    void handle(const std::string& message,
        const std::source_location& loc = std::source_location::current()) const;

private:
    static std::atomic<unsigned long long> freeId;
    unsigned long long id{};
};

}

#endif // LOGR_INCLUDE_LOGR_EXCEPTION
