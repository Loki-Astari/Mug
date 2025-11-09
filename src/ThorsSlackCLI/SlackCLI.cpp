#include "HTTPRequest.h"
#include "SlackStream.h"

std::string const SLACK_TOKEN = "<Token>";

int main()
{
    loguru::g_stderr_verbosity = 9;
    ThorsLogDebug("main", "main", "SlackCLI");

    using ThorsAnvil::Nisse::HTTP::HTTPPost;
    using ThorsAnvil::Nisse::HTTP::HTTPGet;
    using ThorsAnvil::Nisse::HTTP::HTTPSStream;
    using ThorsAnvil::Nisse::HTTP::HTTPHeaders;
    using ThorsAnvil::Nisse::HTTP::HTTPData;

    std::map<std::string, std::string> headers
    {
        {"Connection", "close"},
        {"Content-Length", "72"}
    };
    ThorsAnvil::Slack::PostMessageData data{"C09RU2URYMS", "I hope the tour went well, Mr. Wonka."};

    ThorsAnvil::Slack::SlackStream  stream(SLACK_TOKEN);
    stream  << HTTPPost("https://slack.com/api/chat.postMessage", ThorsAnvil::Nisse::HTTP::Version::HTTP1_1)
            << HTTPHeaders(headers)
            << HTTPData(data)
            << std::flush;

    std::string     line;
    while (std::getline(static_cast<std::istream&>(stream), line)) {
        std::cout << "L: >" << line << "<\n";
    }
    std::cout << "========\n";
}
