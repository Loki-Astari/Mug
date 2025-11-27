#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "APIChatMessage.h"
#include "APIChatUnfurl.h"
#include "SlackBlockKit.h"
#include "SlackClient.h"
#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Unfurl;
using ThorsAnvil::Slack::API::Chat::UnfurlURL;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

#if 0
TEST(APIChatUnfurlTest, SimpleText)
{
    PostMessage::Reply  reply   = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply.ok);

    Unfurl message{
            .channel = "C09RU2URYMS",
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
    PostUnfurl::Reply      reply = client.sendMessage(Unfurl{.channel = "C09RU2URYMS", .user="U095XJHJ1J5", .text = "I hope the tour went well, Mr. Wonka."});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
#endif
}
#endif

#endif
