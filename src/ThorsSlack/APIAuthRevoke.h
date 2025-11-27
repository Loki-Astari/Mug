#ifndef THORSANVIL_SLACK_API_AUTH_REVOKE_H
#define THORSANVIL_SLACK_API_AUTH_REVOKE_H

#include "ThorsSlackConfig.h"
#include "APIAuthTest.h"

namespace ThorsAnvil::Slack::API::Auth
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/auth.revoke/
Response:
{
    "ok":true,
    "url":"https:\/\/thorsanvilworkspace.slack.com\/",
    "team":"ThorsAnvil",
    "user":"thorsslackbotone",
    "team_id":"T095XJHH589",
    "user_id":"U09S3D8R00Z",
    "bot_id":"B09RJ4A000K",
    "is_enterprise_install":false
}
#endif


struct RevokeReply: public API::Reply
{
};

struct Revoke
{
    static constexpr char const* api = "https://slack.com/api/auth.test";
    static constexpr bool hasBody = false;
    using Reply = AuthInfo;

    OptBool     test;

    std::string query() const {return test.has_value() ? std::string("test=") + (test.value() ? "1":"0") : "";}
};

}
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Auth::RevokeReply);


#endif
