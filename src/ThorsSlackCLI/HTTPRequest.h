#ifndef THORSANVIL_NISSE_NISSEHTTP_HTTPREQUEST_H
#define THORSANVIL_NISSE_NISSEHTTP_HTTPREQUEST_H

#include "SlackCLIConfig.h"
#include "NisseHTTP/Util.h"
// Need to serialize the Data structures. See: jsonExporter()
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"

#include <ostream>
#include <string>

namespace ThorsAnvil::Nisse::HTTP
{

class HTTPRequest
{
    static std::string getHost(std::string const& url);
    static std::string_view getRequest(std::string const& url);

    protected:
        std::string const&  url;
        Version             version;

    public:
        HTTPRequest(std::string const& url, Version version = Version::HTTP1_1);
        virtual ~HTTPRequest();
        virtual char const* type() const = 0;
        void print(std::ostream& stream) const;

        friend std::ostream& operator<<(std::ostream& stream, HTTPRequest const& request);
};

class HTTPPost: public HTTPRequest
{
    public:
        HTTPPost(std::string const& url, Version version = Version::HTTP1_1)
            : HTTPRequest(url, version)
        {}
        virtual char const* type() const override {return "POST";}
};

class HTTPGet: public HTTPRequest
{
    public:
        HTTPGet(std::string const& url, Version version = Version::HTTP1_1)
            : HTTPRequest(url, version)
        {}
        virtual char const* type() const override {return "GET";}
};

template<typename T>
class HTTPHeaders
{
    T const&        headers;
    public:
        HTTPHeaders(T const& headers)
            : headers(headers)
        {}
        friend std::ostream& operator<<(std::ostream& stream, HTTPHeaders const& request)
        {
            for (auto const& head: request.headers) {
                stream << head.first << ": " << head.second << "\r\n";
            }
            stream << "\r\n";
            return stream;
        }
};

template<typename T>
class HTTPData
{
    T const&        data;
    public:
        HTTPData(T const& data)
            : data(data)
        {}
        friend std::ostream& operator<<(std::ostream& stream, HTTPData const& request)
        {
            return stream << ThorsAnvil::Serialize::jsonExporter(request.data, Serialize::PrinterConfig{Serialize::OutputType::Stream});
        }
};

}

#endif
