// server-side GENERATED file

#include "ExampleServer.hpp"

#include "COMM/InterfaceProtocol.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Warning.hpp"
#include "LOGR/ILogger.hpp"

#include "zpp_bits.h"

#include <vector>
#include <sstream>

using namespace COMM;

namespace
{

std::string buildFailureMessage(const char* func, const LOGR::Exception& exc)
{
    std::ostringstream message;
    message << "XMPL::ExampleServer::" << func
            << ": LOGR::Exception [" << exc.id() << "]: " << exc.what();
    return message.str();
}

std::string buildFailureMessage(const char* func, const std::exception& exc)
{
    std::ostringstream message;
    message << "XMPL::ExampleServer::" << func
            << ": std::exception: " << exc.what();
    return message.str();
}

struct ErrorResult
{
    RequestResult code;
    std::string message;
};

}

Server::ConnectionStatus XMPL::ExampleServer::handleRequest(Connection& client)
{
    const std::vector<std::byte> COMM_inargs = client.receive();
    auto COMM_read = zpp::bits::in(COMM_inargs, zpp::bits::endian::network{});
    auto [COMM_reply, COMM_write] = zpp::bits::data_out(zpp::bits::endian::network{});

    int COMM_methodCode = 0;
    if (zpp::bits::failure(COMM_read(COMM_methodCode))) // Read fail is probably not our fault
    {
        LOGR::Warning{Messages::COULD_NOT_READ_METHOD_CODE};

        COMM_write(ErrorResult(RequestResult::PROTOCOL_ERROR,
                   Messages::COULD_NOT_READ_METHOD_CODE)).or_throw(); // Write fail shoudn't happen
    }
    else
    {
        switch (COMM_methodCode)
        {
            case -1:
            {
                return Server::ConnectionStatus::DISCONNECTED;
                break;
            }

            case 1:
            {
                // no return
                double value{};
                COMM_read(value).or_throw();
                try
                {
                    set(value);
                }
                catch (LOGR::Exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("set", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }
                catch (std::exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("set", exc))).or_throw();
                }
                COMM_write(RequestResult::OK).or_throw();

                // no return
                break;
            }

            case 2:
            {
                double COMM_retval{};


                try
                {
                    COMM_retval = get();
                }
                catch (LOGR::Exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("get", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }
                catch (std::exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("get", exc))).or_throw();
                }
                COMM_write(RequestResult::OK).or_throw();

                COMM_write(COMM_retval).or_throw();
                break;
            }

            case 3:
            {
                // no return
                double square{};
                double cube{};

                try
                {
                    calculate(square, cube);
                }
                catch (LOGR::Exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("calculate", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }
                catch (std::exception& exc)
                {
                    COMM_write(ErrorResult(RequestResult::METHOD_FAILED,
                               buildFailureMessage("calculate", exc))).or_throw();
                }
                COMM_write(RequestResult::OK).or_throw();
                COMM_write(square).or_throw();
                COMM_write(cube).or_throw();
                // no return
                break;
            }

            default:
            {
                COMM_write(ErrorResult(RequestResult::PROTOCOL_ERROR,
                           Messages::BAD_METHOD_CODE
                           + std::to_string(COMM_methodCode))).or_throw();
            }
        }

        const size_t COMM_leftover = COMM_read.remaining_data().size();
        if (COMM_leftover)
        {
            COMM_reply.clear();
            COMM_write.reset();
            COMM_write(RequestResult::PROTOCOL_ERROR).or_throw();
            COMM_write(Messages::LEFTOVER_DATA + std::to_string(COMM_leftover)).or_throw();
        }
    }

    client.send(COMM_reply);
    return Server::ConnectionStatus::CONNECTED;
}
