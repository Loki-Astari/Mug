#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatPostMessage.h"
#include "APIReactionsGet.h"
#include "APIReactionsAdd.h"
#include "APIReactionsRemove.h"
#include "APIReactionsList.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Reactions::Get;
using ThorsAnvil::Slack::API::Reactions::Add;
using ThorsAnvil::Slack::API::Reactions::Remove;
using ThorsAnvil::Slack::API::Reactions::List;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIReactionsTest, AddTest)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "Will add a reaction to this"});
    ASSERT_TRUE(reply.ok);
    Add::Reply      reply1 = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "thumbsup", .timestamp = reply.message.value().ts});
    if (!reply1.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply1);
    }
    ASSERT_TRUE(reply1.ok);

    Add::Reply      replyA = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "thumbsup", .timestamp = reply.message.value().ts});
    EXPECT_FALSE(replyA.ok); // There is already a thumbs up reaction from this bot.

    Add::Reply      replyB = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "stuck_out_tongue_winking_eye", .timestamp = reply.message.value().ts});
    if (!replyB.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(replyB);
    }
    EXPECT_TRUE(replyB.ok);

    Add::Reply      replyC = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "smile", .timestamp = reply.message.value().ts});
    if (!replyC.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(replyC);
    }
    EXPECT_TRUE(replyC.ok);

    List::Reply     result = client.sendMessage(List{});
    if (!result.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }
    ASSERT_TRUE(result.ok);
    ASSERT_NE(0, result.items.size());


    Get::Reply     get = client.sendMessage(Get{.channel = "C09RU2URYMS", .timestamp = reply.message.value().ts});
    if (!result.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(get);
    }
    ASSERT_TRUE(result.ok);
    ASSERT_EQ(3, get.message.reactions.value().size());

    Add::Reply      reply2 = client.sendMessage(Remove{.name = "thumbsup", .channel = "C09RU2URYMS", .timestamp = reply.message.value().ts});
    if (!reply2.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply2);
    }
    ASSERT_TRUE(reply2.ok);
}

#endif
