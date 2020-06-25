LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

common_SRC_FILES := \
	adler32.c \
	compress.c \
	crc32.c \
	deflate.c \
	gzclose.c \
	gzlib.c \
	gzread.c \
	gzwrite.c \
	infback.c \
	inffast.c \
	inflate.c \
	inftrees.c \
	trees.c \
	uncompr.c \
	zutil.c

common_CFLAGS := ## -fomit-frame-pointer

common_C_INCLUDES += \

# For the host
# =====================================================

LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS)
LOCAL_C_INCLUDES += $(common_C_INCLUDES)

#LOCAL_MODULE:= zlib
LOCAL_MODULE := cocos_zlib_static
LOCAL_MODULE_FILENAME := zlib

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_COPY_HEADERS_TO := $(common_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := $(common_COPY_HEADERS)

include $(BUILD_STATIC_LIBRARY)

