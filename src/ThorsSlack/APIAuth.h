#ifndef THORSANVIL_SLACK_API_AUTH_H
#define THORSANVIL_SLACK_API_AUTH_H

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
Documentation: https://docs.slack.dev/reference/methods/auth.teams.list/
Response:
{
    "ok":true,
    "teams":[
        {
            "id":"T095XJHH589",
            "name":"ThorsAnvil"
        }
    ]
}
#endif

// Response Objects
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

struct TeamInfo
{
    std::string             id;
    std::string             name;
};
using VecTeamInfo = std::vector<TeamInfo>;

struct TeamsListReply: public API::Reply
{
    VecTeamInfo     teams;
};


// Action Objects
struct Test
{
    static constexpr char const* api = "https://slack.com/api/auth.test";
    static constexpr bool hasBody = false;
    using Reply = AuthInfo;

    std::string query() const {return "";}
};

struct Revoke
{
    static constexpr char const* api = "https://slack.com/api/auth.test";
    static constexpr bool hasBody = false;
    using Reply = AuthInfo;

    OptBool     test;

    std::string query() const {return test.has_value() ? std::string("test=") + (test.value() ? "1":"0") : "";}
};

struct TeamsList
{
    static constexpr char const* api = "https://slack.com/api/auth.teams.list";
    static constexpr bool hasBody = true;
    using Reply = TeamsListReply;

    int             limit;          // The maximum number of workspaces to return. Must be a positive integer no larger than 1000.
    OptString       cursor;         // Set cursor to next_cursor returned by the previous call to list items in the next page.
    OptBool         include_icon;   // Whether to return icon paths for each workspace. An icon path represents a URI pointing to the image signifying the workspace.
};

}

// Response objects
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TeamInfo, id, name);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Auth::TeamsListReply, teams);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Auth::AuthInfo, url, team, user, team_id, user_id, bot_id, is_enterprise_install);
// Action objects
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TeamsList, limit, cursor, include_icon);

#endif
