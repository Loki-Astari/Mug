#include "WelcomeMessage.h"
#include "ThorsSlack/SlackBlockKit.h"


using namespace ThorsAnvil::Slack;

BlockKit::Section const WelcomeMessage::startText = BlockKit::makeSectionMarkD(R"(Welcome to this awesome channel! -- *Get started by completing the tasks!*)");
BlockKit::Divider const WelcomeMessage::divider;
