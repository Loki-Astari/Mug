#ifndef THORSANVIL_SLACK_SLACKCLIENT_H
#define THORSANVIL_SLACK_SLACKCLIENT_H

#include "ThorsSlackConfig.h"
#include "NisseHTTP/ClientRequest.h"
#include "NisseHTTP/ClientResponse.h"
#include "NisseHTTP/HeaderResponse.h"
#include "NisseHTTP/Util.h"
#include "SlackStream.h"
#include "ThorSerialize/JsonThor.h"

#include <string>
#include <iostream>

namespace ThorsAnvil::Slack
{

using namespace std::literals::string_literals;
namespace Ser   = ThorsAnvil::Serialize;
namespace Nisse = ThorsAnvil::Nisse::HTTP;

class SlackClient
{
    Nisse::HeaderResponse   headers;
    private:
        template<typename T>
        void sendMessageData(T const& message, Nisse::Method method, SlackStream& stream)
        {
            Nisse::ClientRequest    post(stream, T::api, method);

            post.addHeaders(headers);
            if constexpr (T::hasBody) {
                std::size_t size = Ser::jsonStreanSize(message);
                post.body(size) << Ser::jsonExporter(message, Ser::PrinterConfig{Ser::OutputType::Stream});
            }
            else {
                post.body(0);
            }
        }
    public:
        SlackClient(std::string const& token)
        {
            headers.add("Connection", "close");
            headers.add("Content-Type", "application/json; charset=utf-8");
            headers.add("Authorization", "Bearer " + token);
        }

        template<typename T>
        void  tryMessage(T const& message, Nisse::Method method = Nisse::Method::POST)
        {
            SlackStream             stream;
            sendMessageData(message, method, stream);

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
        T::Reply  sendMessage(T const& message, Nisse::Method method = Nisse::Method::POST)
        {
            SlackStream             stream;
            sendMessageData(message, method, stream);

            Nisse::ClientResponse   response(stream);
            typename T::Reply       reply;
            stream >> Ser::jsonImporter(reply);

            return reply;
        }
};

}

#endif
