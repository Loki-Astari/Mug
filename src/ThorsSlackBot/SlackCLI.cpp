#include "Environment.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/APIChatPostMessage.h"
#include "ThorsSlack/APIChatScheduleMessage.h"
#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

int main()
{
    loguru::g_stderr_verbosity = 9;
    const Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.slackToken);

    using ThorsAnvil::Slack::SlackClient;
    using ThorsAnvil::Slack::API::Chat::PostMessage;
    using ThorsAnvil::Slack::API::Chat::ScheduleMessage;

    SlackClient             client(environment.slackToken);

    // auto reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    auto reply = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "A timed message"});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}
