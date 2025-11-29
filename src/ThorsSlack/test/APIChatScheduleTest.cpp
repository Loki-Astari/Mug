#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))


#include "APIChatSchedule.h"
#include "SlackClient.h"
#include "SlackBlockKit.h"

#include "ThorSerialize/JsonThor.h"

#include <thread>

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::ScheduleMessage;
using ThorsAnvil::Slack::API::Chat::DeleteScheduledMessage;
using ThorsAnvil::Slack::API::Chat::ScheduledMessagesList;

extern SlackClient             client;

TEST(APIChatScheduleTest, CreateScheduledMessage)
{
    ScheduleMessage::Reply      reply;
    client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message.blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message.blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("I hope the tour went well, Mr. Wonka.", rtext.text);
}

TEST(APIChatScheduledTest, DeleteScheduledMessage)
{
    ScheduleMessage::Reply          reply1;
    client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 120, .text = "I hope the tour went well, Mr. Wonka."}, reply1, true);
    ASSERT_TRUE(reply1.ok);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);
    DeleteScheduledMessage  message{.channel = "C09RU2URYMS", .scheduled_message_id = reply1.scheduled_message_id};
    DeleteScheduledMessage::Reply   reply2;
    client.sendMessage(message, reply2, true);
    ASSERT_TRUE(reply2.ok);
}

TEST(APIChatScheduledTest, ListScheduledMessage)
{
    ScheduledMessagesList::Reply    reply0;
    client.sendMessage(ScheduledMessagesList{.channel = "C09RU2URYMS"}, reply0, true);
    std::size_t originalSize = reply0.scheduled_messages.size();

    ScheduleMessage::Reply          reply1;
    client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "I hope the tour went well, Mr. Wonka."}, reply1, true);
    ASSERT_TRUE(reply1.ok);
    ScheduleMessage::Reply          reply2;
    client.sendMessage(ScheduleMessage{.channel = "C09RU2URYMS", .post_at = time(nullptr) + 60, .text = "I hope the tour went well, Mr. Wonka."}, reply2, true);
    ASSERT_TRUE(reply2.ok);


    ScheduledMessagesList::Reply    reply3;
    client.sendMessage(ScheduledMessagesList{.channel = "C09RU2URYMS"}, reply3, true);
    ASSERT_TRUE(reply3.ok);
    ASSERT_EQ(2, reply3.scheduled_messages.size() - originalSize);
    // Can tell the order.
    // But both should be in there
    auto find1 = std::find_if(std::begin(reply3.scheduled_messages), std::end(reply3.scheduled_messages), [&reply1](auto const& val){return val.id == reply1.scheduled_message_id;});
    auto find2 = std::find_if(std::begin(reply3.scheduled_messages), std::end(reply3.scheduled_messages), [&reply2](auto const& val){return val.id == reply2.scheduled_message_id;});
    EXPECT_TRUE(find1 != std::end(reply3.scheduled_messages));
    EXPECT_TRUE(find2 != std::end(reply3.scheduled_messages));
}

#endif
