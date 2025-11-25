#ifndef THORSANVIL_SLACK_SLACKPLUGIN_H
#define THORSANVIL_SLACK_SLACKPLUGIN_H

#include "ThorsSlackConfig.h"
#include "ThorsSlack/Event.h"
#include "EventCallback.h"
#include "EventCallbackMessage.h"
#include "EventURLVerification.h"

#include "ThorsChalice/DLLib.h"
#include "ThorsSlack/EventCallbackReactionAdded.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorsCrypto/hmac.h"
#include "ThorSerialize/Logging.h"

#include <string>
#include <string_view>
#include <variant>
#include <algorithm>
#include <cmath>
#include <ctime>
#if defined(DEBUG) && (DEBUG != 0)
#incude <iostream>
#endif

namespace ThorsAnvil::ThorsSlack
{

class SlackPlugin: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
        std::string_view  slackSecret;
    public:
        SlackPlugin(std::string_view slackSecret);

    protected:
        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
    private:
        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        std::string getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response, bool& found);

        void handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event);

        void handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event);
        virtual void handleCallbackMessageEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::Message const& /*event*/)
        {
            ThorsLogError("SlackPlugin", "handleCallbackMessageEvent", "Call to unimplemented method");
            response.setStatus(501);
        }
        virtual void handleCallbackReactionAddedEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::ReactionAdded const& /*event*/)
        {
            ThorsLogError("SlackPlugin", "handleCallbackMessageEvent", "Call to unimplemented method");
            response.setStatus(501);
        }


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

inline
SlackPlugin::SlackPlugin(std::string_view slackSecret)
    : slackSecret(slackSecret)
{}

inline
void SlackPlugin::handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
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
#if 1
    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using namespace std::string_literals;
    ThorsLogDebug("SlackPlugin", "handleEvent", "Message Recieved: ", request);

    ThorsAnvil::Slack::Event::Event     event;
#if defined(DEBUG) && (DEBUG != 0)
    std::istream&                       stream = request.body();
    std::string     line;
    while (std::getline(stream, line)) {
        std::cerr << "L>" << line << "<\n";
    }
    std::cerr << "========\n";
#else
    // The callback "event" type field is not the first field!!
    bool found = false;
    ThorsAnvil::Serialize::ParserConfig config;
    config.setIdentifyDynamicClass([&](ThorsAnvil::Serialize::DataInputStream&){return getEventType(request, response, found);});
    request.body() >> ThorsAnvil::Serialize::jsonImporter(event, config);
    ThorsLogDebugWithData(event, "SlackPlugin", "handleEvent", "Message Body: ");

    std::visit(VisitorEvent{*this, request, response}, event);
#endif

#else
    std::string_view    body = request.preloadStreamIntoBuffer();
    if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
        ThorsAnvil::Slack::Event::EventURLVerification   event;
        request.body() >> ThorsAnvil::Serialize::jsonImporter(event);
        handleURLVerificationEvent(request, response, event);
        return;
    }
    if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
        std::cerr << "CALL BACK\n";
        ThorsAnvil::Slack::Event::EventCallback    event;
        request.body() >> ThorsAnvil::Serialize::jsonImporter(event);
        handleCallbackEvent(request, response, event);
        return;
    }
    std::cerr << "UNKNOWN\n";
    response.setStatus(404);
#endif
}

inline
bool SlackPlugin::validateRequest(ThorsAnvil::Nisse::HTTP::Request& request)
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
    ThorsLogDebug("SlackPlugin", "validateRequest", "Request Validation: ", (result ? "OK": "FAIL"));
    return result;
}

inline
std::string SlackPlugin::getEventType(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& /*response*/, bool& found)
{
    if (!found) {
        found = true;
        std::string_view    body = request.preloadStreamIntoBuffer();
        if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
            ThorsLogDebug("SlackPlugin", "getEventType", "Found: url_verification");
            return "url_verification";
        }
        if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
            ThorsLogDebug("SlackPlugin", "getEventType", "Found: event_callback");
            return "event_callback";
        }
    }
    ThorsLogDebug("SlackPlugin", "getEventType", "Found: Fallback object members");
    return "";
}


inline
void SlackPlugin::handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event)
{
    ThorsLogDebug("SlackPlugin", "handleURLVerificationEvent", "Sending URL Verification");
    ThorsAnvil::Nisse::HTTP::HeaderResponse  headers;
    headers.add("Content-Type", "application/json; charset=utf-8");

    ThorsAnvil::Slack::Event::ResponseURLVerification   reply{event.challenge};

    std::size_t         challangeBackSize = ThorsAnvil::Serialize::jsonStreanSize(reply);

    response.addHeaders(headers);
    response.body(challangeBackSize) << ThorsAnvil::Serialize::jsonExporter(reply, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream});
    return;
}

inline
void SlackPlugin::handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event)
{
    ThorsLogDebug("SlackPlugin", "handleCallbackEvent", "Handling callback event");
    std::visit(VisitorCallbackEvent{*this, request, response}, event.event);
}
}

#endif
