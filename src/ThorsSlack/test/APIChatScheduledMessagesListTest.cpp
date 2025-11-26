#include "ThorSerialize/JsonThor.h"
#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))


#include "SlackClient.h"
#include "APIChatScheduleMessage.h"
#include "APIChatDeleteScheduledMessage.h"
#include "APIChatScheduledMessagesList.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::ScheduleMessage;
using ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage;
using ThorsAnvil::Slack::API::Chat::ScheduledMessagesList;

extern SlackClient             client;

TEST(APIChatScheduledMessagesListTest, DeleteAScheduledMessage)
{
    ScheduledMessagesList::Reply    reply0 = client.sendMessage(ScheduledMessagesList{.channel = "C09RU2URYMS"});
    std::size_t originalSize = reply0.scheduled_messages.size();

    ScheduleMessage::Reply          reply1 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply1.ok);
    ScheduleMessage::Reply          reply2 = client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply2.ok);


    ScheduledMessagesList::Reply    reply3 = client.sendMessage(ScheduledMessagesList{.channel = "C09RU2URYMS"});

    ASSERT_TRUE(reply3.ok);
    ASSERT_EQ(2, reply3.scheduled_messages.size() - originalSize);
    EXPECT_EQ(reply1.scheduled_message_id, reply3.scheduled_messages[originalSize + 1].id);
    EXPECT_EQ(reply2.scheduled_message_id, reply3.scheduled_messages[originalSize + 0].id);
}

#endif
