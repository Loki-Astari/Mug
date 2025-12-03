THORSANVIL_ROOT             = $(realpath ../../)

CXXFLAGS					+= -DDISABLE_SLACKTEST=$(DISABLE_SLACKTEST)
TARGET                      = ThorsSlack.head
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-deprecated-declarations
LDLIBS_EXTERN_BUILD			= Nisse ThorSerialize
COVERAGE_REQUIRED			= 60

include $(THORSANVIL_ROOT)/build/tools/Makefile
