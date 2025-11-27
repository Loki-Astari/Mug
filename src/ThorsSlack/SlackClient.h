#ifndef THORSANVIL_SLACK_SLACKCLIENT_H
#define THORSANVIL_SLACK_SLACKCLIENT_H

#include "ThorsSlackConfig.h"
#include "API.h"
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
        void sendMessageData(T const& message, SlackStream& stream)
        {

            if constexpr (T::method == API::Method::GET) {
                std::string api = std::string{} + T::api + "?" + message.query();
                Nisse::ClientRequest    post(stream, api, T::method);
                post.addHeaders(headers);
                post.body(0);
            }
            else {
                // Anything that is not a GET
                Nisse::ClientRequest    post(stream, T::api, T::method);
                post.addHeaders(headers);
                std::size_t size = Ser::jsonStreanSize(message);
                // std::cerr << "Send: " << size << " " << Ser::jsonExporter(message, Ser::PrinterConfig{Ser::OutputType::Stream}) << "\n";
                post.body(size) << Ser::jsonExporter(message, Ser::PrinterConfig{Ser::OutputType::Stream});
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
        T::Reply  sendMessage(T const& message)
        {
            SlackStream             stream;
            sendMessageData(message, stream);

            Nisse::ClientResponse   response(stream);
            typename T::Reply       reply;
            stream >> Ser::jsonImporter(reply);

            return reply;
        }
};

}

#endif
