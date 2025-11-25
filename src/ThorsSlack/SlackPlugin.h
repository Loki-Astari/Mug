#ifndef THORSANVIL_SLACK_SLACKPLUGIN_H
#define THORSANVIL_SLACK_SLACKPLUGIN_H

#include "ThorsSlackConfig.h"
#include "EventCallback.h"
#include "EventCallbackMessage.h"
#include "EventURLVerification.h"

#include "ThorsChalice/DLLib.h"
#include "ThorsSlack/EventCallbackReactionAdded.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include <string>

namespace ThorsAnvil::ThorsSlack
{

class SlackPlugin: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
        std::string  slackToken;
    public:
        SlackPlugin(std::string const& slackToken);

    private:
        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        std::string getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response, bool& found);

        void handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event);

        void handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event);
        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& event)             {}
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& event) {}


        struct VisitorEvent
        {
            SlackPlugin&                        plugin;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::EventURLVerification const& event)    {plugin.handleURLVerificationEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::EventCallback const& event)           {plugin.handleCallbackEvent(request, response, event);}
        };
        struct VisitorCallbackEvent
        {
            SlackPlugin&                        plugin;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::Message const& event)                 {plugin.handleCallbackMessageEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::ReactionAdded const& event)           {plugin.handleCallbackReactionAddedEvent(request, response, event);}
        };
};

}

#endif
