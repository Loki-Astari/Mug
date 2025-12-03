#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#if !(defined(DISABLE_SLACKTEST) && (DISABLE_SLACKTEST == 1))

#include "APIChatMessage.h"
#include "Environment.h"
#include "APIChatUtil.h"

#include "SlackClient.h"
#include "SlackBlockKit.h"

#include "ThorSerialize/JsonThor.h"
#include <thread>
#include <variant>


using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::GetPermalink;
using ThorsAnvil::Slack::API::Chat::MeMessage;
using ThorsAnvil::Slack::API::Chat::Unfurl;
using ThorsAnvil::Slack::API::Chat::UnfurlURL;

extern SlackClient             client;
extern Environment             environment;


TEST(APIChatUtilTest, GetPermalink)
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
    GetPermalink::Reply      reply1;
    client.sendMessage(GetPermalink{.channel = environment.slackChannel, .message_ts = reply.message.ts}, reply1, true);
    EXPECT_TRUE(reply1.ok);
}

TEST(APIChatUtilTest, MeMessage)
{
    MeMessage::Reply      reply;
    client.sendMessage(MeMessage{.channel = environment.slackChannel, .text = "I hope the tour went well, Mr. Wonka."}, reply, true);
    ASSERT_TRUE(reply.ok);
}

#if 0
TODO
TEST(APIChatUtilTest, UnfurlURL)
{
    PostMessage::Reply  reply   = client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply.ok);

    Unfurl message{
            .channel = environment.slackChannel,
            .ts = reply.message.value().ts,
#if 0
            .unfurls = UnfurlURL
            {
                {
                    "https://github.com/Loki-Astari/",
                    {
                        .blocks =
                        {
                            ThorsAnvil::Slack::BlockKit::RichText
                            {
                                .elements =
                                {
                                    ThorsAnvil::Slack::BlockKit::RichTextSection
                                    {
                                        .elements =
                                        {
                                            ThorsAnvil::Slack::BlockKit::ElRtText
                                            {
                                                .text = "I see: Time"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
#else
            .unfurls = UnfurlURL
            {
                {
                    "https://gentle-buttons.com/carafe",
                    {
                        .blocks =
                        {
                            ThorsAnvil::Slack::BlockKit::Section
                            {
                                .text = ThorsAnvil::Slack::BlockKit::ElText
                                {
                                    .type = ThorsAnvil::Slack::BlockKit::TextType::mrkdwn,
                                    .text = "Take+a+look+at+this+carafe,+just+another+cousin+of+glass"
                                }
                            }
                        }
                    }
                }
            }
#endif
    };
    std::cerr << ThorsAnvil::Serialize::jsonExporter(message) << "\n";
    client.tryMessage(message);
#if 0
    PostUnfurl::Reply      reply = client.sendMessage(Unfurl{.channel = environment.slackChannel, .user="U095XJHJ1J5", .text = "I hope the tour went well, Mr. Wonka."});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
#endif
}
#endif

#endif
