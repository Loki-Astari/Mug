#include "gtest/gtest.h"
#include <variant>

#include "Environment.h"
#include "SlackClient.h"
#include "SlackAPI_Chat.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/PrinterConfig.h"
#include "SlackBlockKit.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;

extern SlackClient             client;

#if 0
Tested:
=======
    ThorsAnvil::Slack::BlockKit::ElActive
    ThorsAnvil::Slack::BlockKit::ElImg
    ThorsAnvil::Slack::BlockKit::ElBut
    ThorsAnvil::Slack::BlockKit::ElText
    ThorsAnvil::Slack::BlockKit::ElSlackFile
    ThorsAnvil::Slack::BlockKit::ElInput
    // -- Rich Text
    ThorsAnvil::Slack::BlockKit::InfoStyle
    ThorsAnvil::Slack::BlockKit::InfoText
    //---
    ThorsAnvil::Slack::BlockKit::RichTextPreformatted
    ThorsAnvil::Slack::BlockKit::RichTextQuote
    // -- Rich Text
    ThorsAnvil::Slack::BlockKit::ElRow
    ThorsAnvil::Slack::BlockKit::ElColInfo
    /// ----
    ThorsAnvil::Slack::BlockKit::Actions
    ThorsAnvil::Slack::BlockKit::Context
    ThorsAnvil::Slack::BlockKit::Context_Actions
*   ThorsAnvil::Slack::BlockKit::Divider
    ThorsAnvil::Slack::BlockKit::File
    ThorsAnvil::Slack::BlockKit::Header
    ThorsAnvil::Slack::BlockKit::Image
    ThorsAnvil::Slack::BlockKit::Input
    ThorsAnvil::Slack::BlockKit::Markdown
    ThorsAnvil::Slack::BlockKit::RichText
*   ThorsAnvil::Slack::BlockKit::Section
    ThorsAnvil::Slack::BlockKit::Table
    ThorsAnvil::Slack::BlockKit::Video
#endif

TEST(SlackBlockKitTest, Block_Section_ElText)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::Section{
                                                                            .text = BK::ElText{
                                                                                        .type = BK::mrkdwn,
                                                                                        .text = "Section With Text"}
                                                                            },
                                                                        BK::Divider{},
                                                                        BK::Section{
                                                                            .text = BK::ElText{.type = BK::plain_text, .text = "More Text after a divider"}
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_EQ(3, reply.message->blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message->blocks[0]));
    BK::Section&            section1 = std::get<BK::Section>(reply.message->blocks[0]);
    ASSERT_TRUE(section1.text.has_value());
    EXPECT_EQ("Section With Text", section1.text.value().text);

    ASSERT_TRUE(std::holds_alternative<BK::Divider>(reply.message->blocks[1]));

    ASSERT_TRUE(std::holds_alternative<BK::Section>(reply.message->blocks[2]));
    BK::Section&            section2 = std::get<BK::Section>(reply.message->blocks[2]);
    ASSERT_TRUE(section2.text.has_value());
    EXPECT_EQ("More Text after a divider", section2.text.value().text);
}

