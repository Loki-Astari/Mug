#ifndef THORSANVIL_SLACK_API_REACTIONS_GET_H
#define THORSANVIL_SLACK_API_REACTIONS_GET_H


#include "ThorSerialize/Traits.h"
#include "ThorsSlackConfig.h"
#include "API.h"
#include <string>

namespace ThorsAnvil::Slack::API::Reactions
{

#if 0
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
#endif

struct GetReply: public API::Reply
{
    std::string     type;
    API::Message    message;
    std::string     channel;
};

struct Get
{
    static constexpr char const* api = "https://slack.com/api/reactions.get";
    static constexpr bool hasBody = false;
    using Reply = GetReply;

    OptString       channel;        // Channel where the message to get reactions for was posted.
    OptString       file;           // File to get reactions for.
    OptString       file_comment;   // File comment to get reactions for.
    OptBool         full;           // If true always return the complete reaction list.
    OptString       timestamp;      // Timestamp of the message to get reactions for.


    std::string     query() const
    {
        std::stringstream   result;
        std::string         sep = "&";

        if (channel.has_value()) {
            result << sep << "channel=" << channel.value();
            sep = "&";
        }
        if (file.has_value()) {
            result << sep << "file=" << file.value();
            sep = "&";
        }
        if (file_comment.has_value()) {
            result << sep << "file_comment=" << file_comment.value();
            sep = "&";
        }
        if (full.has_value()) {
            result << sep << "full=" << full.value();
            sep = "&";
        }
        if (timestamp.has_value()) {
            result << sep << "timestamp=" << timestamp.value();
            sep = "&";
        }
        return result.str();
    }
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Reactions::GetReply, type, message, channel);

#endif
