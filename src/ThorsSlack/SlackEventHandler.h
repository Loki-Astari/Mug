#ifndef THORSANVIL_SLACK_SLACKEVENTHANDLER_H
#define THORSANVIL_SLACK_SLACKEVENTHANDLER_H

#include "ThorsSlackConfig.h"
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

        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);

    private:
        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        std::string getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response, bool& found);

        void handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event);

        void handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event);
        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "Message");
            response.setStatus(501);
        }
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "ReactionAdded");
            response.setStatus(501);
        }
        virtual void handleCallbackReactionRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionRemoved const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "ReactionRemoved");
            response.setStatus(501);
        }
        virtual void handleCallbackPinAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinAdded const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "PinAdded");
            response.setStatus(501);
        }
        virtual void handleCallbackPinRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::PinRemoved const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "PinRemoved");
            response.setStatus(501);
        }
        virtual void handleCallbackStarAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarAdded const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "StarAdded");
            response.setStatus(501);
        }
        virtual void handleCallbackStarRemovedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::StarRemoved const& /*event*/)
        {
            ThorsLogError("SlackEventHandler", "handleCallbackMessageEvent", "Call to unimplemented method: ", "StarRemoved");
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
    ThorsLogDebug("SlackEventHandler", "handleEvent", "Message Recieved: ", request);

    ThorsAnvil::Slack::Event::Event     event;
    bool found = false;
    ThorsAnvil::Serialize::ParserConfig config;
    config.setIdentifyDynamicClass([&](ThorsAnvil::Serialize::DataInputStream&){return getEventType(request, response, found);});
    request.body() >> ThorsAnvil::Serialize::jsonImporter(event, config);
    ThorsLogDebugWithData(event, "SlackEventHandler", "handleEvent", "Message Body: ");

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
    ThorsLogDebug("SlackEventHandler", "validateRequest", "Request Validation: ", (result ? "OK": "FAIL"));
    return result;
}

inline
std::string SlackEventHandler::getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& /*response*/, bool& found)
{
    if (!found) {
        found = true;
        std::string_view    body = request.preloadStreamIntoBuffer();
        ThorsLogTrack("SlackEventHandler", "getEventType", "Event Sent: ", body);
        if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
            ThorsLogDebug("SlackEventHandler", "getEventType", "Found: url_verification");
            return "url_verification";
        }
        if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
            ThorsLogDebug("SlackEventHandler", "getEventType", "Found: event_callback");
            return "event_callback";
        }
        ThorsLogError("SlackEventHandler", "getEventType", "Could not identify event type: ERROR");
    }
    ThorsLogDebug("SlackEventHandler", "getEventType", "Found: Fallback object members");
    return "";
}


inline
void SlackEventHandler::handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event)
{
    ThorsLogDebug("SlackEventHandler", "handleURLVerificationEvent", "Sending URL Verification");
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
    ThorsLogDebug("SlackEventHandler", "handleCallbackEvent", "Handling callback event");
    std::visit(VisitorCallbackEvent{*this, request, response}, event.event);
}
}

#endif
