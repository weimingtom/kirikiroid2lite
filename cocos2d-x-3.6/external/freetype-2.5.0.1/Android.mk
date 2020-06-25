LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_MODULE := freetype
LOCAL_MODULE := cocos_freetype2_static
LOCAL_MODULE_FILENAME := freetype2
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

APP_SUBDIRS := src/autofit src/base src/bdf src/cache 
APP_SUBDIRS += src/cff src/cid src/gxvalid src/gzip 
APP_SUBDIRS += src/lzw src/otvalid src/pcf src/pfr 
APP_SUBDIRS += src/psaux src/pshinter src/psnames src/raster 
APP_SUBDIRS += src/sfnt src/smooth src/truetype src/type1 
APP_SUBDIRS += src/type42 src/winfonts

# Add more subdirs here, like src/subdir1 src/subdir2

LOCAL_C_INCLUDES := $(foreach D, $(APP_SUBDIRS), $(LOCAL_PATH)/$(D)) $(LOCAL_PATH)/include

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) \
	-DFT2_BUILD_LIBRARY

#Change C++ file extension as appropriate
LOCAL_CPP_EXTENSION := .cpp

#LOCAL_SRC_FILES := $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.cpp))))
# Uncomment to also add C sources
#LOCAL_SRC_FILES += $(foreach F, $(APP_SUBDIRS), $(addprefix $(F)/,$(notdir $(wildcard $(LOCAL_PATH)/$(F)/*.c))))
LOCAL_SRC_FILES := 
LOCAL_SRC_FILES += src/autofit/autofit.c
LOCAL_SRC_FILES += src/base/ftbase.c
LOCAL_SRC_FILES += src/bdf/bdf.c
LOCAL_SRC_FILES += src/cache/ftcache.c
LOCAL_SRC_FILES += src/cff/cff.c
LOCAL_SRC_FILES += src/cid/type1cid.c
LOCAL_SRC_FILES += src/gxvalid/gxvalid.c
LOCAL_SRC_FILES += src/gzip/ftgzip.c
LOCAL_SRC_FILES += src/lzw/ftlzw.c src/lzw/ftzopen.c
LOCAL_SRC_FILES += src/otvalid/otvalid.c
LOCAL_SRC_FILES += src/pcf/pcf.c
LOCAL_SRC_FILES += src/pfr/pfr.c
LOCAL_SRC_FILES += src/psaux/psaux.c
LOCAL_SRC_FILES += src/pshinter/pshinter.c
LOCAL_SRC_FILES += src/psnames/psnames.c
LOCAL_SRC_FILES += src/raster/raster.c
LOCAL_SRC_FILES += src/sfnt/sfnt.c
LOCAL_SRC_FILES += src/smooth/smooth.c
#LOCAL_SRC_FILES += src/tools
LOCAL_SRC_FILES += src/truetype/truetype.c
LOCAL_SRC_FILES += src/type1/type1.c
LOCAL_SRC_FILES += src/type42/type42.c
LOCAL_SRC_FILES += src/winfonts/winfnt.c

LOCAL_SRC_FILES += src/base/ftsystem.c src/base/ftdebug.c src/base/ftinit.c
LOCAL_SRC_FILES += src/base/ftbbox.c src/base/ftbdf.c
LOCAL_SRC_FILES += src/base/ftbitmap.c src/base/ftcid.c src/base/ftfstype.c
LOCAL_SRC_FILES += src/base/ftgasp.c src/base/ftglyph.c src/base/ftgxval.c
LOCAL_SRC_FILES += src/base/ftlcdfil.c src/base/ftmm.c src/base/ftotval.c
LOCAL_SRC_FILES += src/base/ftpatent.c src/base/ftpfr.c src/base/ftstroke.c
LOCAL_SRC_FILES += src/base/ftsynth.c src/base/fttype1.c src/base/ftwinfnt.c
LOCAL_SRC_FILES += src/base/ftxf86.c 


LOCAL_SHARED_LIBRARIES := 

LOCAL_STATIC_LIBRARIES := 

#LOCAL_LDLIBS := -lz
#LOCAL_EXPORT_LDLIBS := -lz

include $(BUILD_STATIC_LIBRARY)
