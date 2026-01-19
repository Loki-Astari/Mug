CONFIG_NAME					= Mug
THORSANVIL_ROOT				?= $(realpath ../../)
COVERAGE_REQUIRED			= 60

CXXFLAGS					+= -DDISABLE_SLACKTEST=$(DISABLE_SLACKTEST)
TARGET = ThorsSlack.head
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-deprecated-declarations
LDLIBS_EXTERN_BUILD			= ThorSerialize MagicEnum Nisse

EVENT_THREADS_LIB			= $(EVENT_THREADS_LIB_$(PLATFORM))
EVENT_THREADS_LIB_Darwin	=
EVENT_THREADS_LIB_Linux		=
EVENT_THREADS_LIB_MSYS_NT	= -lws2_32
EVENT_THREADS_LIB_MINGW64_NT= -lws2_32
UNITTEST_LDLIBS				= $(EVENT_THREADS_LIB)
UNITTEST_LDLIBS_HEADERONLY	= -lcrypto -lssl

include $(THORSANVIL_ROOT)/build/tools/Makefile
