#include "SlackPlugin.h"
#include "Event.h"
#include "ThorsCrypto/hmac.h"


namespace Ser       = ThorsAnvil::Serialize;
namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

using namespace ThorsAnvil::ThorsSlack;

SlackPlugin::SlackPlugin(std::string const& slackToken)
    : slackToken(slackToken)
{}

void SlackPlugin::handleEvent(NisHTTP::Request& request, NisHTTP::Response& response)
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
    using NisHTTP::HeaderResponse;
    using namespace std::string_literals;
    std::cerr << "Recieved: Message\n";
    std::cerr << request << "\n";

    ThorsAnvil::Slack::Event::Event     event;
#if 0
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
    std::cerr << "Body\n" << ThorsAnvil::Serialize::jsonExporter(event) << "\n----------\n";

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

bool SlackPlugin::validateRequest(NisHTTP::Request& request)
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
        HMac hmac(slackToken, digest);
        hmac.appendData(std::string_view{std::begin(sig), std::begin(sig) + versionEnd});
        hmac.appendData(":"s);
        hmac.appendData(timestampStr);
        hmac.appendData(":"s);

        std::string_view    body = request.preloadStreamIntoBuffer();
        hmac.appendData(body);
    }
    std::size_t versionNext = versionEnd + (versionEnd == std::size(sig) ? 0 : 1);
    return ThorsAnvil::Crypto::hexdigest<ThorsAnvil::Crypto::Sha256>(digest) == std::string_view{std::begin(sig) + versionNext, std::end(sig)};
}

std::string SlackPlugin::getEventType(NisHTTP::Request& request, NisHTTP::Response& /*response*/, bool& found)
{
    if (!found) {
        found = true;
        std::string_view    body = request.preloadStreamIntoBuffer();
        if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
            return "url_verification";
        }
        if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
            return "event_callback";
        }
    }
    return "";
}


void SlackPlugin::handleURLVerificationEvent(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventURLVerification const& event)
{
    NisHTTP::HeaderResponse  headers;
    headers.add("Content-Type", "application/json; charset=utf-8");

    ThorsAnvil::Slack::Event::ResponseURLVerification   reply{event.challenge};

    std::size_t         challangeBackSize = Ser::jsonStreanSize(reply);

    response.addHeaders(headers);
    response.body(challangeBackSize) << Ser::jsonExporter(reply, Ser::PrinterConfig{Ser::OutputType::Stream});
    return;
}

void SlackPlugin::handleCallbackEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response,ThorsAnvil::Slack::Event::EventCallback const& event)
{
    std::visit(VisitorCallbackEvent{*this, request, response}, event.event);
}
