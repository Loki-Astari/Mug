#include "gtest/gtest.h"
#include "MugArgs.h"

TEST(MugArgsTest, HelpFlag)
{
    ThorsAnvil::ThorsMug::MugArgs   args;

    EXPECT_FALSE(args.help);
    args.setHelp();
    EXPECT_TRUE(args.help);
}
TEST(MugArgsTest, SilentFlag)
{
    ThorsAnvil::ThorsMug::MugArgs   args;
    EXPECT_FALSE(args.silent);
    args.setSilent();
    EXPECT_TRUE(args.silent);
}
TEST(MugArgsTest, ConfigFlag)
{
    ThorsAnvil::ThorsMug::MugArgs   args;

    EXPECT_EQ("", args.configPath);
    args.setConfigFile("MyFile");
    EXPECT_EQ("MyFile", args.configPath);
}
TEST(MugArgsTest, addLogFile)
{
    ThorsAnvil::ThorsMug::MugArgs   args;
    args.logAddFile("LogFile");
}
TEST(MugArgsTest, addSysLog)
{
    ThorsAnvil::ThorsMug::MugArgs   args;
    args.logAddSys("Application");
}
TEST(MugArgsTest, setLogLevelAll)
{
    ThorsAnvil::ThorsMug::MugArgs   args;

    args.logSetLevel(loguru::Verbosity_5);
    EXPECT_EQ(loguru::g_stderr_verbosity, loguru::Verbosity_5);
}
