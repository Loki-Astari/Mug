#ifndef THORSANVIL_SLACK_API_AUTH_TEAMS_LIST_H
#define THORSANVIL_SLACK_API_AUTH_TEAMS_LIST_H

#include "ThorsSlackConfig.h"
#include "API.h"
#include <string>

namespace ThorsAnvil::Slack::API::Auth
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/auth.teams.list/
Response:
{"ok":true,"teams":[{"id":"T095XJHH589","name":"ThorsAnvil"}]}
#endif

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

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TeamInfo, id, name);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Auth::TeamsListReply, teams);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TeamsList, limit, cursor, include_icon);


#endif
