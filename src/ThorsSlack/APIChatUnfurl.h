#ifndef THORSANVIL_SLACK_API_CHAT_UNFURL_H
#define THORSANVIL_SLACK_API_CHAT_UNFURL_H


#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
#include "APIChat.h"

namespace BK = ThorsAnvil::Slack::BlockKit;

namespace ThorsAnvil::Slack::API::Chat
{

#if 0
Documentation: https://docs.slack.dev/reference/methods/chat.unfurl/
// TODO.
// This code does not work.
// The "unfurls" parameter needs to be URL encoded for some reason.
// Need a working example of this in normal code to study its affects.
#endif

struct UnfurlReply: public API::Reply
{
};

struct UnfurlBlock
{
    BK::Blocks          blocks;
};

using UnfurlURL = std::map<std::string, UnfurlBlock>;
using OptUnfurlURL = std::optional<UnfurlURL>;
struct Unfurl
{
    static constexpr char const* api = "https://slack.com/api/chat.unfurl";
    static constexpr bool hasBody = true;
    using Reply = UnfurlReply;

    OptString           channel;            // Channel ID of the message. Both channel and ts must be provided together, or unfurl_id and source must be provided together.
    OptString           ts;                 // Timestamp of the message to add unfurl behavior to.
    OptUnfurlURL        unfurls;            // URL-encoded JSON map with keys set to URLs featured in the the message, pointing to their unfurl blocks or message attachments. Either unfurls or metadata must be provided.
    OptString           user_auth_message;  // Provide a simply-formatted string to send as an ephemeral message to the user as invitation to authenticate further and enable full unfurling behavior. Provides two buttons, Not now or Never ask me again.
    OptBool             user_auth_required; // Set to true or 1 to indicate the user must install your Slack app to trigger unfurls for this domain
    OptString           user_auth_url;      // Send users to this custom URL where they will complete authentication in your app to fully trigger unfurling. Value should be properly URL-encoded.
    BlockKit::OptBlocks user_auth_blocks;   // Provide a JSON based array of structured blocks presented as URL-encoded string to send as an ephemeral message to the user as invitation to authenticate further and enable full unfurling behavior
    OptString           unfurl_id;          // The ID of the link to unfurl. Both unfurl_id and source must be provided together, or channel and ts must be provided together.
    OptString           source;             // The source of the link to unfurl. The source may either be composer, when the link is inside the message composer, or conversations_history, when the link has been posted to a conversation.
    OptMetadata         metadata;           // JSON object with entity_type and entity_payload fields, presented as a URL-encoded string. Either unfurls or metadata must be provided.
};

}

ThorsAnvil_ExpandTrait(ThorsAnvil::Slack::API::Reply, ThorsAnvil::Slack::API::Chat::UnfurlReply);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::UnfurlBlock, blocks);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Unfurl, channel, ts, unfurls, user_auth_message, user_auth_required, user_auth_url, user_auth_blocks, unfurl_id, source, metadata);

#endif
