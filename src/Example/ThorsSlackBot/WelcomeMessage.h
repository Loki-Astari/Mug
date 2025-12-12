#ifndef THORSANVIL_SLACK_WELCOME_MESSAGE_H
#define THORSANVIL_SLACK_WELCOME_MESSAGE_H

#include "ThorsSlackBotConfig.h"
#include "ThorsSlack/APIChatMessage.h"
#include "ThorsSlack/SlackBlockKit.h"

#include <utility>
#include <string>
#include <ctime>

namespace ThorsAnvil::Slack
{

class WelcomeMessage
{
    static BlockKit::Section const startText;
    static BlockKit::Divider const divider;

    std::string     channel;
    std::string     user;
    std::string     iconEmoji;
    public:
    std::time_t     timestamp;
    private:
    bool            completed;

    public:
        WelcomeMessage(std::string channel, std::string user)
            : channel(std::move(channel))
            , user(std::move(user))
            , iconEmoji(":robot_face:")
            , timestamp(std::time(nullptr))
            , completed(false)
        {}

        API::Chat::PostMessage getMessage()
        {
            return {    .channel = channel,
                        .text = "Plop",
                        .blocks = BlockKit::Blocks
                        {
                            startText,
                            divider,
                            getReactionTask()
                        },
                        .icon_emoji = iconEmoji,
                        .username = "Welcome Robot!",
                   };
        }

        void markCompleted()            {completed = true;}
        void settime(std::time_t time)  {timestamp = time;}
    private:
        BlockKit::Section getReactionTask()
        {
            std::string checkmark = ":white_check_mark:";
            if (not completed) {
                checkmark = ":white_large_square:";
            }
            std::string text = checkmark + " *React to this message!*";

            return BlockKit::makeSectionMarkD(std::move(text));
        }
};

}

#endif
