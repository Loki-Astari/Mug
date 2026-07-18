#include "gtest/gtest.h"

#include "ThorsMugConfig.h"
#include "MugServer.h"
#include "MugConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "NisseHTTP/ClientHTTP.h"

#include <latch>
#include <thread>

#define LOCAL_QUOTE1(X)     #X
#define LOCAL_QUOTE(X)      LOCAL_QUOTE1(X)
#define SLIB                "." LOCAL_QUOTE( SHARED_LIB_EXTENSION )

/*
 * Some locations were we build do not currently support std::jthread.
 * This is a simplified version just for testing purposes.
 */
//    std::jthread
class LocalJthread: public std::thread
{
    public:
        using std::thread::thread;
        ~LocalJthread()
        {
            join();
        }
};

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

    ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
    std::latch                          latch(1);

    auto work = [&]() {
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    // Touch the control point to shut down the server.
    latch.wait();
    ThorsAnvil::Nisse::HTTP::ClientHTTP         client(ThorsAnvil::ThorsSocket::SocketInfo{"localhost", 8079}, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1);
    client.get({.path = "/?command=stophard"});
    client.processResp([](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
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

    ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
    std::latch                          latch(1);
    std::latch                          waitForExit(1);

    auto work = [&]() {
        server.run(
                [&latch](){latch.count_down();}
        );
        waitForExit.count_down();
    };

    LocalJthread     serverThread(work);
    latch.wait();

    ThorsAnvil::Nisse::HTTP::ClientHTTP     client({"localhost", 8070});
    client.get({.path = "/files/page1"});

    std::string line;
    client.processResp([&line](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const& resp)
    {
        std::getline(resp.body(), line);
    });

    EXPECT_EQ("Data for page 1", line);

    ThorsAnvil::Nisse::HTTP::ClientHTTP     control({"localhost", 8079});
    control.get({.path = "/?command=stophard"});
    waitForExit.wait();
}
