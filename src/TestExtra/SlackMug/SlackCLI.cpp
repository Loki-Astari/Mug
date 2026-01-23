#include "Environment.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/APIChatMessage.h"
#include "ThorsSlack/APIChatSchedule.h"
#include "ThorsSlack/APIChatStream.h"
#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

int main()
{
    ThorsLogLevel(9);
    const Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.botToken);

    using ThorsAnvil::Slack::SlackClient;
    using ThorsAnvil::Slack::API::Chat::POSTMessage;
    using ThorsAnvil::Slack::API::Chat::ScheduleMessage;
    using ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage;
    using ThorsAnvil::Slack::API::Chat::ScheduledMessagesList;
    using ThorsAnvil::Slack::API::Chat::StartStream;

    SlackClient             client(environment.botToken, environment.userToken);

    POSTMessage::Reply reply;
    client.sendMessage(POSTMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."}, reply);
    //
    // auto reply1 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "A timed message"});
    // auto reply2 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "A timed message"});
    // auto reply4 = client.sendMessage(ScheduledMessageList{.channel = "C09RU2URYMS"});
    // auto reply3 = client.sendMessage(DeleteScheduledMessage{.channel = "C09RU2URYMS", .scheduled_message_id = reply1.scheduled_message_id,});
    // auto reply5 = client.sendMessage(ScheduledMessageList{.channel = "C09RU2URYMS"});
    // auto reply6 = client.sendMessage(DeleteScheduledMessage{.channel = "C09RU2URYMS", .scheduled_message_id = reply2.scheduled_message_id,});

    client.sendMessage(StartStream{.channel = "@martin.york", .thread_ts = reply.message.ts});
    // std::cout << ThorsAnvil::Serialize::jsonExporter(reply4);
    // std::cout << ThorsAnvil::Serialize::jsonExporter(reply6);
}
