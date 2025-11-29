#ifndef THORSANVIL_SLACK_API_REACTIONS_H
#define THORSANVIL_SLACK_API_REACTIONS_H


#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Reactions
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/reactions.add/
Response:
{"ok":true}
Documentation: https://docs.slack.dev/reference/methods/reactions.get/
Response:
{
    "ok":true,
    "type":"message",
    "message":
    {
        "user":"U09S3D8R00Z",
        "type":"message",
        "ts":"1764267968.082039",
        "bot_id":"B09RJ4A000K",
        "app_id":"A09RQFXSEKC",
        "text":"Will add a reaction to this",
        "team":"T095XJHH589",
        "bot_profile":
        {
            "id":"B09RJ4A000K",
            "deleted":false,
            "name":"ThorsSlackBotOne",
            "updated":1762644664,
            "app_id":"A09RQFXSEKC",
            "user_id":"U09S3D8R00Z",
            "icons":
            {
                "image_36":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/bot_36.png",
                "image_48":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/bot_48.png",
                "image_72":"https:\/\/a.slack-edge.com\/80588\/img\/plugins\/app\/service_72.png"
            },
            "team_id":"T095XJHH589"
        },
        "blocks":[
            {
                "type":"rich_text",
                "block_id":"LmmqG",
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
                "name":"stuck_out_tongue_winking_eye",
                "users":["U09S3D8R00Z"],
                "count":1
            },
            {
                "name":"smile",
                "users":["U09S3D8R00Z"],
                "count":1
            }
        ],
        "permalink":"https:\/\/thorsanvilworkspace.slack.com\/archives\/C09RU2URYMS\/p1764267968082039"
    },
    "channel":"C09RU2URYMS"
}
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
Documentation: https://docs.slack.dev/reference/methods/reactions.remove/
Response:
{"ok":true}
#endif

// Response Objects
struct GetReply
{
    bool                        ok      = false;
    std::string                 type;
    API::Message                message;
    std::string                 channel;
    ThorsAnvil_VariantSerializer(ThorsAnvil::Slack::API::Reaction::GetReply);
};

struct ReactionMessage
{
    std::string                 type;
    std::string                 channel;
    API::Message                message;
};
using VecReactionMessage = std::vector<ReactionMessage>;

struct ListReply
{
    bool                        ok      = false;
    VecReactionMessage          items;
    Cursor                      response_metadata;
    ThorsAnvil_VariantSerializer(ThorsAnvil::Slack::API::Reaction::ListReply);
};

// Action Objects
struct Add
{
    static constexpr char const* api = "https://slack.com/api/reactions.add";
    static constexpr Method method = Method::POST;
    using Reply = API::OK;

    std::string                 channel;            // Channel where the message to add reaction to was posted.
    std::string                 name;               // Reaction (emoji) name
    std::string                 timestamp;          // Timestamp of the message to add reaction to.
};

struct Get
{
    static constexpr char const* api = "https://slack.com/api/reactions.get";
    static constexpr Method method = Method::GET;
    using Reply = GetReply;

    OptString                   channel;        // Channel where the message to get reactions for was posted.
    OptString                   file;           // File to get reactions for.
    OptString                   file_comment;   // File comment to get reactions for.
    OptBool                     full;           // If true always return the complete reaction list.
    OptString                   timestamp;      // Timestamp of the message to get reactions for.
};

struct List
{
    static constexpr char const* api = "https://slack.com/api/reactions.list";
    static constexpr Method method = Method::GET;
    using Reply = ListReply;

    OptString                   user;               // Show reactions made by this user. Defaults to the authed user.
    OptBool                     full;               // If true always return the complete reaction list.
    OptInt                      count;              // Default: 100
    OptInt                      page;               // Default: 1
    OptString                   cursor;             // Parameter for pagination. Set cursor equal to the next_cursor attribute returned by the previous request's response_metadata. This parameter is optional, but pagination is mandatory: the default value simply fetches the first "page" of the collection. See pagination for more details.
    OptInt                      limit;              // The maximum number of items to return. Fewer than the requested number of items may be returned, even if the end of the list hasn't been reached.
    OptString                   team_id;            // encoded team id to list reactions in, required if org token is used
};

struct Remove
{
    static constexpr char const* api = "https://slack.com/api/reactions.remove";
    static constexpr Method method = Method::POST;
    using Reply = API::OK;

    std::string                 name;               // Reaction (emoji) name.
    OptString                   file;               // File to remove reaction from.
    OptString                   file_comment;       // File comment to remove reaction from.
    OptString                   channel;            // Channel where the message to remove reaction from was posted.
    OptString                   timestamp;          // Timestamp of the message to remove reaction from
};

}

// Response objects
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::ReactionMessage, type, channel, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::GetReply, ok, type, message, channel);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::ListReply, ok, items, response_metadata);

// Action objects
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::Get, channel, file, file_comment, full, timestamp);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::Add, channel, name, timestamp);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::Remove, name, file, file_comment, channel, timestamp);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::List, user, full, count, page, cursor, limit, team_id);

#endif
