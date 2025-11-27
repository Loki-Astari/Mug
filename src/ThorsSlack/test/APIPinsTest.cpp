#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatPostMessage.h"
#include "APIPinsAdd.h"
#include "APIPinsList.h"
#include "APIPinsRemove.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Pins::Add;
using ThorsAnvil::Slack::API::Pins::List;
using ThorsAnvil::Slack::API::Pins::Remove;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIPinsTest, Add)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    ASSERT_TRUE(reply.ok);

    Add::Reply      reply1 = client.sendMessage(Add{.channel = "C09RU2URYMS", .timestamp=reply.message.value().ts});
    if (!reply1.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply1);
    }
    ASSERT_TRUE(reply1.ok);
}

TEST(APIPinsTest, List)
{
    List::Reply      reply = client.sendMessage(List{.channel = "C09RU2URYMS"});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(APIPinsTest, Remove)
{
    List::Reply      reply = client.sendMessage(List{.channel = "C09RU2URYMS"});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);

    for (auto const& pin: reply.items) {

        Remove::Reply r = client.sendMessage(Remove{.channel = pin.channel, .timestamp = pin.message.ts});
        if (!reply.ok) {
            std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
        }
        EXPECT_TRUE(r.ok);
    }
}

#endif
