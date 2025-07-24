
ABSTRACT_METHOD_TEMPLATE = "    virtual {ret} {name}({params}) = 0;"

METHOD_TEMPLATE          = "    {ret} {name}({params}) override;"

FILE_TEMPLATE = """// client-side GENERATED file

{header_guard}

#include "COMM/Connection.hpp"

#include <mutex>
#include <string>


namespace {namespace}
{{

class I{interface}Client
{{
public:
    virtual ~I{interface}Client() = default;

{abstract_methods}
}};

class {interface}Client : public I{interface}Client
{{
public:
    {interface}Client(const std::string& localAddress);
    {interface}Client(int serverPort);
    ~{interface}Client() override;

{methods}
private:
    COMM::Connection m_serverConnection;
    std::mutex m_mutex;
}};

}} // {namespace}

{end_header_guard}
"""
