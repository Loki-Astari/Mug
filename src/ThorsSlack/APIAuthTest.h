#ifndef THORSANVIL_SLACK_API_AUTH_TEST_H
#define THORSANVIL_SLACK_API_AUTH_TEST_H

#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Auth
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/auth.test/
Response:
{
     "ok":       true,
     "url":      "https:\/\/thorsanvilworkspace.slack.com\/",
     "team":     "ThorsAnvil",
     "user":     "thorsslackbotone",
     "team_id":  "T095XJHH589",
     "user_id":  "U09S3D8R00Z",
     "bot_id":   "B09RJ4A000K",
     "is_enterprise_install":false
}
#endif


struct AuthInfo: public API::Reply
{
    std::string             url;
    std::string             team;
    std::string             user;
    std::string             team_id;
    std::string             user_id;
    std::string             bot_id;
    bool                    is_enterprise_install;
};

struct Test
{
    static constexpr char const* api = "https://slack.com/api/auth.test";
    static constexpr bool hasBody = false;
    using Reply = AuthInfo;

    std::string query() const {return "";}
};

}
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Auth::AuthInfo, url, team, user, team_id, user_id, bot_id, is_enterprise_install);


#endif
