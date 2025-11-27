#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"
#include <variant>


#include "SlackClient.h"
#include "APIChatPostMessage.h"
#include "APIChatUpdate.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Update;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIChatUpdateTest, SimpleText)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message->blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message->blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("I hope the tour went well, Mr. Wonka.", rtext.text);


    Update::Reply      reply1 = client.sendMessage(Update {
            .channel = "C09RU2URYMS",
            .ts = reply.message.value().ts,
            .blocks = BK::Blocks{},
            .text = "Update text.",
    });

    //if (!reply1.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply1);
    //}
    EXPECT_TRUE(reply1.ok);
    ASSERT_TRUE(reply1.message.has_value());
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply1.message->blocks[0]));
    BK::RichText&           text1 = std::get<BK::RichText>(reply1.message->blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text1.elements[0]));
    BK::RichTextSection&    section1 = std::get<BK::RichTextSection>(text1.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section1.elements[0]));
    BK::ElRtText&           rtext1 = std::get<BK::ElRtText>(section1.elements[0]);

    EXPECT_EQ("Update text.", rtext1.text);
}

#endif
