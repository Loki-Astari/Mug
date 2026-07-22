#include "gtest/gtest.h"

#include "ThorsMugConfig.h"
#include "MugServer.h"
#include "MugConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "NisseHTTP/ClientHTTP.h"

#define LOCAL_QUOTE1(X)     #X
#define LOCAL_QUOTE(X)      LOCAL_QUOTE1(X)
#define SLIB                "." LOCAL_QUOTE( SHARED_LIB_EXTENSION )

using MugServerRunner = ThorsAnvil::Nisse::Server::UnitTest::ServerRunner<ThorsAnvil::ThorsMug::MugServer>;

TEST(SlackMugPluginTest, ServiceRunAddServerWithFile)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "../TestExtra/SlackMug/debug/libSlackMug)" SLIB R"(",
                            "configPath":  "test/data/pages"
                        }
                    ]
                }
            ]
        }
    )");
    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }
    MugServerRunner                     server{config, ThorsAnvil::ThorsMug::Active};

    ThorsAnvil::Nisse::HTTP::ClientHTTP         client(ThorsAnvil::ThorsSocket::SocketInfo{"localhost", 8079}, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1);
    client.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}

TEST(SlackMugPluginTest, ServiceRunAddServerWithFileValidateWorks)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "../TestExtra/SlackMug/debug/libSlackMug)" SLIB R"(",
                            "configPath":  "test/data/pages"
                        }
                    ]
                }
            ]
        }
    )");

    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }
    MugServerRunner                     server{config, ThorsAnvil::ThorsMug::Active};

    ThorsAnvil::Nisse::HTTP::ClientHTTP     client({"localhost", 8070});
    std::string line = client.get<std::string>({.path = "/files/page1"});

    EXPECT_EQ("Data for page 1", line);

    ThorsAnvil::Nisse::HTTP::ClientHTTP     control({"localhost", 8079});
    control.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}
