THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = ThorsChalice.prog ThorsChalice.head
LDLIBS_EXTERN_BUILD			= ThorSerialize Nisse
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor

LDLIBS						+= -levent -levent_pthreads -lboost_coroutine -lboost_context

include $(THORSANVIL_ROOT)/build/tools/Makefile
