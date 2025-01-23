// server-side GENERATED file

#include "DriverServer.hpp"

#include "COMM/InterfaceProtocol.hpp"

#include "LOGR/Exception.hpp"
#include "LOGR/Warning.hpp"
#include "LOGR/ILogger.hpp"

#include "zpp_bits.hpp"

#include <vector>
#include <sstream>

using namespace COMM;

namespace
{

std::string buildFailureMessage(const char* func, const LOGR::Exception& exc)
{
    std::ostringstream message;
    message << "DRVR::DriverServer::" << func
            << ": LOGR::Exception [" << exc.id() << "]: " << exc.what();
    return message.str();
}

std::string buildFailureMessage(const char* func, const std::exception& exc)
{
    std::ostringstream message;
    message << "DRVR::DriverServer::" << func
            << ": std::exception: " << exc.what();
    return message.str();
}

struct ErrorResult
{
    RequestResult code;
    std::string message;
};

}

void DRVR::DriverServer::handleRequest(Connection& client)
{
    const std::vector<std::byte> inBuffer = client.receive();
    auto in = zpp::bits::in(inBuffer, zpp::bits::endian::network{});
    auto [reply, out] = zpp::bits::data_out(zpp::bits::endian::network{});

    int methodCode = -1;
    if (zpp::bits::failure(in(methodCode))) // Read fail is probably not our fault
    {
        if (LOGR::ILogger::isSet())
        {
            LOGR::Warning{Messages::COULD_NOT_READ_METHOD_CODE};
        }

        out(ErrorResult(RequestResult::PROTOCOL_ERROR,
                        Messages::COULD_NOT_READ_METHOD_CODE)).or_throw(); // Write fail shoudn't happen
    }
    else
    {
        switch (methodCode)
        {

            case 0:
            {
                double speed{};
                in(speed).or_throw();
                try
                {
                    set(speed);
                }
                catch (LOGR::Exception& exc)
                {
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("set", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }
                catch (std::exception& exc)
                {
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("set", exc))).or_throw();
                }
                out(RequestResult::OK).or_throw();

                break;
            }

            case 1:
            {
                double speed{};

                try
                {
                    get(speed);
                }
                catch (LOGR::Exception& exc)
                {
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("get", exc))).or_throw();
                    exc.handle(Messages::SEND_ERR_TO_CLIENT);
                }
                catch (std::exception& exc)
                {
                    out(ErrorResult(RequestResult::METHOD_FAILED,
                                    buildFailureMessage("get", exc))).or_throw();
                }
                out(RequestResult::OK).or_throw();
                out(speed).or_throw();
                break;
            }

            default:
            {
                out(ErrorResult(RequestResult::PROTOCOL_ERROR,
                                Messages::BAD_METHOD_CODE
                                + std::to_string(methodCode))).or_throw();
            }
        }

        const size_t leftover = in.remaining_data().size();
        if (leftover)
        {
            reply.clear();
            out.reset();
            out(RequestResult::PROTOCOL_ERROR).or_throw();
            out(Messages::LEFTOVER_DATA + std::to_string(leftover)).or_throw();
        }
    }

    client.send(reply);
}
