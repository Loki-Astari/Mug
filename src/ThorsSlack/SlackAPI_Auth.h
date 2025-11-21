#ifndef THORSANVIL_SLACK_API_AUTH_H
#define THORSANVIL_SLACK_API_AUTH_H

#include "ThorsSlackConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <string>

namespace ThorsAnvil::Slack::API::Auth
{

// Documentation: https://docs.slack.dev/reference/methods/auth.test/
// Example Reply:
//  {
//      "ok":       true,
//      "url":      "https:\/\/thorsanvilworkspace.slack.com\/",
//      "team":     "ThorsAnvil",
//      "user":     "thorsslackbotone",
//      "team_id":  "T095XJHH589",
//      "user_id":  "U09S3D8R00Z",
//      "bot_id":   "B09RJ4A000K",
//      "is_enterprise_install":false
//  }


struct TestReply
{
    bool                    ok;
    std::string             url;
    std::string             team;
    std::string             user;
    std::string             team_id;
    std::string             user_id;
    std::string             bot_id;
    std::string             error;
};
struct Test
{
    static constexpr char const* api = "https://slack.com/api/auth.test";
    static constexpr bool hasBody = false;
    using Reply = TestReply;
};

}
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TestReply, ok, url, team, user, team_id, user_id, bot_id, error);


#endif
