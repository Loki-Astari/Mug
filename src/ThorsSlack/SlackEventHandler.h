#ifndef THORSANVIL_SLACK_SLACKEVENTHANDLER_H
#define THORSANVIL_SLACK_SLACKEVENTHANDLER_H

#include "ThorsSlackConfig.h"
#include "APIBlockActions.h"
#include "Event.h"
#include "EventCallback.h"
#include "EventCallbackMessage.h"
#include "EventCallbackReaction.h"
#include "EventURLVerification.h"

#include "ThorsMug/MugPlugin.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorsCrypto/hmac.h"
#include "ThorSerialize/Logging.h"

#include <string>
#include <string_view>
#include <variant>
#include <algorithm>
#include <algorithm>
#include <cmath>
#include <ctime>

#if defined(DEBUG) && (DEBUG != 0)
#incude <iostream>
#endif

namespace ThorsAnvil::Slack
{

class SlackEventHandler
{
        std::string_view  slackSecret;
    public:
        SlackEventHandler(std::string_view slackSecret);

        // https://api.slack.com/apps/<Application ID>/event-subscriptions?
        // i.e. Event Subscriptions tab in your application configuration.
        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
        // https://api.slack.com/apps/<Application ID>/interactive-messages?
        // i.e. Interactivity & Shortcuts tab in your application configuration.
        void handleUserActions(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);

    private:
        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        std::string getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response, bool& found);

        void handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event);

        void handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event);

        /*
         * The following 7 methods are called from: handleCallbackEvent Which is called from handleEvent.
         */
        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackReactionAddedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionRemoved const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackReactionRemovedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinAdded const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackPinAddedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinRemoved const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackPinRemovedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarAdded const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackStarAddedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarRemoved const& /*event*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackStarRemovedEvent", "Call to unimplemented method");
            response.setStatus(501);
        }

        /*
         * The following 8 methods are called from: handleUserActions
         */
        virtual void handleActionsDatePicker(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*selected_date*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsDatePicker", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsDateTimePicker(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::time_t /*selected_date_time*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsDateTimePicker", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsTimePicker(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*selected_time*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsTimePicker", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsCheckBox(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, BK::VecElOption const& /*value*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "SlackEventHandler", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsRadioButton(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*value*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsRadioButton", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsStaticMenu(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*value*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsStaticMenu", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsOverflowMenu(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*value*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsOverflowMenu", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleActionsButton(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response, API::BlockActions const& /*event*/, std::string const& /*action_id*/, std::string const& /*value*/)
        {
            ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "handleActionsButton", "Call to unimplemented method");
            response.setStatus(501);
        }

        struct VisitorEvent
        {
            SlackEventHandler&                  plugin;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::EventURLVerification const& event)    {plugin.handleURLVerificationEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::EventCallback const& event)           {plugin.handleCallbackEvent(request, response, event);}
        };
        struct VisitorCallbackEvent
        {
            SlackEventHandler&                  plugin;
            ThorsAnvil::Nisse::HTTP::Request&   request;
            ThorsAnvil::Nisse::HTTP::Response&  response;

            void operator()(ThorsAnvil::Slack::Event::Message const& event)                 {plugin.handleCallbackMessageEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::ReactionAdded const& event)           {plugin.handleCallbackReactionAddedEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::ReactionRemoved const& event)         {plugin.handleCallbackReactionRemovedEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::PinAdded const& event)                {plugin.handleCallbackPinAddedEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::PinRemoved const& event)              {plugin.handleCallbackPinRemovedEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::StarAdded const& event)               {plugin.handleCallbackStarAddedEvent(request, response, event);}
            void operator()(ThorsAnvil::Slack::Event::StarRemoved const& event)             {plugin.handleCallbackStarRemovedEvent(request, response, event);}
        };
};

inline
SlackEventHandler::SlackEventHandler(std::string_view slackSecret)
    : slackSecret(slackSecret)
{}

inline
void SlackEventHandler::handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    /*
     * TODO:
     * =====
     * Validate that the request is timely.
     * and that it is correctly signed.
     * See: https://docs.slack.dev/authentication/verifying-requests-from-slack
     */
    if (!validateRequest(request)) {
        response.setStatus(400);
        return;
    }

    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using namespace std::string_literals;
    ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "handleEvent", "Message Recieved: ", request);

    ThorsAnvil::Slack::Event::Event     event;
    bool found = false;
    ThorsAnvil::Serialize::ParserConfig config;
    config.setIdentifyDynamicClass([&](ThorsAnvil::Serialize::DataInputStream&){return getEventType(request, response, found);});
    request.body() >> ThorsAnvil::Serialize::jsonImporter(event, config);
    ThorsLogDebugWithData(event, "ThorsAnvil::Slack::SlackEventHandler", "handleEvent", "Message Body: ");

    std::visit(VisitorEvent{*this, request, response}, event);
}