TEST(SlackBlockKitTest, Block_RichText_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextSection{
                                                                                    .elements = {
                                                                                        BK::ElRtText{.text = "Rich Text: Section", .style = BK::InfoText{true,true,true,true}}
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_EQ(1, reply.message->blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message->blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message->blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextSection>(text.elements[0]));
    BK::RichTextSection&    section = std::get<BK::RichTextSection>(text.elements[0]);

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("Rich Text: Section", rtext.text);
    EXPECT_TRUE(rtext.style->bold);
    EXPECT_TRUE(rtext.style->italic);
    EXPECT_TRUE(rtext.style->strike);
    EXPECT_TRUE(rtext.style->code);
}

TEST(SlackBlockKitTest, Block_RichText_ListBullet_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextList{
                                                                                    .style = BK::bullet,
                                                                                    .elements = {
                                                                                        BK::RichTextSection{
                                                                                            .elements = {
                                                                                                BK::ElRtText{.text = "Rich Text: List-bullet", .style = BK::InfoText{true,true,true,true}}
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_EQ(1, reply.message->blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message->blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message->blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextList>(text.elements[0]));
    BK::RichTextList&       list = std::get<BK::RichTextList>(text.elements[0]);
    EXPECT_EQ(BK::bullet, list.style);

    BK::RichTextSection&    section = list.elements[0];

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("Rich Text: List-bullet", rtext.text);
    EXPECT_TRUE(rtext.style->bold);
    EXPECT_TRUE(rtext.style->italic);
    EXPECT_TRUE(rtext.style->strike);
    EXPECT_TRUE(rtext.style->code);
}

TEST(SlackBlockKitTest, Block_RichText_ListOrder_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextList{
                                                                                    .style = BK::ordered,
                                                                                    .elements = {
                                                                                        BK::RichTextSection{
                                                                                            .elements = {
                                                                                                BK::ElRtText{.text = "Rich Text: List-ordered", .style = BK::InfoText{true,true,true,true}}
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
    ASSERT_TRUE(reply.message.has_value());
    ASSERT_EQ(1, reply.message->blocks.size());
    ASSERT_TRUE(std::holds_alternative<BK::RichText>(reply.message->blocks[0]));
    BK::RichText&           text = std::get<BK::RichText>(reply.message->blocks[0]);

    ASSERT_TRUE(std::holds_alternative<BK::RichTextList>(text.elements[0]));
    BK::RichTextList&       list = std::get<BK::RichTextList>(text.elements[0]);
    EXPECT_EQ(BK::ordered, list.style);

    BK::RichTextSection&    section = list.elements[0];

    ASSERT_TRUE(std::holds_alternative<BK::ElRtText>(section.elements[0]));
    BK::ElRtText&           rtext = std::get<BK::ElRtText>(section.elements[0]);

    EXPECT_EQ("Rich Text: List-ordered", rtext.text);
    EXPECT_TRUE(rtext.style->bold);
    EXPECT_TRUE(rtext.style->italic);
    EXPECT_TRUE(rtext.style->strike);
    EXPECT_TRUE(rtext.style->code);
}

TEST(SlackBlockKitTest, Block_RichText_Pre_Formatted)
{
    PostMessage     message = {.channel = "C09RU2URYMS",
                                                                   .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextPreformatted{
                                                                                    .elements = {
                                                                                        BK::ElRtText{.text = "Rich Text PreFormatted Test", .style = BK::InfoText{true,true,true,true}}
                                                                                    },
                                                                                    .border=1
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                              };
    PostMessage::Reply      reply = client.sendMessage(message);
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(SlackBlockKitTest, Block_RichText_QUOTE)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS",
                                                                   .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextQuote{
                                                                                    .elements = {
                                                                                        BK::ElRtText{.text = "Rich Text Quote", .style = BK::InfoText{true,true,true,true}}
                                                                                    },
                                                                                    .border=1
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                  });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(SlackBlockKitTest, BlockAllRichTextElements)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{
                                                            .channel = "C09RU2URYMS",
                                                            .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextSection{
                                                                                    .elements = {
                                                                                        BK::ElRtText{.text = "Rich Text: All ELEMENTS TEST",}
                                                                                        ,BK::ElRtText{.text = "Broadcast",}
                                                                                        ,BK::ElRtBroadcast{.range = BK::here}
                                                                                        ,BK::ElRtText{.text = "Color",}
                                                                                        ,BK::ElRtColor{.value = "#FF0000"}
                                                                                        ,BK::ElRtText{.text = "Channel",}
                                                                                        ,BK::ElRtChannel{.channel_id="C09SGCEFLPJ", .style=BK::InfoStyle{true, false, false, true, false, false}}
                                                                                        ,BK::ElRtText{.text = "Date",}
                                                                                        ,BK::ElRtDate{.timestamp=-21636000, .format="{date_long_pretty}",.fallback="Date Failed to parse"}
                                                                                        ,BK::ElRtText{.text = "Emoji",}
                                                                                        ,BK::ElRtEmoji{.name="wave"}
                                                                                        ,BK::ElRtText{.text = "Link",}
                                                                                        ,BK::ElRtLink{.url="https://github.com/Loki-Astari/ThorsMongo", .text= "Thors Mongo Lib"}
                                                                                        ,BK::ElRtText{.text = "Text",}
                                                                                        ,BK::ElRtText{.text = "For Completness another text item",}
                                                                                        ,BK::ElRtText{.text = "User",}
                                                                                        ,BK::ElRtUser{.user_id = "U095XJHJ1J5"}
                                                                                        ,BK::ElRtText{.text = "UserGroup",}
#if 0
                                                                                        // Can't test UserGroups with the free version of slack.
                                                                                        ,BK::ElRtUserGroup{.usergroup_id = "U095XJHJ1J5"}
#endif
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                            }
                                                       });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}

TEST(SlackBlockKitTest, Block_RichText_Failure)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS",
                                                                   .blocks = BK::Blocks{
                                                                        BK::RichText{
                                                                            .elements = {
                                                                                BK::RichTextQuote{
                                                                                    .elements = {
                                                                                        BK::ElRtText{.text = "Rich Text Quote", .style = BK::InfoText{true,true,true,true}}
                                                                                    },
                                                                                    .border=3 // Invalid Value should result in an error.
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                  });
    if (reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_FALSE(reply.ok);
}

TEST(SlackBlockKitTest, Block_Action_Button)
{
    PostMessage     message{    .channel = "C09RU2URYMS",
                                .blocks = BK::Blocks{
                                    BK::Context{
                                        .elements = {
                                            BK::ElImg{
                                                .alt_text = "Alternative Text 1",
                                                .image_url = "https://images.unsplash.com/photo-1486365227551-f3f90034a57c?q=80&w=1740&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D"
                                            },
                                            BK::ElImg{
                                                .alt_text = "Alternative Text 2",
                                                .image_url = "https://images.unsplash.com/photo-1606567595334-d39972c85dbe?q=80&w=1287&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D"
                                            }
                                        }
                                    },
                                }
                           };
#if 0
    PostMessage     message{    .channel = "C09RU2URYMS",
                                .blocks = BK::Blocks{
                                    BK::Actions{
                                        .elements = {
                                            BK::ElActButton{.text = BK::ElText{.text = "Push"}, .value="Clicked"}
                                        }
                                    },
                                }
                           };
#endif
    BK::Context& actions = std::get<BK::Context>(message.blocks.value()[0]);
    std::cerr << "Count: " << actions.elements.size() << "\n";

    ThorsAnvil::Serialize::PrinterConfig config{ThorsAnvil::Serialize::OutputType::Stream};
    std::cerr << ThorsAnvil::Serialize::jsonExporter(message) << "\n"
              << ThorsAnvil::Serialize::jsonExporter(message, config) << "\n"
              << "Size: " << ThorsAnvil::Serialize::jsonStreanSize(message) << "\n";
    PostMessage::Reply      reply = client.sendMessage(message);
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
#if 0
#endif
}
TEST(SlackBlockKitTest, Block_Section_All_Standard_Elements)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS",
                                                                        // Actions, Context, Context_Actions, Divider, File, Header, Image, Input, Markdown, RichText, Section, Table, Video
                                                                   .blocks = BK::Blocks{
                                                                        BK::Section{.text = BK::ElText{.text="Selection with all Elements"}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Actions"}},
                                                                        BK::Actions{
                                                                            .elements = {
                                                                                BK::ElActButton{.text = BK::ElText{.text = "Push"}, .value="Clicked"}
                                                                                , BK::ElActCheckbox{
                                                                                    .options = {
                                                                                        BK::ElOption{.text=BK::ElText{.text="Opt 1"},.value="V1",.description=BK::ElText{.text="The description 1"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 2"},.value="V2",.description=BK::ElText{.text="The description 2"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 3"},.value="V3",.description=BK::ElText{.text="The description 3"}}
                                                                                    }
                                                                                }
                                                                                , BK::ElActDatePicker{.placeholder=BK::ElText{.text="Pick a date"}}
                                                                                , BK::ElActDatetimePicker{.initial_date_time=-21636000}
                                                                                , BK::ElActOverflowMenu{
                                                                                    .options = {
                                                                                        BK::ElOption{.text=BK::ElText{.text="Opt 1"},.value="V1",.description=BK::ElText{.text="The description 1"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 2"},.value="V2",.description=BK::ElText{.text="The description 2"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 3"},.value="V3",.description=BK::ElText{.text="The description 3"}}
                                                                                    }
                                                                                }
                                                                            }
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Actions"}},
                                                                        BK::Actions{
                                                                            .elements = {
                                                                                BK::ElActRadioButton {
                                                                                    .options = {
                                                                                        BK::ElOption{.text=BK::ElText{.text="Opt 1"},.value="V1",.description=BK::ElText{.text="The description 1"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 2"},.value="V2",.description=BK::ElText{.text="The description 2"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 3"},.value="V3",.description=BK::ElText{.text="The description 3"}}
                                                                                    }
                                                                                }
                                                                                , BK::ElActSelectMenu{
                                                                                    .options = {
                                                                                        BK::ElOption{.text=BK::ElText{.text="Opt 1"},.value="V1",.description=BK::ElText{.text="The description 1"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 2"},.value="V2",.description=BK::ElText{.text="The description 2"}}
                                                                                        , BK::ElOption{.text=BK::ElText{.text="Opt 3"},.value="V3",.description=BK::ElText{.text="The description 3"}}
                                                                                    }
                                                                                }
                                                                                , BK::ElActTimePicker{}
#if 0
// TODO: Need to implement workflows before this becomes useful.
                                                                                , BK::ElActWorkflowButton{
                                                                                    .text = BK::ElText{.text = "Workflow"},
                                                                                    .workflow = BK::Workflow{
                                                                                        .trigger = {
                                                                                            .url = "https://slack.com/shortcuts/Ft0123ABC456/321...zyx",
                                                                                            .customizable_input_parameters ={
                                                                                                BK::NameValue{"input_parameter_a", "Value for input param A"},
                                                                                                BK::NameValue{"input_parameter_b", "Value for input param B"}
                                                                                            }
                                                                                        }
                                                                                    },
                                                                                    .action_id = "Action",
                                                                                }
#endif
                                                                            }
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Context"}},
                                                                        BK::Context{
                                                                            .elements = {
                                                                                BK::ElImg{
                                                                                    .alt_text = "Alternative Text 1",
                                                                                    .image_url = "https://images.unsplash.com/photo-1486365227551-f3f90034a57c?q=80&w=1740&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D"
                                                                                },
                                                                                BK::ElImg{
                                                                                    .alt_text = "Alternative Text 2",
                                                                                    .image_url = "https://images.unsplash.com/photo-1606567595334-d39972c85dbe?q=80&w=1287&auto=format&fit=crop&ixlib=rb-4.1.0&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D"
                                                                                }
                                                                            }
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Context Actions"}},
                                                                        BK::ContextActions{
                                                                            .elements = {
                                                                                BK::ElFeedbackButton {
                                                                                    .positive_button = BK::ElButton{
                                                                                        .text = BK::ElText{.text="Pos"},.value="Pos1Val"
                                                                                    },
                                                                                    .negative_button = BK::ElButton{
                                                                                        .text = BK::ElText{.text="Neg"},.value="Neg1Val"
                                                                                    },
                                                                                },
                                                                                BK::ElFeedbackButton{
                                                                                    .positive_button = BK::ElButton{
                                                                                        .text = BK::ElText{.text="Pos"},.value="Pos2Val"
                                                                                    },
                                                                                    .negative_button = BK::ElButton{
                                                                                        .text = BK::ElText{.text="Neg"},.value="Neg2Val"
                                                                                    },
                                                                                }
                                                                            }
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Divider"}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Header"}},
                                                                        BK::Header{.text = BK::ElText{.text = "Header Text"}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Image"}},
                                                                        BK::Image{.alt_text="An image", .image_url="https://marvelofficial.com/wp-content/uploads/2020/10/1030357-247x296.jpg", .title=BK::ElText{.text="Thors Hammer"}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Input"}},
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Checkbox"},
                                                                            .element=BK::ElActCheckbox{
                                                                                .options={
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option1"},.value="1",.description=BK::ElText{.text="Description1"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option2"},.value="2",.description=BK::ElText{.text="Description2"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option3"},.value="3",.description=BK::ElText{.text="Description3"}}
                                                                                }
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: DatePicker"},
                                                                            .element=BK::ElActDatePicker{},
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: DateTimePicker"},
                                                                            .element=BK::ElActDatetimePicker{},
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: RadioButton"},
                                                                            .element=BK::ElActRadioButton{
                                                                                .options={
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option1"},.value="1",.description=BK::ElText{.text="Description1"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option2"},.value="2",.description=BK::ElText{.text="Description2"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option3"},.value="3",.description=BK::ElText{.text="Description3"}}
                                                                                }
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Email"},
                                                                            .element=BK::ElActEMail{},
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Number Input"},
                                                                            .element=BK::ElActNumberInput{
                                                                                .min_value="10",
                                                                                .max_value="24"
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Plain Text Input"},
                                                                            .element=BK::ElActPlainTextInput{
                                                                                .min_length=10,
                                                                                .max_length=24
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
#if 0
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Rich Text Input"},
                                                                            .element=BK::ElActRichTextInput{
                                                                                .action_id="rich_text_input-action"
                                                                                .initial_value=BK::RichText{
                                                                                    .elements ={
                                                                                        BK::RichTextSection{
                                                                                            .elements={
                                                                                                BK::ElRtColor{.value="#00FF00"}
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }*/
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
#endif
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Select Menu"},
                                                                            .element=BK::ElActSelectMenu{
                                                                                .options={
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option1"},.value="1",.description=BK::ElText{.text="Description1"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option2"},.value="2",.description=BK::ElText{.text="Description2"}},
                                                                                    BK::ElOption{.text=BK::ElText{.text="Option3"},.value="3",.description=BK::ElText{.text="Description3"}}
                                                                                }
                                                                            },
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: Time Picker"},
                                                                            .element=BK::ElActTimePicker{},
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Input{
                                                                            .label=BK::ElText{.text="User Input: URL Input"},
                                                                            .element=BK::ElActURLInput{},
                                                                            .hint=BK::ElText{.text="Hinty"}
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Markdown"}},
                                                                        BK::Markdown{.text="**MarkDown** Bold?"},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="RichText"}},
                                                                        BK::RichText{.elements = {BK::RichTextQuote{.elements = {BK::ElRtText{.text = "Rich Text Quote", .style = BK::InfoText{true,true,false,false}}}}}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Section"}},
                                                                        BK::Section{.text = BK::ElText{.text="This is the section test!!"}},
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Table"}},
                                                                        BK::Table{
                                                                            .rows = {
                                                                                {
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-1-1"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-1-2"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-1-3"}}}}}
                                                                                },
                                                                                {
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-2-1"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-2-2"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-2-3"}}}}}
                                                                                },
                                                                                {
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-3-1"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-3-2"}}}}},
                                                                                    BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "C-3-3"}}}}}
                                                                                }
                                                                            },
                                                                            .column_settings = BK::VecElColInfo{
                                                                                BK::ElColInfo{.align="left"},BK::ElColInfo{.align="center"},BK::ElColInfo{.align="right"}
                                                                            }
                                                                        },
                                                                        BK::Divider{},
                                                                        BK::Section{.text = BK::ElText{.text="Video:TODO"}},
                                                                    #if 0
                                                                        // Need to own the domain that the video is coming from.
                                                                        BK::Video{
                                                                            .alt_text="A video",
                                                                            .author_name="Not me",
                                                                            .description=BK::ElText{.text="The desction"},
                                                                            .provider_icon_url="",
                                                                            .provider_name="Provider Name",
                                                                            .title=BK::ElText{.text="Title"},
                                                                            .thumbnail_url="https://i.ytimg.com/vi/0kNpnjPpf1c/oar2.jpg?sqp=-oaymwEoCJUDENAFSFqQAgHyq4qpAxcIARUAAIhC2AEB4gEKCBgQAhgGOAFAAQ==&rs=AOn4CLDSJuHZ9q0tZXS8Kz6QPXW-5NqG3g",
                                                                            .video_url="https://www.youtube.com/shorts/0kNpnjPpf1c"
                                                                        },
                                                                    #endif
                                                                        BK::Divider{}
                                                                    }
                                                                  });
    if (!reply.ok) {
        std::cerr << ThorsAnvil::Serialize::jsonExporter(reply);
    }
    ASSERT_TRUE(reply.ok);
}


