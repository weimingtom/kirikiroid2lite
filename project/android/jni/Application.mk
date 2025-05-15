#APP_STL := gnustl_static
#for NDK25
APP_STL := c++_static


APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char -fpermissive
#-fpermissive for c++_static for cocos2d-x-3.6/extensions/assets-manager/AssetsManagerEx.cpp 
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif

#APP_ABI := armeabi
#for NDK25
APP_ABI := armeabi-v7a
