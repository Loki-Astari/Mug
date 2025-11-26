#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"
#include <thread>
#include <variant>


#include "SlackClient.h"
#include "APIChatPostMessage.h"
#include "APIChatDelete.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Delete;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIChatDeleteTest, SimpleText)
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

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    Delete::Reply      reply1 = client.sendMessage(Delete {
            .channel = "C09RU2URYMS",
            .ts = reply.message.value().ts,
    });

    if (!reply1.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply1);
    }
    EXPECT_TRUE(reply1.ok);
}

#endif
