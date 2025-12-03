#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "APIChatMessage.h"
#include "Environment.h"
#include "SlackClient.h"
#include "SlackBlockKit.h"
#include "ThorSerialize/JsonThor.h"

#include <variant>
#include <thread>

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::PostEphemeral;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Chat::Update;

Environment             environment("test/data/environment.json");
SlackClient             client(environment.botToken, environment.userToken);

#if !(defined(DISABLE_SLACKTEST) && (DISABLE_SLACKTEST == 1))


TEST(APIChatMessageTest, SimpleText)
{
    PostMessage::Reply      reply;
    client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message.blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message.blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("I hope the tour went well, Mr. Wonka.", rtext.text);
}

TEST(APIChatMessageTest, Block_Section_ElText)
{
    PostMessage::Reply      reply;
    client.sendMessage(PostMessage{
                            .channel = environment.slackChannel,
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
                       }, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_EQ(3, reply.message.blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message.blocks[0]));
    BK::Section&            section1 = std::get<BK::Section>(reply.message.blocks[0]);
    ASSERT_TRUE(section1.text.has_value());
    EXPECT_EQ("Stuff to print", section1.text.value().text);

    ASSERT_TRUE(std::holds_alternative<BK::Divider>(reply.message.blocks[1]));

    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message.blocks[2]));
    BK::Section&            section2 = std::get<BK::Section>(reply.message.blocks[2]);
    ASSERT_TRUE(section2.text.has_value());
    EXPECT_EQ("Here we go", section2.text.value().text);
}

TEST(APIChatMessageTest, MessageWithBadJSON)
{
    PostMessage::Reply      reply;
    client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "Json does not support new line\n"}, reply);
    ASSERT_FALSE(reply.ok);
}

TEST(APIChatMessageTest, Delete)
{
    PostMessage::Reply      reply;
    client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message.blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message.blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("I hope the tour went well, Mr. Wonka.", rtext.text);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    Delete::Reply      reply1;
    client.sendMessage(Delete{
                            .channel = environment.slackChannel,
                            .ts = reply.message.ts,
                       }, reply1, true);

    EXPECT_TRUE(reply1.ok);
}


TEST(APIChatMessageTest, PostEphemeral)
{
    PostEphemeral::Reply      reply;
    client.sendMessage(PostEphemeral{.channel = environment.slackChannel, .user="U095XJHJ1J5", .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}


TEST(APIChatMessageTest, Update)
{
    PostMessage::Reply      reply;
    client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message.blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message.blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("I hope the tour went well, Mr. Wonka.", rtext.text);


    Update::Reply      reply1;
    client.sendMessage(Update {
                            .channel = environment.slackChannel,
                            .ts = reply.message.ts,
                            .blocks = BK::Blocks{},
                            .text = "Update text.",
                       }, reply1, true);

    EXPECT_TRUE(reply1.ok);
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply1.message.blocks[0]));
    BK::RichText&           text1 = std::get<BK::RichText>(reply1.message.blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text1.elements[0]));
    BK::RichTextSection&    section1 = std::get<BK::RichTextSection>(text1.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section1.elements[0]));
    BK::ElRtText&           rtext1 = std::get<BK::ElRtText>(section1.elements[0]);

    EXPECT_EQ("Update text.", rtext1.text);
}

#endif
