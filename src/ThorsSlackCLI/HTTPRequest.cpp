#include "HTTPRequest.h"
#include "ThorsLogging/ThorsLogging.h"

using namespace ThorsAnvil::Nisse::HTTP;

std::string HTTPRequest::getHost(std::string const& url)
{
    auto find = url.find("://");
    if (find == std::string::npos) {
        find = 0;
    }
    else {
        find += 3;
    }
    auto path = std::min(std::size(url), url.find('/', find));
    auto port = url.find(':', find);
    auto end  = std::min(path, port);
    return url.substr(find, (end - find));
}

std::string_view HTTPRequest::getRequest(std::string const& url)
{
    auto find = url.find("://");
    if (find == std::string::npos) {
        find = 0;
    }
    else {
        find += 3;
    }
    auto path = std::min(std::size(url), url.find('/', find));
    return {std::begin(url) + path, std::end(url)};
}

HTTPRequest::HTTPRequest(std::ostream& baseStream, std::string url, Method method, Version version)
    : method(method)
    , version(version)
    , url(std::move(url))
    , baseStream(baseStream)
    , headerSent(false)
{}

HTTPRequest::~HTTPRequest()
{
    ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "~HTTPRequest", "Responce object destruction");
    if (stream.rdbuf() == nullptr)
    {
        if (!headerSent)
        {
            ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "~HTTPRequest", "Sending header: ", method, " ", url, " ", version);
            baseStream << method << " " << getRequest(url) << " " << version << "\r\n"
                       << "Host: " << getHost(url) << "\r\n";

            headerSent = true;
        }
        ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "~HTTPRequest", "Setting content length to zero and flushing");
        baseStream << "content-length: 0\r\n"
            << "\r\n"
            << std::flush;
    }
}

namespace ThorsAnvil::Nisse::HTTP
{
    // Forward declare as it currently is not in header file.
    // TODO FIX
    std::ostream& operator<<(std::ostream& stream, Header const& header);
}

void HTTPRequest::addHeaders(Header const& headers)
{
    if (stream.rdbuf() != nullptr) {
        ThorsLogAndThrowWarning(std::runtime_error, "ThorsAnvil::Nisse::HTTPRequest", "addHeaders", "Headers can not be sent after the body has been started");
    }

    if (!headerSent)
    {
        ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "~HTTPRequest", "Sending header: ", method, " ", url, " ", version);
        baseStream << method << " " << getRequest(url) << " " << version << "\r\n"
                   << "Host: " << getHost(url) << "\r\n";
        headerSent = true;
    }

    baseStream << headers;
}

std::ostream& HTTPRequest::body(BodyEncoding bodyEncoding)
{
    ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "body", "adding body");
    if (!headerSent)
    {
        ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "~HTTPRequest", "Sending header: ", method, " ", url, " ", version);
        baseStream << method << " " << getRequest(url) << " " << version << "\r\n"
                   << "Host: " << getHost(url) << "\r\n";
        headerSent = true;
    }
    ThorsLogDebug("ThorsAnvil::Nisse::HTTP::HTTPRequest", "body", "adding body to stream");
    baseStream << bodyEncoding
               << "\r\n"
               << std::flush;

    stream.addBuffer(StreamBufOutput{baseStream, bodyEncoding});
    return stream;
}
