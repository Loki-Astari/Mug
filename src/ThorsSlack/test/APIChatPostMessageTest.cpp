#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"
#include <variant>



#include "Environment.h"
#include "SlackClient.h"
#include "APIChatPostMessage.h"
#include "ThorSerialize/JsonThor.h"
#include "SlackBlockKit.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;

Environment             environment("test/data/environment.json");
SlackClient             client(environment.slackToken);

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIChatPostMessageTest, SimpleText)
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
}

TEST(APIChatPostMessageTest, Block_Section_ElText)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::Section{
                                                                            .text = BK::ElText{
                                                                                        .type = BK::mrkdwn,
                                                                                        .text = "Stuff to print"}
                                                                            },
                                                                        BK::Divider{},
                                                                        BK::Section{
                                                                            .text = BK::ElText{
                                                                                        .type = BK::plain_text,
                                                                                        .text = "Here we go"
                                                                            }
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_EQ(3, reply.message->blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message->blocks[0]));
    BK::Section&            section1 = std::get<BK::Section>(reply.message->blocks[0]);
    ASSERT_TRUE(section1.text.has_value());
    EXPECT_EQ("Stuff to print", section1.text.value().text);

    ASSERT_TRUE(std::holds_alternative<BK::Divider>(reply.message->blocks[1]));

    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message->blocks[2]));
    BK::Section&            section2 = std::get<BK::Section>(reply.message->blocks[2]);
    ASSERT_TRUE(section2.text.has_value());
    EXPECT_EQ("Here we go", section2.text.value().text);
}

TEST(APIChatPostMessageTest, MessageWithBadJSON)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "Json does not support new line\n"});
    ASSERT_FALSE(reply.ok);
    EXPECT_TRUE(reply.error.has_value());
}
#endif
