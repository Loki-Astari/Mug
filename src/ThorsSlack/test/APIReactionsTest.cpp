#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#if !(defined(DISABLE_SLACKTEST) && (DISABLE_SLACKTEST == 1))

#include "SlackClient.h"
#include "Environment.h"
#include "APIChatMessage.h"
#include "APIReactions.h"

#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Reactions::Get;
using ThorsAnvil::Slack::API::Reactions::Add;
using ThorsAnvil::Slack::API::Reactions::Remove;
using ThorsAnvil::Slack::API::Reactions::List;

extern SlackClient             client;
extern Environment             environment;

class APIReactionsTest : public ::testing::Test {
    protected:
        static PostMessage::Reply      post;
    protected:
        //void SetUp() override {}
        //void TearDown() override {}
    public:
        static void SetUpTestSuite()
        {
            client.sendMessage(PostMessage{.channel = environment.slackChannel, .text = "The APIReactionsTest::Add message to add reactions to"}, post, true);
            ASSERT_TRUE(post.ok);
            Add::Reply      replyB;
            client.sendMessage(Add{.channel = environment.slackChannel, .name = "stuck_out_tongue_winking_eye", .timestamp = post.message.ts}, replyB, true);
            EXPECT_TRUE(replyB.ok);

            Add::Reply      replyC;
            client.sendMessage(Add{.channel = environment.slackChannel, .name = "smile", .timestamp = post.message.ts}, replyC, true);
            EXPECT_TRUE(replyC.ok);
        }
        static void TearDownTestSuite()
        {
            client.sendMessage(Delete{.channel = environment.slackChannel, .ts = post.message.ts});
        }
};

PostMessage::Reply APIReactionsTest::post;
TEST_F(APIReactionsTest, AddTest)
{
    Add::Reply      reply;
    client.sendMessage(Add{.channel = environment.slackChannel, .name = "thumbsup", .timestamp = post.message.ts}, reply, true);
    ASSERT_TRUE(reply.ok);
}

TEST_F(APIReactionsTest, AddFailTest)
{
    Add::Reply      reply;
    client.sendMessage(Add{.channel = environment.slackChannel, .name = "smile", .timestamp = post.message.ts}, reply);
    EXPECT_FALSE(reply.ok); // There is already a smile up reaction from this bot.
}

TEST_F(APIReactionsTest, ListTest)
{
    List::Reply     result;
    client.sendMessage(List{}, result, true);
    ASSERT_TRUE(result.ok);
    ASSERT_NE(0, result.items.size());
}

TEST_F(APIReactionsTest, GetTest)
{
    Get::Reply     get;
    client.sendMessage(Get{.channel = environment.slackChannel, .timestamp = post.message.ts}, get, true);
    ASSERT_TRUE(get.ok);
    ASSERT_EQ(3, get.message.reactions.value().size());
}

TEST_F(APIReactionsTest, RemoveTest)
{
    Add::Reply      reply2;
    client.sendMessage(Remove{.name = "smile", .channel = environment.slackChannel, .timestamp = post.message.ts}, reply2, true);
    ASSERT_TRUE(reply2.ok);
}

TEST_F(APIReactionsTest, RemoveFailTest)
{
    Add::Reply      reply2;
    client.sendMessage(Remove{.name = "-1", .channel = environment.slackChannel, .timestamp = post.message.ts}, reply2);
    ASSERT_FALSE(reply2.ok);
}

#endif
