#include "Environment.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/APIChatPostMessage.h"
#include "ThorsSlack/APIChatScheduleMessage.h"
#include "ThorsSlack/APIChatDeleteScheduledMessage.h"
#include "ThorsSlack/APIChatScheduledMessageList.h"
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
    using ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage;
    using ThorsAnvil::Slack::API::Chat::ScheduledMessageList;

    SlackClient             client(environment.slackToken);

    // auto reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    auto reply1 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "A timed message"});
    auto reply2 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "A timed message"});
    auto reply4 = client.sendMessage(ScheduledMessageList{.channel = "C09RU2URYMS"});
    auto reply3 = client.sendMessage(DeleteScheduledMessage{.channel = "C09RU2URYMS", .scheduled_message_id = reply1.scheduled_message_id,});
    auto reply5 = client.sendMessage(ScheduledMessageList{.channel = "C09RU2URYMS"});
    auto reply6 = client.sendMessage(DeleteScheduledMessage{.channel = "C09RU2URYMS", .scheduled_message_id = reply2.scheduled_message_id,});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply4);
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply6);
}
