#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatMessage.h"
#include "APIStar.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Star::Add;
using ThorsAnvil::Slack::API::Star::Remove;
using ThorsAnvil::Slack::API::Star::List;

extern SlackClient             client;


#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

class APIStarTest : public ::testing::Test {
    protected:
        static PostMessage::Reply      postNoStar;
        static PostMessage::Reply      postWithStar;
    protected:
    public:
        //static void SetUpTestSuite()
        void SetUp() override
        {
            client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "The APIStarTest::Add message No star"}, postNoStar, true);
            ASSERT_TRUE(postNoStar.ok);
            client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "The APIStarTest::Add message With star"}, postWithStar, true);
            ASSERT_TRUE(postWithStar.ok);

            Add::Reply      replyB;
            client.sendMessage(Add{.channel = "C09RU2URYMS", .timestamp = postWithStar.message.ts}, replyB, true);
            EXPECT_TRUE(replyB.ok);
        }
        //static void TearDownTestSuite()
        void TearDown() override
        {
            client.sendMessage(Delete{.channel = "C09RU2URYMS", .ts = postWithStar.message.ts});
            client.sendMessage(Delete{.channel = "C09RU2URYMS", .ts = postNoStar.message.ts});
        }
};

PostMessage::Reply APIStarTest::postNoStar;
PostMessage::Reply APIStarTest::postWithStar;

TEST_F(APIStarTest, AddStarToNoStar)
{
    Add::Reply      reply;
    client.sendMessage(Add{.channel = "C09RU2URYMS", .timestamp = postNoStar.message.ts}, reply, true);
    ASSERT_TRUE(reply.ok);
}

TEST_F(APIStarTest, AddStarToWithStar)
{
    Add::Reply      reply;
    client.sendMessage(Add{.channel = "C09RU2URYMS", .timestamp = postWithStar.message.ts}, reply);
    ASSERT_FALSE(reply.ok);
}

TEST_F(APIStarTest, RemoveStarFromMessage)
{
    Remove::Reply      reply2;
    client.sendMessage(Remove{.channel = "C09RU2URYMS", .timestamp = postWithStar.message.ts}, reply2, true);
    ASSERT_TRUE(reply2.ok);
}

TEST_F(APIStarTest, RemoveStarFromNoStar)
{
    Remove::Reply      reply2;
    client.sendMessage(Remove{.channel = "C09RU2URYMS", .timestamp = postNoStar.message.ts}, reply2);
    ASSERT_FALSE(reply2.ok);
}

TEST_F(APIStarTest, ListTest)
{
    List::Reply     result;
    client.sendMessage(List{}, result, true);
    ASSERT_TRUE(result.ok);
    ASSERT_NE(0, result.items.size());
}

#endif
