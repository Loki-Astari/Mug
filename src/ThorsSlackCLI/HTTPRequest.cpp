#include "HTTPRequest.h"

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

HTTPRequest::HTTPRequest(std::string const& url, Version version)
    : url(url)
    , version(version)
{}

HTTPRequest::~HTTPRequest()
{}

void HTTPRequest::print(std::ostream& stream) const
{
    stream  << type() << " " << getRequest(url) << " " << version << "\r\n"
            << "Host: " << getHost(url) << "\r\n";
}

namespace ThorsAnvil::Nisse::HTTP
{
std::ostream& operator<<(std::ostream& stream, HTTPRequest const& request)
{
    request.print(stream);
    return stream;
}
}
