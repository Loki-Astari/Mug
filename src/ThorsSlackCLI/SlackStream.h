#ifndef THORSANVIL_SLACK_SLACKSTREAM_H
#define THORSANVIL_SLACK_SLACKSTREAM_H

#include "SlackCLIConfig.h"
#include "HTTPSStream.h"
#include "ThorSerialize/Traits.h"

#include <string>
#include <ostream>

namespace ThorsAnvil::Slack
{

class SlackStream: public ThorsAnvil::Nisse::HTTP::HTTPSStream
{
    std::string     authToken;

    public:
        SlackStream(std::string authToken);
        friend std::ostream& operator<<(SlackStream& stream, ThorsAnvil::Nisse::HTTP::HTTPRequest const& request);
};

struct PostMessageData
{
    std::string     channel;
    std::string     text;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::PostMessageData, channel, text);

#endif
