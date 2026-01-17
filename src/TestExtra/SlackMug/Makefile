CONFIG_NAME					= Mug
THORSANVIL_ROOT				?= $(realpath ../../../)

TARGET						= SlackMug.slib SlackCLI.prog
NO_HEADER					= YES
FILE_WARNING_FLAGS			= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-deprecated-declarations -Wno-error=type-limits -Wno-error=missing-field-initializers
LDLIBS_EXTERN_BUILD			= MagicEnum Nisse ThorSerialize crypto

include $(THORSANVIL_ROOT)/build/tools/Makefile

done:
	echo "Stopping Mug"
	# curl localhost:8079?command=stophard
