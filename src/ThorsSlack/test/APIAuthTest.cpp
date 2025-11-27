#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIAuthTest.h"
#include "APIAuthTeamsList.h"
#include "APIAuthRevoke.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Auth::Test;
using ThorsAnvil::Slack::API::Auth::TeamsList;
using ThorsAnvil::Slack::API::Auth::Revoke;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIAuthTests, AuthTest)
{
    ThorsAnvil::Slack::API::Auth::Test::Reply      reply = client.sendMessage(ThorsAnvil::Slack::API::Auth::Test{});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(APIAuthTests, AuthTeamsList)
{
    TeamsList::Reply      reply = client.sendMessage(TeamsList{});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(APIAuthTests, AuthRevoke)
{
    Revoke::Reply      reply = client.sendMessage(Revoke{.test = true});
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

#endif
