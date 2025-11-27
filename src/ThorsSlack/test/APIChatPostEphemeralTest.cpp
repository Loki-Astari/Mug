#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatPostEphemeral.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostEphemeral;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIChatPostEphemeralTest, SimpleText)
{
    PostEphemeral::Reply      reply = client.sendMessage(PostEphemeral{.channel = "C09RU2URYMS", .user="U095XJHJ1J5", .text = "I hope the tour went well, Mr. Wonka."});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

#endif
