#include "HTTPRequest.h"
#include "NisseHTTP/HeaderResponse.h"
#include "SlackStream.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/PrinterConfig.h"

std::string const SLACK_TOKEN = "<Token>";

using namespace std::literals::string_literals;

int main()
{
    loguru::g_stderr_verbosity = 9;
    ThorsLogDebug("main", "main", "SlackCLI");

    using ThorsAnvil::Nisse::HTTP::HTTPRequest;
    using ThorsAnvil::Nisse::HTTP::Method;
    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using ThorsAnvil::Nisse::HTTP::Encoding;
    using ThorsAnvil::Serialize::PrinterConfig;
    using ThorsAnvil::Serialize::OutputType;
    using ThorsAnvil::Slack::SlackStream;
    using ThorsAnvil::Slack::PostMessageData;

    HeaderResponse  headers;
    headers.add("Connection", "close");
    headers.add("Content-Type", "application/json; charset=utf-8");
    headers.add("Authorization", "Bearer " + SLACK_TOKEN);

    PostMessageData data{"C09RU2URYMS", "I hope the tour went well, Mr. Wonka."};

    SlackStream     stream;
    {
        HTTPRequest     post(stream, "https://slack.com/api/chat.postMessage"s, Method::POST);
        post.addHeaders(headers);
        std::size_t size = ThorsAnvil::Serialize::jsonStreanSize(data);
        post.body(size) << ThorsAnvil::Serialize::jsonExporter(data, PrinterConfig{OutputType::Stream});
    }

    std::string     line;
    while (std::getline(static_cast<std::istream&>(stream), line)) {
        std::cout << "L: >" << line << "<\n";
    }
    std::cout << "========\n";
}
