#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"
#include <thread>

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))


#include "SlackClient.h"
#include "APIChatScheduleMessage.h"
#include "APIChatDeleteScheduledMessage.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::ScheduleMessage;
using ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage;

extern SlackClient             client;

TEST(APIChatDeleteScheduledMessageTest, DeleteAScheduledMessage)
{
    ScheduleMessage::Reply          reply1 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 120, .text = "I hope the tour went well, Mr. Wonka."});

    ASSERT_TRUE(reply1.ok);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
    DeleteScheduledMessage  message{.channel = "C09RU2URYMS", .scheduled_message_id = reply1.scheduled_message_id};
    DeleteScheduledMessage::Reply   reply2 = client.sendMessage(message);

    if (!reply2.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply2);
    }
    ASSERT_TRUE(reply2.ok);
}

#endif
