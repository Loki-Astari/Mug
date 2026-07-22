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

TEST(WebServerPluginTest, ServiceRunAddServerWithFile)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "../TestExtra/WebServerMug/debug/libWebServerMug)" SLIB R"(",
                            "config":      "test/data/pages"
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

    ThorsAnvil::Nisse::HTTP::ClientHTTP         client({"localhost", 8079});
    client.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}

TEST(WebServerPluginTest, ServiceRunAddServerWithFileValidateWorks)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "../TestExtra/WebServerMug/debug/libWebServerMug)" SLIB R"(",
                            "config":      "test/data/pages"
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
    ThorsAnvil::Nisse::HTTP::HeaderRequest  headers;
    headers.add("accept", "application/json");
    std::string result = client.get<std::string>({.path = "/files/page1", .headers = headers});

    ASSERT_EQ("Data for page 1", result);

    // Touch the control point to shut down the server.
    ThorsAnvil::Nisse::HTTP::ClientHTTP       client2({"localhost", 8079});
    client2.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}

