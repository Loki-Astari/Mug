#include "Environment.h"
#include "SlackClient.h"
#include "SlackAPI_Chat.h"
#include "ThorSerialize/JsonThor.h"
#include "SlackBlockKit.h"
#include "ThorSerialize/PrinterConfig.h"

using namespace std::literals::string_literals;

int main()
{
    using ThorsAnvil::Slack::BlockKit::Block;
    using ThorsAnvil::Slack::BlockKit::Blocks;
    using ThorsAnvil::Slack::BlockKit::Section;
    using ThorsAnvil::Slack::BlockKit::Divider;
    using ThorsAnvil::Slack::BlockKit::Element::Text;

    Block block = Section{.text = Text{.type = "mrkdwn", .text = "Hi there this is text"}};
    std::cerr << ThorsAnvil::Serialize::jsonExporter(block);
    std::cerr << ThorsAnvil::Serialize::jsonExporter(block, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
    std::cerr << "Size: " << ThorsAnvil::Serialize::jsonStreanSize(block) << "\n";

    loguru::g_stderr_verbosity = 9;
    const Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.slackToken);

    using ThorsAnvil::Slack::SlackClient;
    using ThorsAnvil::Slack::API::Chat::PostMessage;

    SlackClient             client(environment.slackToken);
    PostMessage::Reply      reply1 = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply1);

    PostMessage::Reply      reply2 = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = Blocks{Section{.text = Text{.type = "mrkdwn", .text = "Stuff to print"}}, Divider{}, Section{.text = Text{.type = "plain_text", .text = "Here we go"}}}});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply2);


#if 0
struct PostMessage
{
    std::string                 channel;
    OptString                   text;
    BlockKit::OptBlocks         blocks;
    OptString                   icon_emoji;
    OptString                   username;
    OptString                   thread_ts;
};
struct Section
{
    // https://docs.slack.dev/reference/block-kit/blocks/section-block/
    Element::OptText            text;           // The text for the block, in the form of a text object. Minimum length for the text in this field is 1 and maximum length is 3000 characters.
    Element::OptTexts           fields;         // Required if no text is provided. An array of text objects. Any text objects included with fields will be rendered in a compact format that allows for 2 columns of side-by-side text.
};
#endif

}
