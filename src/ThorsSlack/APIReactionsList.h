#ifndef THORSANVIL_SLACK_API_REACTIONS_LIST_H
#define THORSANVIL_SLACK_API_REACTIONS_LIST_H


#include "ThorSerialize/Traits.h"
#include "ThorsSlackConfig.h"
#include "API.h"
#include <string>

namespace ThorsAnvil::Slack::API::Reactions
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/reactions.list/
Response:
{
    "ok":true,
    "items":[
        {
            "type":"message",
            "channel":"C09RU2URYMS",
            "message":
            {
                "user":"U09S3D8R00Z",
                "type":"message",
                "ts":"1764260011.353119",
                "bot_id":"B09RJ4A000K",
                "app_id":"A09RQFXSEKC",
                "text":"Will add a reaction to this",
                "team":"T095XJHH589",
                "bot_profile":
                {
                    "id":"B09RJ4A000K",
                    "app_id":"A09RQFXSEKC",
                    "user_id":"U09S3D8R00Z",
                    "name":"ThorsSlackBotOne",
                    "icons":
                    {
                        "image_36":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/bot_36.png",
                        "image_48":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/bot_48.png",
                        "image_72":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/service_72.png"
                    },
                    "deleted":false,
                    "updated":1762644664,
                    "team_id":"T095XJHH589"
                },
                "blocks":[
                    {
                        "type":"rich_text",
                        "block_id":"740mo",
                        "elements":[
                            {
                                "type":"rich_text_section",
                                "elements":[
                                    {
                                        "type":"text",
                                        "text":"Will add a reaction to this"
                                    }
                                ]
                            }
                        ]
                    }
                ],
                "reactions":[
                    {
                        "name":"+1",
                        "users":["U09S3D8R00Z"],
                        "count":1
                    },
                    {
                        "name":"-1",
                        "users":["U09S3D8R00Z"],
                        "count":1
                    },
                    {"name":"smile","users":["U09S3D8R00Z"],"count":1}
                ],
                "permalink":"https:\/\/thorsanvilworkspace.slack.com\/archives\/C09RU2URYMS\/p1764260011353119"
            }
        }
        ...
    ],
    "response_metadata":
    {
        "next_cursor":""
    }
}
#endif

struct ReactionMessage
{
    std::string         type;
    std::string         channel;
    API::Message        message;
};
using VecReactionMessage = std::vector<ReactionMessage>;

struct ListReply: public API::Reply
{
    VecReactionMessage  items;
    Cursor              response_metadata;
};

struct List
{
    static constexpr char const* api = "https://slack.com/api/reactions.list";
    static constexpr Method method = Method::GET;
    using Reply = ListReply;

    OptString           user;               // Show reactions made by this user. Defaults to the authed user.
    OptBool             full;               // If true always return the complete reaction list.
    OptInt              count;              // Default: 100
    OptInt              page;               // Default: 1
    OptString           cursor;             // Parameter for pagination. Set cursor equal to the next_cursor attribute returned by the previous request's response_metadata. This parameter is optional, but pagination is mandatory: the default value simply fetches the first "page" of the collection. See pagination for more details.
    OptInt              limit;              // The maximum number of items to return. Fewer than the requested number of items may be returned, even if the end of the list hasn't been reached.
    OptString           team_id;            // encoded team id to list reactions in, required if org token is used

    std::string query() const
    {
        return buildQuery(std::tie("user", user),
                          std::tie("full", full),
                          std::tie("count", count),
                          std::tie("page", page),
                          std::tie("cursor", cursor),
                          std::tie("limit", limit),
                          std::tie("team_id", team_id));
    }
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::ReactionMessage, type, channel, message);
ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Reactions::ListReply, items, response_metadata);


#endif
