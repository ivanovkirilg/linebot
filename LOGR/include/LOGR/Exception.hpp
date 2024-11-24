#ifndef LOGR_INCLUDE_LOGR_EXCEPTION
#define LOGR_INCLUDE_LOGR_EXCEPTION

#include <atomic>
#include <source_location>
#include <stdexcept>


namespace LOGR
{

///
/// Custom exception base class automatically logged on creation.
///
/// Each `Exception` instance gets a process-wide unique id in the log.
///
/// Derive your own exception classes from this one like so:
///
///     class SpecificException : public LOGR::Exception
///     {
///         using LOGR::Exception::Exception;
///     };
///
class Exception : public std::runtime_error
{
public:
    Exception(const std::string& message,
        const std::source_location& loc = std::source_location::current());

    /// Log the 'end' of a caught exception, distinguishable by id.
    void handle(const std::string& message,
        const std::source_location& loc = std::source_location::current()) const;

    long long id() const;

private:
    static std::atomic<unsigned long long> freeId;
    unsigned long long m_id{};
};

}

#endif // LOGR_INCLUDE_LOGR_EXCEPTION
