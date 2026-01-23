#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#if !(defined(DISABLE_SLACKTEST) && (DISABLE_SLACKTEST == 1))

#include "SlackClient.h"
#include "Environment.h"
#include "APIChatMessage.h"
#include "APIPins.h"

#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::POSTMessage;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Pins::Add;
using ThorsAnvil::Slack::API::Pins::List;
using ThorsAnvil::Slack::API::Pins::Remove;

extern SlackClient             client;
extern Environment             environment;

class APIPinsTest : public ::testing::Test {
    protected:
        static POSTMessage::Reply      post;
    protected:
        //void SetUp() override {}
        //void TearDown() override {}
    public:
        static void SetUpTestSuite()
        {
            client.sendMessage(POSTMessage{.channel = environment.slackChannel, .text = "The APIPinTest::Add message to add pins to"}, post, true);
            ASSERT_TRUE(post.ok);
        }
        static void TearDownTestSuite()
        {
            client.sendMessage(Delete{.channel = environment.slackChannel, .ts = post.message.ts});
        }
};
POSTMessage::Reply APIPinsTest::post;

TEST_F(APIPinsTest, Add)
{
    Add::Reply      reply;
    client.sendMessage(Add{.channel = environment.slackChannel, .timestamp=post.message.ts}, reply, true);
    ASSERT_TRUE(reply.ok);
}

TEST_F(APIPinsTest, List)
{
    List::Reply      reply;
    client.sendMessage(List{.channel = environment.slackChannel}, reply, true);
    ASSERT_TRUE(reply.ok);
    ASSERT_NE(0, reply.items.size());
    bool findPin = false;
    for (auto const& item: reply.items) {
        if (item.message.ts == post.message.ts) {
            findPin = true;
        }
    }
    ASSERT_TRUE(findPin);
}

TEST_F(APIPinsTest, Remove)
{
    List::Reply      reply;
    client.sendMessage(List{.channel = environment.slackChannel}, reply, true);
    ASSERT_TRUE(reply.ok);

    int pinRemoved = 0;
    for (auto const& pin: reply.items) {

        Remove::Reply r;
        client.sendMessage(Remove{.channel = pin.channel, .timestamp = pin.message.ts}, r, true);
        EXPECT_TRUE(r.ok);
        ++pinRemoved;
    }
    ASSERT_NE(0, pinRemoved);
}

#endif
