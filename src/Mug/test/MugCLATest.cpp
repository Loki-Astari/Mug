#include "gtest/gtest.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "MugCLA.h"

struct MockFile
{
    std::string name;
    MockFile(std::string const& name, std::string const& content)
        : name(name)
    {
        std::ofstream   file(name);
        file << "{}";
    }

    ~MockFile()
    {
        std::filesystem::remove(name);
    }
};

struct MockArguments: public ThorsAnvil::ThorsMug::MugCLAInterface
{
    std::size_t                 classCount      = 0;
    std::array<std::size_t, 6>  methodCallCount = {0, 0, 0, 0, 0, 0};
    std::filesystem::path       config;
    std::filesystem::path       logFile;
    std::string                 appName;
    loguru::Verbosity           verbosity;

    virtual void logAddFile(std::filesystem::path file)     override
    {
        ++classCount;
        ++methodCallCount[0];
        logFile = file;
    }
    virtual void logAddSys(std::string_view app)            override
    {
        ++classCount;
        ++methodCallCount[1];
        appName = app;
    }
    virtual void logSetLevel(loguru::Verbosity level)       override
    {
        ++classCount;
        ++methodCallCount[2];
        verbosity = level;
    }
    virtual void setHelp()                                  override
    {
        ++classCount;
        ++methodCallCount[3];
    }
    virtual void setSilent()                                override
    {
        ++classCount;
        ++methodCallCount[4];
    }
    virtual void setConfigFile(std::filesystem::path file)  override
    {
        ++classCount;
        ++methodCallCount[5];
        config = file;
    }
};

TEST(MugCLATest, NoArgumentsOnlyApplicationName)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test"}, mockArgs);

    // Call to load
    ASSERT_EQ(0, mockArgs.classCount);
}
TEST(MugCLATest, InvalidArgument)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--invalid"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
}
TEST(MugCLATest, HelpFlag)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--help"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
}
TEST(MugCLATest, SilentFlag)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--silent"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[4]);
}
TEST(MugCLATest, ConfigFlag)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--config=LocalFile"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);
    ASSERT_EQ("LocalFile", mockArgs.config);
}
TEST(MugCLATest, addLogFile)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logFile=LogFile"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[0]);
    ASSERT_EQ("LogFile", mockArgs.logFile);
}
TEST(MugCLATest, addSysLog)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logSys"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ("Test", mockArgs.appName);
}
TEST(MugCLATest, addSysLogExplicit)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logSys=AppName"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ("AppName", mockArgs.appName);
}
TEST(MugCLATest, setLogLevelAll)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=All"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_9, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelTrace)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Trace"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_7, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelTrack)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Track"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_5, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelDebug)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Debug"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_3, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelInfo)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Info"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_INFO, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelWarn)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Warn"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_WARNING, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelError)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Error"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_ERROR, mockArgs.verbosity);
}
TEST(MugCLATest, setLogLevelInvalid)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Fake"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
}
TEST(MugCLATest, displayHelp)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logLevel=Error"}, mockArgs);

    std::stringstream                   output;
    cla.displayHelp("TestingCommand", output);

    EXPECT_FALSE(output.str().empty());

}
TEST(MugCLATest, findDefaultConfigFile)
{
    MockFile                                file("mug.cfg", "{}");

    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test"}, mockArgs);

    EXPECT_EQ(mockArgs.config, "./mug.cfg");
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);

}
TEST(MugCLATest, dontUseDefaultIfUserSpecified)
{
    MockFile                            file("mug.cfg", "{}");

    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--config=MyConfig"}, mockArgs);

    EXPECT_EQ(mockArgs.config, "MyConfig");
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);
}
TEST(MugCLATest, allFlags)
{
    MockArguments                       mockArgs;
    ThorsAnvil::ThorsMug::MugCLA        cla({"Test", "--logFile=log1", "--logSys=Sys", "--logLevel=Warn", "--config=Configer", "--help", "--silent"}, mockArgs);

    // Call to load
    ASSERT_EQ(6, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[0]);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
    ASSERT_EQ(1, mockArgs.methodCallCount[4]);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);
    ASSERT_EQ("Sys", mockArgs.appName);
    ASSERT_EQ("log1", mockArgs.logFile);
    ASSERT_EQ("Configer", mockArgs.config);
    ASSERT_EQ(loguru::Verbosity_WARNING, mockArgs.verbosity);
}

