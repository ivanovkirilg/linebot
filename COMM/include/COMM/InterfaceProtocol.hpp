#ifndef COMM_INCLUDE_COMM_INTERFACE_PROTOCOL
#define COMM_INCLUDE_COMM_INTERFACE_PROTOCOL

namespace COMM
{

enum class RequestResult
{
    OK = 0,
    PROTOCOL_ERROR,
    METHOD_FAILED,
};

namespace Messages
{

constexpr const char* SEND_ERR_TO_CLIENT = "Sending error to client";

constexpr const char* CONNECTION_CLOSED = "Client closed the connection";
constexpr const char* COULD_NOT_READ_METHOD_CODE = "Failed to read request method code";
constexpr const char* BAD_METHOD_CODE = "Bad method code: ";
constexpr const char* LEFTOVER_DATA = "Leftover request data: ";

}

}


#endif // COMM_INCLUDE_COMM_INTERFACE_PROTOCOL
