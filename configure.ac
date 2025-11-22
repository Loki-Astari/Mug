AC_INIT([ThorsChalice], [0.1], [loki.Astari+ThorsChalice@gmail.com])
AC_PREREQ([2.65])

AC_CONFIG_MACRO_DIR([build/autotools/m4])
AC_CONFIG_AUX_DIR([build/autotools/build])

AX_THOR_FUNC_INIT_BUILD([ThorsChalice], [src/ThorsChalice/ThorsChalice.cpp])


#
# Add extra configuration here
#
AX_THOR_CHECK_USE_THORS_SERIALIZE
AX_THOR_CHECK_USE_MAGIC_ENUM
AX_THOR_CHECK_USE_THORS_NISSE
AX_THOR_CHECK_USE_EVENT
AX_THOR_CHECK_USE_BOOST([1.70], [], [
    AC_MSG_ERROR([
        Nisse requires boost CoRoutine2
        The minimum version we support is via boost 1.70 please upgrade your boost libraries to this (or later).
    ])
])

AC_ARG_WITH(
    [slackToken],
    AS_HELP_STRING([--with-slackToken=<token>], [Slack authentication token. Used to authenticate with Slack when running tests])
)
AC_ARG_WITH(
    [slackSecret],
    AS_HELP_STRING([--with-slackSecret=<secret>], [Slack secret. Used to hash requests and thus validate incomming requests from slack])
)

AS_IF(
    [test "x${with_slackToken}" == "x"],
    [
        AC_MSG_ERROR([No slack token defined. Can not run the slack tests. Please look at tutorials on how to generate a slack Bot Token.])
    ]
)
AS_IF(
    [test "x${with_slackSecret}" == "x"],
    [
        AC_MSG_ERROR([No slack secret defined. Can not run the slack tests. Please look at tutorials on how to generate a slack Bot Secret.])
    ]
)


data=$(curl \
   --header "authorization: Bearer ${with_slackToken}" \
✗  --header "content-type: application/json; charset=utf-8" \
│  --request GET \
│  https://slack.com/api/auth.test 2> /dev/null)

dataok=$(echo ${data} | jq -r .ok)
AS_IF(
    [test "x${dataok}" != "xtrue"],
    [
        AC_MSG_ERROR([
Failed to authenticate with slack please check your token

Command used:
    curl --header "authorization: Bearer ${with_slackToken}" --header "content-type: application/json; charset=utf-8" --request GET https://slack.com/api/auth.test

Response:
    ${data}
        ])
    ]
)


mkdir -p src/ThorsSlack/test/data
cat - <<ENVIRONMENT > src/ThorsSlack/test/data/environment.json
{
    "slackToken": "${with_slackToken}",
    "slackSecret": "${with_slackSecret}"
}
ENVIRONMENT

LT_INIT

AX_THOR_FEATURE_HEADER_ONLY_VARIANT([THORSCHALICE])
AX_THOR_FUNC_TERM_BUILD([THORSCHALICE], [src/ThorsChalice/ThorsChaliceConfig.h:config.h.in src/ThorsSlack/ThorsSlackConfig.h:config.h.in src/ThorsSlackBot/ThorsSlackBotConfig.h:config.h.in ])

AC_OUTPUT
