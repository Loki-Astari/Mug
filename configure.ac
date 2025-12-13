AC_INIT([ThorsMug], [0.1], [loki.Astari+ThorsMug@gmail.com])
AC_PREREQ([2.65])

AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])

AX_THOR_FUNC_INIT_BUILD([ThorsMug], [src/Mug/mug.cpp])


#
# Add extra configuration here
#
AX_THOR_CHECK_USE_THORS_SERIALIZE
AX_THOR_CHECK_USE_MAGIC_ENUM
AX_THOR_CHECK_USE_THORS_NISSE
AX_THOR_CHECK_USE_EVENT
AX_THOR_CHECK_IS_SLACK_TESTABLE
AX_THOR_CHECK_USE_BOOST([1.70], [], [
    AC_MSG_ERROR([
        Nisse requires boost CoRoutine2
        The minimum version we support is via boost 1.70 please upgrade your boost libraries to this (or later).
    ])
])

sedStrip='s/-.*//'
UNAME=`uname -s | sed "${sedStrip}"`
AS_IF(
    [test "x${UNAME}" = "xMSYS_NT" || test "x${UNAME}" = "xMINGW64_NT" ],
    [
        AC_MSG_NOTICE([Windows])
        AC_MSG_NOTICE([Currently not supporting Windows])
    ]
)
AS_IF(
    [test "x${UNAME}" = "xLinux"],
    [
        AC_MSG_NOTICE([Linux])
        AC_DEFINE([SHARED_LIB_EXTENSION], [so], ["Extension used by shared libs"])
        AC_DEFINE([DLOPEN_PLAT_FLAG], [RTLD_DEEPBIND], ["Extra flags used by dlopen"])
    ]
)
AS_IF(
    [test "x${UNAME}" = "xDarwin"],
    [
        AC_MSG_NOTICE([Darwin])
        AC_DEFINE([SHARED_LIB_EXTENSION], [dylib], ["Extension used by shared libs"])
        AC_DEFINE([DLOPEN_PLAT_FLAG], [0], ["Extra flags used by dlopen"])
    ]
)

LT_INIT

AX_THOR_FEATURE_HEADER_ONLY_VARIANT([THORSMUG])
AX_THOR_FUNC_TERM_BUILD([THORSMUG], [src/Mug/ThorsMugConfig.h:config.h.in src/ThorsMug/ThorsMugConfig.h:config.h.in src/ThorsSlack/ThorsSlackConfig.h:config.h.in src/TestExtra/SlackMug/SlackMugConfig.h:config.h.in src/TestExtra/WebServerMug/WebServerMugConfig.h:config.h.in])

AC_OUTPUT
