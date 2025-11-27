#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatMeMessage.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::MeMessage;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIChatMeMessageTest, SimpleText)
{
    MeMessage::Reply      reply = client.sendMessage(MeMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

#endif
