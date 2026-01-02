#ifndef THORSANVIL_SLACK_API_CONVERSATION_HISTORY_H
#define THORSANVIL_SLACK_API_CONVERSATION_HISTORY_H

#include "ThorsSlackConfig.h"
#include "API.h"

namespace ThorsAnvil::Slack::API::Conversation
{

// Action Objects
struct History
{
    static constexpr char const* api = "https://slack.com/api/conversations.history";
    static constexpr Method method = Method::GET;
    static constexpr Scope  scope = Scope::Bot;
    using Reply = OK;

    std::string         channel;                // Conversation ID to fetch history for.
    OptString           cursor;                 // Paginate through collections of data by setting the cursor parameter to a next_cursor attribute returned by a previous request's response_metadata.
                                                // Default value fetches the first "page" of the collection. See pagination for more detail.
    OptBool             include_all_metadata;   // Return all metadata associated with this message.
    OptBool             inclusive;              // Include messages with oldest or latest timestamps in results. Ignored unless either timestamp is specified.
    OptString           latest;                 // Only messages before this Unix timestamp will be included in results.
                                                // Default is the current time.
    OptString           oldest;                 // Only messages after this Unix timestamp will be included in results.
                                                // Default: 0
    OptInt              limit;                  // The maximum number of items to return.
                                                // Fewer than the requested number of items may be returned, even if the end of the conversation history hasn't been reached.
                                                // Maximum of 999.
};

}

// Action Objects
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Conversation::History, channel, cursor, include_all_metadata, inclusive, latest, oldest, limit);

#endif
