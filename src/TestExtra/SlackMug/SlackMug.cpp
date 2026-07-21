#include "SlackMug.h"

#include "Environment.h"
#include "NisseHTTP/Util.h"


const Environment           environment("/Users/martinyork/Repo/ThorsMug/src/ThorsSlackMug/.slackenv");
std::unique_ptr<SlackMug>   slackMug;

extern "C" ThorsAnvil::ThorsMug::MugPlugin* mugCreateInstance(int init, char const* config)
{
    if (init) {
        slackMug.reset(new SlackMug(config));
    }
    else {
        slackMug.reset();
    }
    return slackMug.get();
}

namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

SlackMug::SlackMug(char const*)
    : client(environment.botToken, environment.userToken)
    , eventHandler(environment.slackSecret, client, messageCount)
{}

void SlackMug::handleCommand(NisHTTP::Request const& request, NisHTTP::Response& response)
{
    ThorsLogDebug("SlackMug", "handleCommand", "Recievent Command");
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::API::Chat::POSTMessage{.channel = channel, .text = "I have seen " + std::to_string(messageCount[userId])});
    response.setStatus(200);
}

void SlackMug::handleFilePage1(NisHTTP::Request const& /*request*/, NisHTTP::Response& response)
{
    response.body(NisHttp::Encoding::Chunked) << "\"Data for page 1\"";
}

std::vector<ThorsAnvil::ThorsMug::Action> SlackMug::getAction()
{
    return {
            {NisHTTP::Method::POST, "/event",           [&](NisHTTP::Request const& request, NisHTTP::Response& response){eventHandler.handleEvent(request, response);return true;}, [&](NisHTTP::Request const& request){return eventHandler.validateRequest(request);}},
            {NisHTTP::Method::POST, "/command/speak",   [&](NisHTTP::Request const& request, NisHTTP::Response& response){handleCommand(request, response);return true;},            [&](NisHTTP::Request const& request){return eventHandler.validateRequest(request);}},
            {NisHTTP::Method::GET,  "/files/page1",     [&](NisHTTP::Request const& request, NisHTTP::Response& response){handleFilePage1(request, response);return true;}}
           };
}
