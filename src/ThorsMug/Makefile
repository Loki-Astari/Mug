CONFIG_NAME					= Mug
THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = mug.prog ThorsMug.head
LDLIBS_EXTERN_BUILD			= ThorSerialize Nisse
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor

LDLIBS						+= -levent -levent_pthreads -lboost_coroutine -lboost_context

include $(THORSANVIL_ROOT)/build/tools/Makefile
