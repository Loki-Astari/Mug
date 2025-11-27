#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatMessage.h"
#include "APIReactions.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Reactions::Get;
using ThorsAnvil::Slack::API::Reactions::Add;
using ThorsAnvil::Slack::API::Reactions::Remove;
using ThorsAnvil::Slack::API::Reactions::List;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

class APIReactionsTest : public ::testing::Test {
    protected:
        static PostMessage::Reply      post;
    protected:
        //void SetUp() override {}
        //void TearDown() override {}
    public:
        static void SetUpTestSuite()
        {
            post = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "The APIReactionsTest::Add message to add reactions to"});
            ASSERT_TRUE(post.ok);
            Add::Reply      replyB = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "stuck_out_tongue_winking_eye", .timestamp = post.message.value().ts});
            if (!replyB.ok) {
                std::cerr << ThorsAnvil::Serialize::jsonExporter(replyB);
            }
            EXPECT_TRUE(replyB.ok);

            Add::Reply      replyC = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "smile", .timestamp = post.message.value().ts});
            if (!replyC.ok) {
                std::cerr << ThorsAnvil::Serialize::jsonExporter(replyC);
            }
            EXPECT_TRUE(replyC.ok);
        }
        static void TearDownTestSuite()
        {
            client.sendMessage(Delete{.channel = "C09RU2URYMS", .ts = post.message.value().ts});
        }
};

PostMessage::Reply APIReactionsTest::post;
TEST_F(APIReactionsTest, AddTest)
{
    Add::Reply      reply = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "thumbsup", .timestamp = post.message.value().ts});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST_F(APIReactionsTest, AddFailTest)
{
    Add::Reply      reply = client.sendMessage(Add{.channel = "C09RU2URYMS", .name = "smile", .timestamp = post.message.value().ts});
    EXPECT_FALSE(reply.ok); // There is already a smile up reaction from this bot.
}

TEST_F(APIReactionsTest, ListTest)
{
    List::Reply     result = client.sendMessage(List{});
    if (!result.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(result);
    }
    ASSERT_TRUE(result.ok);
    ASSERT_NE(0, result.items.size());
}

TEST_F(APIReactionsTest, GetTest)
{
    Get::Reply     get = client.sendMessage(Get{.channel = "C09RU2URYMS", .timestamp = post.message.value().ts});
    if (!get.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(get);
    }
    ASSERT_TRUE(get.ok);
    ASSERT_EQ(3, get.message.reactions.value().size());
}

TEST_F(APIReactionsTest, RemoveTest)
{
    Add::Reply      reply2 = client.sendMessage(Remove{.name = "smile", .channel = "C09RU2URYMS", .timestamp = post.message.value().ts});
    if (!reply2.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply2);
    }
    ASSERT_TRUE(reply2.ok);
}

TEST_F(APIReactionsTest, RemoveFailTest)
{
    Add::Reply      reply2 = client.sendMessage(Remove{.name = "-1", .channel = "C09RU2URYMS", .timestamp = post.message.value().ts});
    ASSERT_FALSE(reply2.ok);
}

#endif
