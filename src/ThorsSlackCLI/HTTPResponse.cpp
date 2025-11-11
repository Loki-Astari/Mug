#include "HTTPResponse.h"
#include <string>

using namespace ThorsAnvil::Nisse::HTTP;

    /*
    protected:
        Version         version;
        StatusCode      status;
        std::map<std::string, std::string>  headers;
    */
namespace ThorsAnvil::Nisse::HTTP
{
    std::istream& operator>>(std::istream& stream, StatusResponse& data)
    {
        stream >> data.code;
        std::getline(stream, data.message);
        return stream;
    }
}

HTTPResponse::HTTPResponse(std::istream& stream)
{
    std::string     line;
    if (stream >> version >> status)
    {
        std::string     line;
        while (std::getline(stream, line)) {
            if (line == "\r") {
                break;
            }
            auto colon = std::min(std::size(line), line.find(':'));
            auto value = colon == std::size(line) ? colon : line.find_first_not_of(" ", colon + 1);

            headers.insert_or_assign(line.substr(0, colon), line.substr(value));
        }
    }
}
