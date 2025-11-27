#ifndef THORSANVIL_SLACK_API_REACTIONS_REMOVE_H
#define THORSANVIL_SLACK_API_REACTIONS_REMOVE_H


#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Reactions
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/reactions.remove/
Response:
{"ok":true}
#endif

struct Remove
{
    static constexpr char const* api = "https://slack.com/api/reactions.remove";
    static constexpr bool hasBody = true;
    using Reply = API::Reply;

    std::string             name;               // Reaction (emoji) name.
    OptString               file;               // File to remove reaction from.
    OptString               file_comment;       // File comment to remove reaction from.
    OptString               channel;            // Channel where the message to remove reaction from was posted.
    OptString               timestamp;          // Timestamp of the message to remove reaction from
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reactions::Remove, name, file, file_comment, channel, timestamp);

#endif
