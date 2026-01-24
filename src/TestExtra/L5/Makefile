CONFIG_NAME					= Mug
THORSANVIL_ROOT				?= $(realpath ../../../)
TARGET						= L5.slib
TEST_ONLY					= YES


FILE_WARNING_FLAGS			+= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor
LDLIBS_EXTERN_BUILD			+= Nisse ThorSerialize


include $(THORSANVIL_ROOT)/build/tools/Makefile
