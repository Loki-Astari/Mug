#include "SlackStream.h"

#include <utility>

using namespace ThorsAnvil::Slack;

SlackStream::SlackStream()
    : HTTPSStream("slack.com")
{}


using namespace std::string_literals;

const std::string PostMessageData::api  = "https://slack.com/api/chat.postMessage"s;
const std::string AuthTest::api         = "https://slack.com/api/auth.test"s;
