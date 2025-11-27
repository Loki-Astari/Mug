#include "ThorsSlackConfig.h"
#include "ThorSerialize/JsonThor.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIChatMessage.h"
#include "APIPins.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;
using ThorsAnvil::Slack::API::Chat::Delete;
using ThorsAnvil::Slack::API::Pins::Add;
using ThorsAnvil::Slack::API::Pins::List;
using ThorsAnvil::Slack::API::Pins::Remove;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

class APIPinsTest : public ::testing::Test {
    protected:
        static PostMessage::Reply      post;
    protected:
        //void SetUp() override {}
        //void TearDown() override {}
    public:
        static void SetUpTestSuite()
        {
            post = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "The APIPinTest::Add message to add pins to"});
            ASSERT_TRUE(post.ok);
        }
        static void TearDownTestSuite()
        {
            client.sendMessage(Delete{.channel = "C09RU2URYMS", .ts = post.message.value().ts});
        }
};
PostMessage::Reply APIPinsTest::post;

TEST_F(APIPinsTest, Add)
{
    Add::Reply      reply = client.sendMessage(Add{.channel = "C09RU2URYMS", .timestamp=post.message.value().ts});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST_F(APIPinsTest, List)
{
    List::Reply      reply = client.sendMessage(List{.channel = "C09RU2URYMS"});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_NE(0, reply.items.size());
    bool findPin = false;
    for (auto const& item: reply.items) {
        if (item.message.ts == post.message.value().ts) {
            findPin = true;
        }
    }
    ASSERT_TRUE(findPin);
}

TEST_F(APIPinsTest, Remove)
{
    List::Reply      reply = client.sendMessage(List{.channel = "C09RU2URYMS"});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);

    int pinRemoved = 0;
    for (auto const& pin: reply.items) {

        Remove::Reply r = client.sendMessage(Remove{.channel = pin.channel, .timestamp = pin.message.ts});
        if (!reply.ok) {
            std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
        }
        EXPECT_TRUE(r.ok);
        ++pinRemoved;
    }
    ASSERT_NE(0, pinRemoved);
}

#endif
