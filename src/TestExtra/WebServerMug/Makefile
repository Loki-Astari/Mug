CONFIG_NAME					= Mug
THORSANVIL_ROOT				?= $(realpath ../../../)
TARGET						= WebServerMug.slib
NO_HEADER					= YES


FILE_WARNING_FLAGS			+= -Wno-missing-braces -Wno-deprecated-copy-with-user-provided-dtor -Wno-deprecated-declarations
LDLIBS_EXTERN_BUILD			+= Nisse ThorSerialize


include $(THORSANVIL_ROOT)/build/tools/Makefile

done:
	echo "Stopping Mug"
	# curl localhost:8079?command=stophard
