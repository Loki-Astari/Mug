CONFIG_NAME					= Mug
THORSANVIL_ROOT				?= $(realpath ../../)
TARGET						= mug.prog
COVERAGE_REQUIRED			= 70

FILE_WARNING_FLAGS			+= -Wno-uninitialized
LDLIBS_EXTERN_BUILD			+= ThorSerialize MagicEnum Nisse event BoostCoRoutine
UNITTEST_LDLIBS				+= -lboost_filesystem
FILE_WARNING_FLAGS			+= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-error=type-limits


include $(THORSANVIL_ROOT)/build/tools/Makefile
