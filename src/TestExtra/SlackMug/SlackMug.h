#ifndef THORSANVIL_EXAMPLE_SLACKBOT_SLACKBOT_H
#define THORSANVIL_EXAMPLE_SLACKBOT_SLACKBOT_H

#include "SlackMugConfig.h"
#include "SlackEventHandler.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"

#include <string>
#include <map>

class SlackMug: public ThorsAnvil::ThorsMug::MugPluginSimple
{
        ThorsAnvil::Slack::SlackClient          client;
        SlackEventHandler                       eventHandler;
        std::map<std::string, int>              messageCount;


        void handleCommand(ThorsAnvil::Nisse::HTTP::Request const& request, ThorsAnvil::Nisse::HTTP::Response& response);

    public:
        SlackMug(char const* config);

        virtual std::vector<ThorsAnvil::ThorsMug::Action> getAction() override;
};

#endif
