#ifndef THORSANVIL_SLACK_API_H
#define THORSANVIL_SLACK_API_H

#include "ThorsSlackConfig.h"
#include "SlackBlockKit.h"
#include "NisseHTTP/Util.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <sstream>
#include <string>
#include <vector>
#include <optional>

namespace ThorsAnvil::Slack::API
{

using OptBool       = std::optional<bool>;
using OptInt        = std::optional<int>;
using OptString     = std::optional<std::string>;
using VecString     = std::vector<std::string>;
using OptVecString  = std::optional<std::vector<std::string>>;
using Method        = ThorsAnvil::Nisse::HTTP::Method;

struct BotIcon
{
    std::string                 image_36;
    std::string                 image_48;
    std::string                 image_72;
};

struct BotProfile
{
    std::string                 id;
    std::string                 app_id;
    std::string                 user_id;
    std::string                 name;
    BotIcon                     icons;
    bool                        deleted;
    std::time_t                 updated;
    std::string                 team_id;
};

struct Reaction
{
    std::string                 name;
    VecString                   users;
    int                         count;
};
using VecReaction = std::vector<Reaction>;
using OptVecReaction = std::optional<VecReaction>;

struct Message
{
    std::string                 user;
    std::string                 type;
    std::string                 ts;
    std::string                 bot_id;
    std::string                 app_id;
    std::string                 text;
    std::string                 team;
    BotProfile                  bot_profile;
    BlockKit::Blocks            blocks;
    OptVecString                pinned_to;
    OptString                   permalink;
    OptVecReaction              reactions;
};
using OptMessage = std::optional<Message>;

struct ResponseMetaData
{
    VecString                   messages;
};
using OptResponseMetaData = std::optional<ResponseMetaData>;

struct Reply
{
    bool                        ok;
    OptString                   error;
    OptVecString                errors;
    OptString                   warning;
    OptVecString                warnings;
    OptResponseMetaData         response_metadata;
};

struct OK
{
    bool                        ok;
    ThorsAnvil_VariantSerializer(ThorsAnvil::Slack::API::OK);
};

struct Error
{
    bool                        ok;
    std::string                 error;
    std::vector<std::string>    errors;
    std::string                 warning;
    std::vector<std::string>    warnings;
    ResponseMetaData            response_metadata;
    ThorsAnvil_VariantSerializer(ThorsAnvil::Slack::API::Error);
};

struct Cursor
{
    std::string                         next_cursor;
};

// Primary template: By default, a type is not std::optional
template<typename>
constexpr bool is_optional_impl = false;

// Partial specialization for std::optional<T>: This will be chosen when the type is std::optional
template<typename T>
constexpr bool is_optional_impl<std::optional<T>> = true;

// Helper variable template to handle cv-qualifiers and references
template<typename T>
constexpr bool is_optional = is_optional_impl<std::remove_cvref_t<T>>; // C++20 and later

template<typename Arg>
void addQueryParam(std::stringstream& stream, std::string& sep, Arg const& arg)
{
    if constexpr (is_optional<typename std::tuple_element<1, Arg>::type>) {
        if (std::get<1>(arg).has_value()) {
            stream << sep << std::get<0>(arg) << "=" << std::get<1>(arg).value();
            sep = "&";
        }
    }
    else {
        stream << sep << std::get<0>(arg) << "=" << std::get<1>(arg);
        sep = "&";
    }
}
template<typename... Args>
std::string buildQuery(Args const&... arg)
{
    std::stringstream   query;
    std::string         sep = "";

    (addQueryParam(query, sep, arg), ...);
    return query.str();
}


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reaction, name, users, count);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks, pinned_to, permalink, reactions);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::ResponseMetaData, messages);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Reply, ok, error, errors, warning, warnings, response_metadata);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::OK, ok);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Error, ok, error, errors, warning, warnings, response_metadata);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Cursor, next_cursor);


#endif
