#include "ThorsSlackConfig.h"
#include "gtest/gtest.h"

#include "SlackClient.h"
#include "APIAuth.h"

using namespace std::literals::string_literals;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Auth::Test;
using ThorsAnvil::Slack::API::Auth::TeamsList;
using ThorsAnvil::Slack::API::Auth::Revoke;

extern SlackClient             client;

#if !(defined(DISABLE_TEST) && (DISABLE_TEST == 1))

TEST(APIAuthTests, AuthTest)
{
    ThorsAnvil::Slack::API::Auth::Test::Reply      reply;
    client.sendMessage(ThorsAnvil::Slack::API::Auth::Test{}, reply, true);
    ASSERT_TRUE(reply.ok);
}

TEST(APIAuthTests, AuthTeamsList)
{
    TeamsList::Reply      reply;
    client.sendMessage(TeamsList{}, reply, true);
    ASSERT_TRUE(reply.ok);
    EXPECT_NE(0, reply.teams.size());   // At least your current team (may be more).
}

TEST(APIAuthTests, AuthRevoke)
{
    Revoke::Reply      reply;
    client.sendMessage(Revoke{.test = true}, reply, true);
    ASSERT_TRUE(reply.ok);
}

#endif
