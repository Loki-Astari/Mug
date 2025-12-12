CONFIG_NAME					= Mug
THORSANVIL_ROOT             = $(realpath ../../)

TARGET                      = mug.prog ThorsMug.head
LDLIBS_EXTERN_BUILD			= ThorSerialize MagicEnum Nisse
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-error=type-limits

LDLIBS						+= -levent -levent_pthreads -lboost_coroutine -lboost_context

include $(THORSANVIL_ROOT)/build/tools/Makefile
