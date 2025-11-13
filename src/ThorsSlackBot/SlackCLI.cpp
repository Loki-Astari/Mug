#include "Environment.h"
#include "SlackClient.h"
#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

int main()
{
    loguru::g_stderr_verbosity = 9;
    const Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.slackToken);

    using ThorsAnvil::Slack::SlackClient;
    using ThorsAnvil::Slack::PostMessageData;

    SlackClient             client(environment.slackToken);
    PostMessageData         data{"C09RU2URYMS", "I hope the tour went well, Mr. Wonka."};
    PostMessageData::Reply  reply = client.sendMessage(data);

    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}
