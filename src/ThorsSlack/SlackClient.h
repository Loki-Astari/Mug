#ifndef THORSANVIL_SLACK_SLACKCLIENT_H
#define THORSANVIL_SLACK_SLACKCLIENT_H

#include "ThorSerialize/ParserConfig.h"
#include "ThorSerialize/PrinterConfig.h"
#include "ThorsSlackConfig.h"
#include "API.h"
#include "NisseHTTP/ClientRequest.h"
#include "NisseHTTP/ClientResponse.h"
#include "NisseHTTP/HeaderResponse.h"
#include "NisseHTTP/StreamInput.h"
#include "NisseHTTP/Util.h"
#include "SlackStream.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"

#include <string>
#include <iostream>

namespace ThorsAnvil::Slack
{

using namespace std::literals::string_literals;
namespace Ser   = ThorsAnvil::Serialize;
namespace Nisse = ThorsAnvil::Nisse::HTTP;

template<typename T>
using SuccFunc = std::function<void(T&&)>;
using FailFunc = std::function<void(API::Error&&)>;

template<typename T>
struct VisitResult
{
    SuccFunc<T>&& succ;
    FailFunc&&    fail;
    VisitResult(SuccFunc<T>&& succ, FailFunc&& fail)
        : succ(std::move(succ))
        , fail(std::move(fail))
    {}

    void operator()(T& val)            {succ(std::move(val));}
    void operator()(API::Error& val)   {fail(std::move(val));}
};

class SlackClient
{
    Nisse::HeaderResponse   headers;
    Ser::ParserConfig      config;
    private:
        template<typename T>
        void sendMessageData(T const& message, SlackStream& stream)
        {

            if constexpr (T::method == API::Method::GET) {
                std::string api = std::string{} + T::api + "?" + ThorsAnvil::Slack::API::buildQueryA(message);
                Nisse::ClientRequest    post(stream, api, T::method);
                post.addHeaders(headers);
                post.body(0);
            }
            else {
                // Anything that is not a GET
                Nisse::ClientRequest    post(stream, T::api, T::method);
                post.addHeaders(headers);
                std::size_t size = Ser::jsonStreanSize(message);
                post.body(size) << Ser::jsonExporter(message, Ser::PrinterConfig{Ser::OutputType::Stream});
            }
        }
        template<typename T>
        std::string getEventType(Nisse::StreamInput& input, bool& hit)
        {
            if (hit) {
                return "";
            }
            hit = true;
            std::string_view    body = input.preloadStreamIntoBuffer();
            if (body.find(R"("ok":false)") != std::string_view::npos) {
                ThorsLogDebug("SlackClient", "getEventType", "Found: Error");
                return "ThorsAnvil::Slack::API::Error";
            }
            if (body.find(R"("ok":true)") != std::string_view::npos) {
                ThorsLogDebug("SlackClient", "getEventType", "Found: Result: ", T::polyMorphicSerializerName());
                return T::polyMorphicSerializerName();
            }
            ThorsLogDebug("SlackClient", "getEventType", "Found: Fallback object members");
            return "";
        }
    public:
        SlackClient(std::string const& token)
        {
            headers.add("Connection", "close");
            headers.add("Content-Type", "application/json; charset=utf-8");
            headers.add("Authorization", "Bearer " + token);

            config.setIdentifyDynamicClass([](Ser::DataInputStream&){return "";});
        }

        template<typename T>
        void  tryMessage(T const& message)
        {
            SlackStream             stream;
            sendMessageData(message, stream);

            Nisse::ClientResponse   response(stream);
            std::istream&           output = stream;
            std::string             line;

            std::cerr << response << "\n";
            while (std::getline(output, line)) {
                std::cerr << line << "\n";
            }
            std::cerr << "========\n";
        }
        template<typename T>
        void  sendMessage(T const& message, SuccFunc<typename T::Reply>&& succ = [](typename T::Reply&&){}, FailFunc&& fail = [](API::Error&&){})
        {
            using ResultType = typename T::Reply;
            using OutputType = std::variant<API::Error, ResultType>;

            SlackStream             stream;
            sendMessageData(message, stream);

            Nisse::ClientResponse   response(stream);
            Nisse::StreamInput      input(stream, response.getContentSize());
            OutputType              reply;
            bool hit = false;
            input >> Ser::jsonImporter(reply, Ser::ParserConfig{}.setIdentifyDynamicClass([&](Ser::DataInputStream&){return getEventType<ResultType>(input, hit);}));

            std::visit(VisitResult<ResultType>{std::move(succ), std::move(fail)}, reply);
        }
        template<typename T>
        bool  sendMessage(T const& message, typename T::Reply& result, bool dumpError = false)
        {
            bool good = true;;
            sendMessage(message, [&result](typename T::Reply&& value){result = std::move(value);}, [&dumpError,&good](API::Error&& value){good = false;if (dumpError){std::cerr << ThorsAnvil::Serialize::jsonExporter(value) << "\n";}});
            return good;
        }
};

}

#endif