inline
bool SlackEventHandler::validateRequest(ThorsAnvil::Nisse::HTTP::Request& request)
{
    std::string const&  sig = request.variables()["x-slack-signature"];
    std::string const&  timestampStr = request.variables()["x-slack-request-timestamp"];
    auto                timestamp = std::stoll(timestampStr);
    auto                versionEnd = std::min(std::size(sig), sig.find('='));

    if (std::abs(std::time(nullptr) - timestamp) > (60 * 5)) {
        return false;
    }

    using namespace std::literals::string_literals;
    using HMac = ThorsAnvil::Crypto::HMacBuilder<ThorsAnvil::Crypto::Sha256>;

    ThorsAnvil::Crypto::Digest<ThorsAnvil::Crypto::Sha256>      digest;
    {
        HMac hmac(slackSecret, digest);
        hmac.appendData(std::string_view{std::begin(sig), std::begin(sig) + versionEnd});
        hmac.appendData(":"s);
        hmac.appendData(timestampStr);
        hmac.appendData(":"s);

        std::string_view    body = request.preloadStreamIntoBuffer();
        hmac.appendData(body);
    }
    std::size_t versionNext = versionEnd + (versionEnd == std::size(sig) ? 0 : 1);
    bool result = (ThorsAnvil::Crypto::hexdigest<ThorsAnvil::Crypto::Sha256>(digest) == std::string_view{std::begin(sig) + versionNext, std::end(sig)});
    ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "validateRequest", "Request Validation: ", (result ? "OK": "FAIL"));
    return result;
}

inline
std::string SlackEventHandler::getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& /*response*/, bool& found)
{
    if (!found) {
        found = true;
        std::string_view    body = request.preloadStreamIntoBuffer();
        ThorsLogTrack("ThorsAnvil::Slack::SlackEventHandler", "getEventType", "Event Sent: ", body);
        if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
            ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "getEventType", "Found: url_verification");
            return "url_verification";
        }
        if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
            ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "getEventType", "Found: event_callback");
            return "event_callback";
        }
        ThorsLogError("ThorsAnvil::Slack::SlackEventHandler", "getEventType", "Could not identify event type: ERROR");
    }
    ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "getEventType", "Found: Fallback object members");
    return "";
}


inline
void SlackEventHandler::handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event)
{
    ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "handleURLVerificationEvent", "Sending URL Verification");
    ThorsAnvil::Nisse::HTTP::HeaderResponse  headers;
    headers.add("Content-Type", "application/json; charset=utf-8");

    ThorsAnvil::Slack::Event::ResponseURLVerification   reply{event.challenge};

    std::size_t         challangeBackSize = ThorsAnvil::Serialize::jsonStreanSize(reply);

    response.addHeaders(headers);
    response.body(challangeBackSize) << ThorsAnvil::Serialize::jsonExporter(reply, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream});
    return;
}

inline
void SlackEventHandler::handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event)
{
    ThorsLogDebug("ThorsAnvil::Slack::SlackEventHandler", "handleCallbackEvent", "Handling callback event");
    std::visit(VisitorCallbackEvent{*this, request, response}, event.event);
}

inline
void SlackEventHandler::handleUserActions(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    ThorsLogDebug("ThorsAnvil::Slack::TodoSlackEventHandler", "handleUserActions", "Recievent User Action");
    std::stringstream stream(request.variables()["payload"]);

    API::BlockActions       event;
    stream >> ThorsAnvil::Serialize::jsonImporter(event);

    API::SlackAction const& action = event.actions.value()[0];
    std::string const&      type   = action.type;

    if (type == "datepicker") {
        handleActionsDatePicker(request, response, event, action.action_id, action.selected_date.value());
    }
    else if (type == "datetimepicker") {
        handleActionsDateTimePicker(request, response, event, action.action_id, action.selected_date_time.value());
    }
    else if (type == "timepicker") {
        handleActionsTimePicker(request, response, event, action.action_id, action.selected_time.value());
    }
    else if (type == "checkboxes") {
        handleActionsCheckBox(request, response, event, action.action_id, action.selected_options.value());
    }
    else if (type == "radio_buttons") {
        handleActionsRadioButton(request, response, event, action.action_id, action.selected_option->value);
    }
    else if (type == "static_select") {
        handleActionsStaticMenu(request, response, event, action.action_id, action.selected_option->value);
    }
    else if (type == "overflow") {
        handleActionsOverflowMenu(request, response, event, action.action_id, action.selected_option->value);
    }
    else if (type == "button") {
        handleActionsButton(request, response, event, action.action_id, action.value.value());
    }
    else {
        ThorsLogError("UserTodoSlackEventHandler", "handleUserActions", "Unknown Actions", type);
    }
}
}

#endif
