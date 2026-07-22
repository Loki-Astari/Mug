#include "gtest/gtest.h"
#include <string>
#include <thread>
#include "MugConfig.h"
#include "MugServer.h"

#include "NisseHTTP/ClientHTTP.h"
#include "NisseHTTP/Util.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsSocket/SocketStream.h"

#include <sstream>
#include <latch>

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

TEST(MugServerTest, CreateHeadless)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    ThorsAnvil::ThorsMug::MugServerMode mode = ThorsAnvil::ThorsMug::Headless;
    ThorsAnvil::ThorsMug::MugServer     server(config, mode);
}
TEST(MugServerTest, CreateActive)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    ThorsAnvil::ThorsMug::MugServerMode mode = ThorsAnvil::ThorsMug::Active;
    ThorsAnvil::ThorsMug::MugServer     server(config, mode);
}

TEST(MugServerTest, ServiceRunDefaultConfigHitControl)
{
    ThorsAnvil::ThorsMug::MugConfig     config;
    std::latch                          latch(1);

    auto work = [&]() {
        ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    // Touch the control point to shut down the server.
    latch.wait();
    ThorsAnvil::Nisse::HTTP::ClientHTTP     client({"localhost", 8079});
    client.get<std::string>({.path = "/?command=stophard"});
}

TEST(MugServerTest, ServiceRunModifiedControl)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8078,
            "servers": []
        }
    )");
    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }


    std::latch                          latch(1);

    auto work = [&]() {
        ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    // Touch the control point to shut down the server.
    latch.wait();
    ThorsAnvil::Nisse::HTTP::ClientHTTP       client({"localhost", 8078});
    client.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}

TEST(MugServerTest, ServiceRunAddServer)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                    ]
                }
            ]
        }
    )");
    ThorsAnvil::ThorsMug::MugConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    std::latch                          latch(1);

    auto work = [&]() {
        ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
        server.run(
                [&latch](){latch.count_down();}
        );
    };

    LocalJthread     serverThread(work);

    // Touch the control point to shut down the server.
    latch.wait();
    ThorsAnvil::Nisse::HTTP::ClientHTTP       client({"localhost", 8079});
    client.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
}

TEST(MugServerTest, CallALoadedLib)
{
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "pluginPath":  "../TestExtra/L3/release/libL3)" SLIB R"(",
                            "config":  "/page1"
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

    std::latch                          latch(1);
    std::latch                          waitForExit(1);

    auto work = [&]() {
        ThorsAnvil::ThorsMug::MugServer     server(config, ThorsAnvil::ThorsMug::Active);
        server.run(
                [&latch](){latch.count_down();}
        );
        waitForExit.count_down();
    };

    LocalJthread     serverThread(work);
    latch.wait();

    // Talk to server.
    ThorsAnvil::Nisse::HTTP::ClientHTTP     client({"localhost", 8070});
    ThorsAnvil::Nisse::HTTP::HeaderRequest  headers;
    headers.add("accept", "application/json");
    bool called = false;
    client.get_async({.path = "/Plop/"}, [&called](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const& resp)
    {
        called = true;
        ASSERT_EQ(305, resp.getStatus());
    });
    ASSERT_TRUE(called);

    // Touch the control point to shut down the server.
    ThorsAnvil::Nisse::HTTP::ClientHTTP       client2({"localhost", 8079});
    client2.get_async({.path = "/?command=stophard"}, [](ThorsAnvil::Nisse::HTTP::ClientHTTPResponse const&){});
    waitForExit.wait();
}

