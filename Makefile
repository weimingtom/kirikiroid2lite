#0: pc
#1: miyoo a30 and rg28xx
#2: trimui smart pro
#3: trimui brick
MIYOO := 0
#sudo apt install libglfw3-dev libfreetype-dev
#cocos/2d/CCFontFreeType.h

#tvpgl_arm.cpp: && !defined(LINUX)
#CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#rw |= O_BINARY;
#defined(LINUX)
#t = __strftime_fmt_1(&buf, &k, *f, tm);
#b src/core/tjs2/tjsConfig.cpp:2494
#b src/core/tjs2/tjsConfig.cpp:2512
#b src/core/tjs2/tjsConfig.cpp:1986


ifeq ($(MIYOO),3)
CC = /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-gcc
CPP := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-g++
AR := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ar cru
RANLIB := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ranlib 
else ifeq ($(MIYOO),2)
CC = /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-gcc
CPP := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-g++
AR := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ar cru
RANLIB := /home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-ranlib 
else ifeq ($(MIYOO),1)
CC = /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CPP := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
AR := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-ar cru
RANLIB := /home/wmt/work_a30/gcc-linaro-7.5.0-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-ranlib 
else
CC  := gcc
CPP := g++
AR  := ar cru
RANLIB := ranlib
endif
RM := rm -rf

CPPFLAGS := 
CPPFLAGS += -I.   
#CPPFLAGS += -g -O2
#CPPFLAGS += -g3 -O0

ifeq ($(MIYOO),3)
#trimui brick
CPPFLAGS += -DUSE_APP_WIDTH=1024
CPPFLAGS += -DUSE_APP_HEIGHT=768
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g0 -O3
else ifeq ($(MIYOO),2)
#trimui smart pro
CPPFLAGS += -DUSE_APP_WIDTH=1280
CPPFLAGS += -DUSE_APP_HEIGHT=720
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g0 -O3
else ifeq ($(MIYOO),1)
#miyoo a30 and rg28xx
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480
CPPFLAGS += -DUSE_NO_GLFW=1
CPPFLAGS += -DUSE_ROTATE90=1
CPPFLAGS += -g0 -O3
else
#pc
CPPFLAGS += -DUSE_APP_WIDTH=640
CPPFLAGS += -DUSE_APP_HEIGHT=480
CPPFLAGS += -DUSE_NO_GLFW=0 -DUSE_SHADER_PRECISION=1
#if PC use OpenGLES to replace OpenGL, need USE_SHADER_PRECISION
#cocos2d-x debug info [cocos2d: 0:1(1): error: No precision specified in this scope for type `mat4'
CPPFLAGS += -DUSE_ROTATE90=0
CPPFLAGS += -g3 -O0
endif

CPPFLAGS += -DCC_ENABLE_BOX2D_INTEGRATION=0 
CPPFLAGS += -DCC_USE_PHYSICS=0 

#CPPFLAGS += -DCC_USE_WEBP=1
CPPFLAGS += -DCC_USE_WEBP=0
CPPFLAGS += -DCC_USE_TIFF=0
#if USE_FONTCONFIG_LIB

CPPFLAGS += -DGLFW_DLL 
CPPFLAGS += -DLINUX 
CPPFLAGS += -DCOCOS2D_DEBUG=1

CPPFLAGS += -DMY_USE_FREESERIF=1
CPPFLAGS += -DMY_USE_UIBUTTON_SETTITLECOLOR=1

#see below
CPPFLAGS2 := 
CPPFLAGS2 += -std=c++11 

CPPFLAGS += -fno-exceptions 
CPPFLAGS += -fexceptions 

CPPFLAGS += -Wno-deprecated-declarations 
#only c:
#CPPFLAGS += -Wno-reorder 
CPPFLAGS += -Wall 

#################
#kirikiroid2
CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -fsigned-char
#only c: -frtti
#
#
#################

CPPFLAGS += -I. 
CPPFLAGS += -Icocos 
CPPFLAGS += -Icocos/platform 
#FIXME:
CPPFLAGS += -Icocos/platform/desktop 
CPPFLAGS += -Icocos/platform/linux 
CPPFLAGS += -Icocos/audio/include 
CPPFLAGS += -Icocos/editor-support
CPPFLAGS += -Ideprecated 
CPPFLAGS += -Iextensions 

CPPFLAGS += -Iexternal 
CPPFLAGS += -Iexternal/ConvertUTF 
CPPFLAGS += -Iexternal/poly2tri 
CPPFLAGS += -Iexternal/poly2tri/common 
CPPFLAGS += -Iexternal/poly2tri/sweep 
CPPFLAGS += -Iexternal/edtaa3func 
CPPFLAGS += -Iexternal/xxtea 
CPPFLAGS += -Iexternal/unzip 
CPPFLAGS += -Iexternal/tinyxml2 
CPPFLAGS += -Iexternal/xxhash 

CPPFLAGS += -Icocos/ui
CPPFLAGS += -Icocos/ui/UIEditBox
CPPFLAGS += -Icocos/editor-support/cocosbuilder
CPPFLAGS += -Icocos/editor-support/cocostudio
CPPFLAGS += -Icocos/editor-support/cocostudio/ActionTimeline
CPPFLAGS += -Icocos/editor-support/cocostudio/WidgetReader

#################
#kirikiroid2
CPPFLAGS += -I./Classes
CPPFLAGS += -I./extensions
CPPFLAGS += -I./cocos
CPPFLAGS += -I./cocos/editor-support
CPPFLAGS += -I./vendor/libgdiplus/src
CPPFLAGS += -I./vendor/google_breakpad/current/src
CPPFLAGS += -I./vendor/google_breakpad/current/src/common/android/include
CPPFLAGS += -I./src/core/environ
CPPFLAGS += -I./src/core/environ/android
CPPFLAGS += -I./src/core/tjs2
CPPFLAGS += -I./src/core/base
CPPFLAGS += -I./src/core/visual
CPPFLAGS += -I./src/core/visual/win32
CPPFLAGS += -I./src/core/sound
CPPFLAGS += -I./src/core/sound/win32
CPPFLAGS += -I./src/core/utils
CPPFLAGS += -I./src/plugins
CPPFLAGS += -I./src/core/base/win32
CPPFLAGS += -I./src/core/msg
CPPFLAGS += -I./src/core/msg/win32
CPPFLAGS += -I./src/core/utils/win32
CPPFLAGS += -I./src/core/environ/win32
CPPFLAGS += -I./src/core/extension
CPPFLAGS += -I./src/core
CPPFLAGS += -I./src/core/external/onig
CPPFLAGS += -I./src/core/external/opencv-2.4.13/modules/core/include
CPPFLAGS += -I./src/core/external/opencv-2.4.13/modules/dynamicuda/include
CPPFLAGS += -I./src/core/external/opencv-2.4.13/modules/imgproc/include
CPPFLAGS += -I./src/core/external/opencv-2.4.13
CPPFLAGS += -I./src/core/external/freetype-2.5.0.1/include

#vorbis
CPPFLAGS += -I./src/core/external/libvorbis-1.2.0/include/
CPPFLAGS += -I./src/core/external/libogg-1.1.3/include
CPPFLAGS += -I./src/core/external/libvorbis-1.2.0/lib

# FIXME: src/core/extension, two extension.h
# TODO: modify main.cpp	
CPPFLAGS += -DTJS_TEXT_OUT_CRLF 
#-DONIG_EXTERN=extern -DNOT_RUBY -DEXPORT

#kirikiroid2
#################




#CPPFLAGS += -I/usr/include/webp
#CPPFLAGS += -I/usr/include/i386-linux-gnu 
#CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/GLFW 
#CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/local/include/GLFW 

ifeq ($(MIYOO),3)
CPPFLAGS += -I/home/wmt/work_trimui/usr/include
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/webp
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/freetype2
else ifeq ($(MIYOO),2)
CPPFLAGS += -I/home/wmt/work_trimui/usr/include
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/webp
CPPFLAGS += -I/home/wmt/work_trimui/usr/include/freetype2
else ifeq ($(MIYOO),1)
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/webp
CPPFLAGS += -I/home/wmt/work_a30/staging_dir/target/usr/include/freetype2
else
#ubuntu 16 and xubuntu 20
#sudo apt install libglfw3-dev libpng-dev zlib1g-dev libjpeg-dev libfreetype-dev libbz2-dev
CPPFLAGS += -I/usr/include
CPPFLAGS += -I/usr/include/webp
CPPFLAGS += -I/usr/include/freetype2
endif



LDFLAGS := 
#LDFLAGS += -lGLU 

#LDFLAGS += -lGL 
#LDFLAGS += -lGLEW 
#LDFLAGS += -lglfw 
#LDFLAGS += -lfontconfig 
#LDFLAGS += -ltiff
#LDFLAGS += -lwebp
#LDFLAGS += -lwebsockets 
#LDFLAGS += -lcurl 
#LDFLAGS += -lX11 

LDFLAGS += -lfreetype -lbz2
#NOTE: -lbz2 is not necessary for PC xubuntu 20.04
LDFLAGS += -lpthread 
LDFLAGS += -lz 
LDFLAGS += -ljpeg 
LDFLAGS += -lpng 
LDFLAGS += -lGLESv2 -lEGL

#kirikiroid2
LDFLAGS += -latomic
LDFLAGS += -lSDL2

ifeq ($(MIYOO),3)
#trimui brick replace glfw3 with EGL
LDFLAGS += -lIMGegl -lsrv_um -lusc -lglslcompiler -L/home/wmt/work_trimui/usr/lib
else ifeq ($(MIYOO),2)
#trimui smart pro replace glfw3 with EGL
LDFLAGS += -lIMGegl -lsrv_um -lusc -lglslcompiler -L/home/wmt/work_trimui/usr/lib
else ifeq ($(MIYOO),1)
#miyoo a30 replace glfw3 with EGL
LDFLAGS += -L/home/wmt/work_a30/staging_dir/target/usr/lib
else
#xubuntu 16, need glfw3 and EGL
#sudo apt install libglfw3-dev libpng-dev zlib1g-dev libjpeg-dev libfreetype-dev libbz2-dev
LDFLAGS += -lglfw #cocos2dx/platform/linux/CCEGLView.cpp
endif


OBJS := 

#libcocos2d.a
OBJS += cocos/cocos2d.o

#2d
OBJS += cocos/2d/CCAction.o
OBJS += cocos/2d/CCActionCamera.o
OBJS += cocos/2d/CCActionCatmullRom.o
OBJS += cocos/2d/CCActionEase.o
OBJS += cocos/2d/CCActionGrid.o
OBJS += cocos/2d/CCActionGrid3D.o
OBJS += cocos/2d/CCActionInstant.o
OBJS += cocos/2d/CCActionInterval.o
OBJS += cocos/2d/CCActionManager.o
OBJS += cocos/2d/CCActionPageTurn3D.o
OBJS += cocos/2d/CCActionProgressTimer.o
OBJS += cocos/2d/CCActionTiledGrid.o
OBJS += cocos/2d/CCActionTween.o
OBJS += cocos/2d/CCAnimation.o
OBJS += cocos/2d/CCAnimationCache.o
OBJS += cocos/2d/CCAtlasNode.o
OBJS += cocos/2d/CCCamera.o
OBJS += cocos/2d/CCClippingNode.o
OBJS += cocos/2d/CCClippingRectangleNode.o
OBJS += cocos/2d/CCComponent.o
OBJS += cocos/2d/CCComponentContainer.o
OBJS += cocos/2d/CCDrawNode.o
OBJS += cocos/2d/CCDrawingPrimitives.o
OBJS += cocos/2d/CCFont.o
OBJS += cocos/2d/CCFontAtlas.o
OBJS += cocos/2d/CCFontAtlasCache.o
OBJS += cocos/2d/CCFontCharMap.o
OBJS += cocos/2d/CCFontFNT.o
OBJS += cocos/2d/CCFontFreeType.o
OBJS += cocos/2d/CCGLBufferedNode.o
OBJS += cocos/2d/CCGrabber.o
OBJS += cocos/2d/CCGrid.o
OBJS += cocos/2d/CCLabel.o
OBJS += cocos/2d/CCLabelAtlas.o
OBJS += cocos/2d/CCLabelBMFont.o
OBJS += cocos/2d/CCLabelTTF.o
OBJS += cocos/2d/CCLabelTextFormatter.o
OBJS += cocos/2d/CCLayer.o
OBJS += cocos/2d/CCLight.o
OBJS += cocos/2d/CCMenu.o
OBJS += cocos/2d/CCMenuItem.o
OBJS += cocos/2d/CCMotionStreak.o
OBJS += cocos/2d/CCNode.o
OBJS += cocos/2d/CCNodeGrid.o
OBJS += cocos/2d/CCParallaxNode.o
OBJS += cocos/2d/CCParticleBatchNode.o
OBJS += cocos/2d/CCParticleExamples.o
OBJS += cocos/2d/CCParticleSystem.o
OBJS += cocos/2d/CCParticleSystemQuad.o
OBJS += cocos/2d/CCProgressTimer.o
OBJS += cocos/2d/CCProtectedNode.o
OBJS += cocos/2d/CCRenderTexture.o
OBJS += cocos/2d/CCScene.o
OBJS += cocos/2d/CCSprite.o
OBJS += cocos/2d/CCSpriteBatchNode.o
OBJS += cocos/2d/CCSpriteFrame.o
OBJS += cocos/2d/CCSpriteFrameCache.o
OBJS += cocos/2d/MarchingSquare.o
OBJS += cocos/2d/SpritePolygon.o
OBJS += cocos/2d/SpritePolygonCache.o
OBJS += cocos/2d/CCTMXLayer.o
OBJS += cocos/2d/CCFastTMXLayer.o
OBJS += cocos/2d/CCTMXObjectGroup.o
OBJS += cocos/2d/CCTMXTiledMap.o
OBJS += cocos/2d/CCFastTMXTiledMap.o
OBJS += cocos/2d/CCTMXXMLParser.o
OBJS += cocos/2d/CCTextFieldTTF.o
OBJS += cocos/2d/CCTileMapAtlas.o
OBJS += cocos/2d/CCTransition.o
OBJS += cocos/2d/CCTransitionPageTurn.o
OBJS += cocos/2d/CCTransitionProgress.o
OBJS += cocos/2d/CCTweenFunction.o

OBJS += cocos/3d/CCFrustum.o
OBJS += cocos/3d/CCPlane.o
OBJS += cocos/platform/CCGLView.o
OBJS += cocos/platform/CCFileUtils.o
OBJS += cocos/platform/CCSAXParser.o
OBJS += cocos/platform/CCThread.o
OBJS += cocos/platform/CCImage.o

OBJS += cocos/platform/linux/CCApplication-linux.o
OBJS += cocos/platform/linux/CCCommon-linux.o
OBJS += cocos/platform/linux/CCDevice-linux.o
OBJS += cocos/platform/linux/CCFileUtils-linux.o
OBJS += cocos/platform/linux/CCStdC-linux.o

OBJS += cocos/platform/desktop/CCGLViewImpl-desktop.o

#math
OBJS += cocos/math/CCAffineTransform.o
OBJS += cocos/math/CCGeometry.o
OBJS += cocos/math/CCVertex.o
OBJS += cocos/math/MathUtil.o
OBJS += cocos/math/Mat4.o
OBJS += cocos/math/Quaternion.o
OBJS += cocos/math/TransformUtils.o
OBJS += cocos/math/Vec2.o
OBJS += cocos/math/Vec3.o
OBJS += cocos/math/Vec4.o

#base
OBJS += cocos/base/CCAsyncTaskPool.o
OBJS += cocos/base/CCAutoreleasePool.o
OBJS += cocos/base/CCConfiguration.o
OBJS += cocos/base/CCConsole.o
OBJS += cocos/base/CCData.o
OBJS += cocos/base/CCDataVisitor.o
OBJS += cocos/base/CCDirector.o
OBJS += cocos/base/CCEvent.o
OBJS += cocos/base/CCEventAcceleration.o
OBJS += cocos/base/CCEventCustom.o
OBJS += cocos/base/CCEventDispatcher.o
OBJS += cocos/base/CCEventFocus.o
OBJS += cocos/base/CCEventKeyboard.o
OBJS += cocos/base/CCEventController.o
OBJS += cocos/base/CCEventListener.o
OBJS += cocos/base/CCEventListenerController.o
OBJS += cocos/base/CCEventListenerAcceleration.o
OBJS += cocos/base/CCEventListenerCustom.o
OBJS += cocos/base/CCEventListenerFocus.o
OBJS += cocos/base/CCEventListenerKeyboard.o
OBJS += cocos/base/CCEventListenerMouse.o
OBJS += cocos/base/CCEventListenerTouch.o
OBJS += cocos/base/CCEventMouse.o
OBJS += cocos/base/CCEventTouch.o
OBJS += cocos/base/CCIMEDispatcher.o
OBJS += cocos/base/CCNS.o
OBJS += cocos/base/CCProfiling.o
OBJS += cocos/base/ccRandom.o
OBJS += cocos/base/CCRef.o
OBJS += cocos/base/CCScheduler.o
OBJS += cocos/base/CCScriptSupport.o
OBJS += cocos/base/CCTouch.o
OBJS += cocos/base/CCUserDefault.o
#OBJS += cocos/base/CCUserDefault-android.o
OBJS += cocos/base/CCValue.o
OBJS += cocos/base/TGAlib.o
OBJS += cocos/base/ZipUtils.o
OBJS += cocos/base/atitc.o
OBJS += cocos/base/base64.o
OBJS += cocos/base/ccCArray.o
OBJS += cocos/base/ccFPSImages.o
OBJS += cocos/base/ccTypes.o
OBJS += cocos/base/ccUTF8.o
OBJS += cocos/base/ccUtils.o
OBJS += cocos/base/etc1.o
OBJS += cocos/base/pvr.o
OBJS += cocos/base/s3tc.o
OBJS += cocos/base/CCController.o
#OBJS += cocos/base/CCController-android.o
OBJS += cocos/base/allocator/CCAllocatorDiagnostics.o
OBJS += cocos/base/allocator/CCAllocatorGlobal.o
OBJS += cocos/base/allocator/CCAllocatorGlobalNewDelete.o
OBJS += cocos/base/ObjectFactory.o

#renderer
OBJS += cocos/renderer/CCBatchCommand.o
OBJS += cocos/renderer/CCCustomCommand.o
OBJS += cocos/renderer/CCGLProgram.o
OBJS += cocos/renderer/CCGLProgramCache.o
OBJS += cocos/renderer/CCGLProgramState.o
OBJS += cocos/renderer/CCGLProgramStateCache.o
OBJS += cocos/renderer/CCGroupCommand.o
OBJS += cocos/renderer/CCQuadCommand.o
OBJS += cocos/renderer/CCMeshCommand.o
OBJS += cocos/renderer/CCRenderCommand.o
OBJS += cocos/renderer/CCRenderer.o
OBJS += cocos/renderer/CCTexture2D.o
OBJS += cocos/renderer/CCTextureAtlas.o
OBJS += cocos/renderer/CCTextureCache.o
OBJS += cocos/renderer/ccGLStateCache.o
OBJS += cocos/renderer/ccShaders.o
OBJS += cocos/renderer/CCVertexIndexBuffer.o
OBJS += cocos/renderer/CCVertexIndexData.o
OBJS += cocos/renderer/CCPrimitive.o
OBJS += cocos/renderer/CCPrimitiveCommand.o
OBJS += cocos/renderer/CCTrianglesCommand.o

#deprecated
OBJS += cocos/deprecated/CCArray.o
OBJS += cocos/deprecated/CCSet.o
OBJS += cocos/deprecated/CCString.o
OBJS += cocos/deprecated/CCDictionary.o
OBJS += cocos/deprecated/CCDeprecated.o
OBJS += cocos/deprecated/CCNotificationCenter.o

OBJS += external/ConvertUTF/ConvertUTFWrapper.o
OBJS += external/ConvertUTF/ConvertUTF.o

OBJS += external/edtaa3func/edtaa3func.o

OBJS += external/poly2tri/common/shapes.o
OBJS += external/poly2tri/sweep/advancing_front.o
OBJS += external/poly2tri/sweep/cdt.o
OBJS += external/poly2tri/sweep/sweep_context.o
OBJS += external/poly2tri/sweep/sweep.o

#LDFLAGS += ../../lib/libtinyxml2.a 
OBJS += external/tinyxml2/tinyxml2.o

#LDFLAGS += ../../lib/libunzip.a 
OBJS += external/unzip/ioapi_mem.o
OBJS += external/unzip/ioapi.o
OBJS += external/unzip/unzip.o

#LDFLAGS += ../../lib/libxxhash.a 
OBJS += external/xxhash/xxhash.o

#ui
OBJS += cocos/ui/UIWidget.o
OBJS += cocos/ui/UILayout.o
OBJS += cocos/ui/UILayoutParameter.o
OBJS += cocos/ui/UILayoutManager.o
OBJS += cocos/ui/CocosGUI.o
OBJS += cocos/ui/UIHelper.o
OBJS += cocos/ui/UIListView.o
OBJS += cocos/ui/UIPageView.o
OBJS += cocos/ui/UIScrollView.o
OBJS += cocos/ui/UIButton.o
OBJS += cocos/ui/UICheckBox.o
OBJS += cocos/ui/UIImageView.o
OBJS += cocos/ui/UIText.o
OBJS += cocos/ui/UITextAtlas.o
OBJS += cocos/ui/UITextBMFont.o
OBJS += cocos/ui/UILoadingBar.o
OBJS += cocos/ui/UISlider.o
OBJS += cocos/ui/UITextField.o
OBJS += cocos/ui/UIRichText.o
OBJS += cocos/ui/UIHBox.o
OBJS += cocos/ui/UIVBox.o
OBJS += cocos/ui/UIRelativeBox.o
OBJS += cocos/ui/UIVideoPlayer-android.o
OBJS += cocos/ui/UIDeprecated.o
OBJS += cocos/ui/UIScale9Sprite.o
OBJS += cocos/ui/UIWebView.o
OBJS += cocos/ui/UIWebViewImpl-android.o
OBJS += cocos/ui/UIEditBox/UIEditBox.o
OBJS += cocos/ui/UIEditBox/UIEditBoxImpl-android.o
OBJS += cocos/ui/UILayoutComponent.o

#cocosbuilder
OBJS += cocos/editor-support/cocosbuilder/CCBAnimationManager.o
OBJS += cocos/editor-support/cocosbuilder/CCBFileLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCBKeyframe.o
OBJS += cocos/editor-support/cocosbuilder/CCBReader.o
OBJS += cocos/editor-support/cocosbuilder/CCBSequence.o
OBJS += cocos/editor-support/cocosbuilder/CCBSequenceProperty.o
OBJS += cocos/editor-support/cocosbuilder/CCControlButtonLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCControlLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLabelBMFontLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLabelTTFLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerColorLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerGradientLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCLayerLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCMenuItemImageLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCMenuItemLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCNode+CCBRelativePositioning.o
OBJS += cocos/editor-support/cocosbuilder/CCNodeLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCNodeLoaderLibrary.o
OBJS += cocos/editor-support/cocosbuilder/CCParticleSystemQuadLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCScale9SpriteLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCScrollViewLoader.o
OBJS += cocos/editor-support/cocosbuilder/CCSpriteLoader.o

#cocosstudio
OBJS += cocos/editor-support/cocostudio/CCActionFrame.o
OBJS += cocos/editor-support/cocostudio/CCActionFrameEasing.o
OBJS += cocos/editor-support/cocostudio/CCActionManagerEx.o
OBJS += cocos/editor-support/cocostudio/CCActionNode.o
OBJS += cocos/editor-support/cocostudio/CCActionObject.o
OBJS += cocos/editor-support/cocostudio/CCArmature.o
OBJS += cocos/editor-support/cocostudio/CCBone.o
OBJS += cocos/editor-support/cocostudio/CCArmatureAnimation.o
OBJS += cocos/editor-support/cocostudio/CCProcessBase.o
OBJS += cocos/editor-support/cocostudio/CCTween.o
OBJS += cocos/editor-support/cocostudio/CCDatas.o
OBJS += cocos/editor-support/cocostudio/CCBatchNode.o
OBJS += cocos/editor-support/cocostudio/CCDecorativeDisplay.o
OBJS += cocos/editor-support/cocostudio/CCDisplayFactory.o
OBJS += cocos/editor-support/cocostudio/CCDisplayManager.o
OBJS += cocos/editor-support/cocostudio/CCSkin.o
OBJS += cocos/editor-support/cocostudio/CCColliderDetector.o
OBJS += cocos/editor-support/cocostudio/CCArmatureDataManager.o
OBJS += cocos/editor-support/cocostudio/CCArmatureDefine.o
OBJS += cocos/editor-support/cocostudio/CCDataReaderHelper.o
OBJS += cocos/editor-support/cocostudio/CCSpriteFrameCacheHelper.o
OBJS += cocos/editor-support/cocostudio/CCTransformHelp.o
OBJS += cocos/editor-support/cocostudio/CCUtilMath.o
OBJS += cocos/editor-support/cocostudio/CCComAttribute.o
OBJS += cocos/editor-support/cocostudio/CCComAudio.o
OBJS += cocos/editor-support/cocostudio/CCComController.o
OBJS += cocos/editor-support/cocostudio/CCComRender.o
OBJS += cocos/editor-support/cocostudio/CCInputDelegate.o
OBJS += cocos/editor-support/cocostudio/DictionaryHelper.o
OBJS += cocos/editor-support/cocostudio/CCSGUIReader.o
OBJS += cocos/editor-support/cocostudio/CCSSceneReader.o
OBJS += cocos/editor-support/cocostudio/TriggerBase.o
OBJS += cocos/editor-support/cocostudio/TriggerMng.o
OBJS += cocos/editor-support/cocostudio/TriggerObj.o
OBJS += cocos/editor-support/cocostudio/CocoLoader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/NodeReader/NodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SingleNodeReader/SingleNodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SpriteReader/SpriteReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ParticleReader/ParticleReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/GameMapReader/GameMapReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ProjectNodeReader/ProjectNodeReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ComAudioReader/ComAudioReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/WidgetReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ButtonReader/ButtonReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/CheckBoxReader/CheckBoxReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ImageViewReader/ImageViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/LayoutReader/LayoutReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ListViewReader/ListViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/LoadingBarReader/LoadingBarReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/PageViewReader/PageViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ScrollViewReader/ScrollViewReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/SliderReader/SliderReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextAtlasReader/TextAtlasReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextBMFontReader/TextBMFontReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextFieldReader/TextFieldReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/TextReader/TextReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/Node3DReader/Node3DReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/Sprite3DReader/Sprite3DReader.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/UserCameraReader/UserCameraReader.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimelineCache.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCFrame.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCTimeLine.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimeline.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CCActionTimelineNode.o
OBJS += cocos/editor-support/cocostudio/ActionTimeline/CSLoader.o
OBJS += cocos/editor-support/cocostudio/FlatBuffersSerialize.o
OBJS += cocos/editor-support/cocostudio/WidgetCallBackHandlerProtocol.o
OBJS += cocos/editor-support/cocostudio/WidgetReader/ArmatureNodeReader/ArmatureNodeReader.o
OBJS += cocos/editor-support/cocostudio/CCObjectExtensionData.o
OBJS += cocos/editor-support/cocostudio/CocoStudio.o

#3d
OBJS += cocos/3d/CCRay.o
OBJS += cocos/3d/CCAABB.o
OBJS += cocos/3d/CCOBB.o
OBJS += cocos/3d/CCAnimate3D.o
OBJS += cocos/3d/CCAnimation3D.o
OBJS += cocos/3d/CCAttachNode.o
OBJS += cocos/3d/CCBillBoard.o
OBJS += cocos/3d/CCBundle3D.o
OBJS += cocos/3d/CCBundleReader.o
OBJS += cocos/3d/CCMesh.o
OBJS += cocos/3d/CCMeshSkin.o
OBJS += cocos/3d/CCMeshVertexIndexData.o
OBJS += cocos/3d/CCSprite3DMaterial.o
OBJS += cocos/3d/CCObjLoader.o
OBJS += cocos/3d/CCSkeleton3D.o
OBJS += cocos/3d/CCSprite3D.o
OBJS += cocos/3d/CCTerrain.o
OBJS += cocos/3d/CCSkybox.o
OBJS += cocos/3d/CCTextureCube.o

#extensions ScrollView
OBJS += extensions/GUI/CCControlExtension/CCControl.o
OBJS += extensions/GUI/CCControlExtension/CCControlButton.o
OBJS += extensions/GUI/CCControlExtension/CCControlColourPicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlHuePicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlPotentiometer.o
OBJS += extensions/GUI/CCControlExtension/CCControlSaturationBrightnessPicker.o
OBJS += extensions/GUI/CCControlExtension/CCControlSlider.o
OBJS += extensions/GUI/CCControlExtension/CCControlStepper.o
OBJS += extensions/GUI/CCControlExtension/CCControlSwitch.o
OBJS += extensions/GUI/CCControlExtension/CCControlUtils.o
OBJS += extensions/GUI/CCControlExtension/CCInvocation.o
OBJS += extensions/GUI/CCScrollView/CCScrollView.o
OBJS += extensions/GUI/CCScrollView/CCTableView.o
OBJS += extensions/GUI/CCScrollView/CCTableViewCell.o

#################
#kirikiroid2
KIRIKIROID2_OBJS :=
#KIRIKIROID2_OBJS += ./proj.linux/main.o

KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/algorithm.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/alloc.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/arithm.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/array.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/cmdparser.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/convert.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/copy.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/datastructs.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/drawing.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/dxt.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/gl_core_3_1.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/glob.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/gpumat.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/lapack.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/mathfuncs.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/matmul.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/matop.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/matrix.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/opengl_interop.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/opengl_interop_deprecated.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/out.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/parallel.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/persistence.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/rand.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/stat.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/system.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/core/src/tables_core.o

KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/avx/imgwarp_avx.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/avx2/imgwarp_avx2.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/accum.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/approx.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/canny.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/clahe.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/color.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/contours.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/convhull.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/corner.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/cornersubpix.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/deriv.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/distransform.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/emd.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/featureselect.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/filter.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/floodfill.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/gabor.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/generalized_hough.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/geometry.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/grabcut.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/histogram.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/hough.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/imgwarp.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/linefit.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/matchcontours.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/moments.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/morph.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/phasecorr.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/pyramids.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/rotcalipers.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/samplers.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/segmentation.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/shapedescr.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/smooth.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/subdivision2d.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/sumpixels.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/tables.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/templmatch.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/thresh.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/undistort.o
KIRIKIROID2_OBJS += ./src/core/external/opencv-2.4.13/modules/imgproc/src/utils.o

KIRIKIROID2_OBJS += ./src/core/external/onig/enc/ascii.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/big5.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/cp1251.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/euc_jp.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/euc_kr.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/euc_tw.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/gb18030.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_1.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_2.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_3.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_4.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_5.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_6.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_7.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_8.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_9.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_10.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_11.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_13.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_14.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_15.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/iso8859_16.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/koi8.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/koi8_r.o
#KIRIKIROID2_OBJS += ./src/core/external/onig/enc/mktable.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/sjis.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/unicode.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/utf8.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/utf16_be.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/utf16_le.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/utf32_be.o
KIRIKIROID2_OBJS += ./src/core/external/onig/enc/utf32_le.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regcomp.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regenc.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regerror.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regexec.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regext.o
KIRIKIROID2_OBJS += ./src/core/external/onig/reggnu.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regparse.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regposerr.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regposix.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regsyntax.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regtrav.o
KIRIKIROID2_OBJS += ./src/core/external/onig/regversion.o
KIRIKIROID2_OBJS += ./src/core/external/onig/st.o

KIRIKIROID2_OBJS += ./src/core/visual/ARM/tvpgl_arm.o

KIRIKIROID2_OBJS += ./src/core/visual/gl/blend_function.o
KIRIKIROID2_OBJS += ./src/core/visual/gl/ResampleImage.o
KIRIKIROID2_OBJS += ./src/core/visual/gl/WeightFunctor.o

KIRIKIROID2_OBJS += ./src/core/visual/ogl/astcrt.o
KIRIKIROID2_OBJS += ./src/core/visual/ogl/etcpak.o
KIRIKIROID2_OBJS += ./src/core/visual/ogl/imagepacker.o
KIRIKIROID2_OBJS += ./src/core/visual/ogl/pvrtc.o
KIRIKIROID2_OBJS += ./src/core/visual/ogl/RenderManager_ogl.o


KIRIKIROID2_OBJS += ./src/core/visual/win32/BasicDrawDevice.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/BitmapBitsAlloc.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/BitmapInfomation.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/DInputMgn.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/DrawDevice.o
## KIRIKIROID2_OBJS += ./src/core/visual/win32/GDIFontRasterizer.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/GraphicsLoaderImpl.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/LayerBitmapImpl.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/LayerImpl.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/MenuItemImpl.o
## KIRIKIROID2_OBJS += ./src/core/visual/win32/NativeFreeTypeFace.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/PassThroughDrawDevice.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/TVPScreen.o
## KIRIKIROID2_OBJS += ./src/core/visual/win32/TVPSysFont.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/VideoOvlImpl.o
## KIRIKIROID2_OBJS += ./src/core/visual/win32/VSyncTimingThread.o
KIRIKIROID2_OBJS += ./src/core/visual/win32/WindowImpl.o

KIRIKIROID2_OBJS += ./src/core/visual/argb.o
KIRIKIROID2_OBJS += ./src/core/visual/BitmapIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/BitmapLayerTreeOwner.o
KIRIKIROID2_OBJS += ./src/core/visual/CharacterData.o
KIRIKIROID2_OBJS += ./src/core/visual/ComplexRect.o
KIRIKIROID2_OBJS += ./src/core/visual/FontImpl.o
KIRIKIROID2_OBJS += ./src/core/visual/FontSystem.o
KIRIKIROID2_OBJS += ./src/core/visual/FreeType.o
KIRIKIROID2_OBJS += ./src/core/visual/FreeTypeFontRasterizer.o
KIRIKIROID2_OBJS += ./src/core/visual/GraphicsLoaderIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/GraphicsLoadThread.o
KIRIKIROID2_OBJS += ./src/core/visual/ImageFunction.o
KIRIKIROID2_OBJS += ./src/core/visual/LayerBitmapIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/LayerIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/LayerManager.o
KIRIKIROID2_OBJS += ./src/core/visual/LayerTreeOwnerImpl.o
## KIRIKIROID2_OBJS += ./src/core/visual/LoadBPG.o
KIRIKIROID2_OBJS += ./src/core/visual/LoadJPEG.o
## KIRIKIROID2_OBJS += ./src/core/visual/LoadJXR.o
KIRIKIROID2_OBJS += ./src/core/visual/LoadPNG.o
## KIRIKIROID2_OBJS += ./src/core/visual/LoadPVRv3.o
KIRIKIROID2_OBJS += ./src/core/visual/LoadTLG.o
## KIRIKIROID2_OBJS += ./src/core/visual/LoadWEBP.o
KIRIKIROID2_OBJS += ./src/core/visual/MenuItemIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/PrerenderedFont.o
KIRIKIROID2_OBJS += ./src/core/visual/RectItf.o
KIRIKIROID2_OBJS += ./src/core/visual/RenderManager.o
KIRIKIROID2_OBJS += ./src/core/visual/SaveTLG5.o
KIRIKIROID2_OBJS += ./src/core/visual/SaveTLG6.o
KIRIKIROID2_OBJS += ./src/core/visual/TransIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/tvpgl.o
KIRIKIROID2_OBJS += ./src/core/visual/VideoOvlIntf.o
KIRIKIROID2_OBJS += ./src/core/visual/WindowIntf.o


KIRIKIROID2_OBJS += ./src/core/utils/encoding/gbk2unicode.o
KIRIKIROID2_OBJS += ./src/core/utils/encoding/jis2unicode.o

KIRIKIROID2_OBJS += ./src/core/utils/minizip/ioapi.o
KIRIKIROID2_OBJS += ./src/core/utils/minizip/unzip.o
KIRIKIROID2_OBJS += ./src/core/utils/minizip/zip.o

KIRIKIROID2_OBJS += ./src/core/utils/win32/ClipboardImpl.o
KIRIKIROID2_OBJS += ./src/core/utils/win32/DebugImpl.o
KIRIKIROID2_OBJS += ./src/core/utils/win32/PadImpl.o
KIRIKIROID2_OBJS += ./src/core/utils/win32/ThreadImpl.o
KIRIKIROID2_OBJS += ./src/core/utils/win32/TimerImpl.o
KIRIKIROID2_OBJS += ./src/core/utils/win32/TVPTimer.o

KIRIKIROID2_OBJS += ./src/core/utils/ClipboardIntf.o
KIRIKIROID2_OBJS += ./src/core/utils/DebugIntf.o
KIRIKIROID2_OBJS += ./src/core/utils/KAGParser.o
KIRIKIROID2_OBJS += ./src/core/utils/MathAlgorithms_Default.o
KIRIKIROID2_OBJS += ./src/core/utils/md5.o
KIRIKIROID2_OBJS += ./src/core/utils/MiscUtility.o
KIRIKIROID2_OBJS += ./src/core/utils/PadIntf.o
KIRIKIROID2_OBJS += ./src/core/utils/Random.o
KIRIKIROID2_OBJS += ./src/core/utils/RealFFT_Default.o
KIRIKIROID2_OBJS += ./src/core/utils/ThreadIntf.o
KIRIKIROID2_OBJS += ./src/core/utils/TickCount.o
KIRIKIROID2_OBJS += ./src/core/utils/TimerIntf.o
KIRIKIROID2_OBJS += ./src/core/utils/VelocityTracker.o


KIRIKIROID2_OBJS += ./src/core/tjs2/tjs.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjs.tab.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsArray.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsBinarySerializer.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsByteCodeLoader.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsCompileControl.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsConfig.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsConstArrayData.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsDate.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsdate.tab.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsDateParser.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsDebug.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsDictionary.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsDisassemble.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsError.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsException.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsGlobalStringMap.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsInterCodeExec.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsInterCodeGen.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsInterface.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsLex.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsMath.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsMessage.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsMT19937ar-cok.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsNamespace.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsNative.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsObject.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsObjectExtendable.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsOctPack.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjspp.tab.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsRandomGenerator.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsRegExp.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsScriptBlock.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsScriptCache.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsString.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsUtils.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsVariant.o
KIRIKIROID2_OBJS += ./src/core/tjs2/tjsVariantString.o

KIRIKIROID2_OBJS += ./src/core/sound/CDDAIntf.o
KIRIKIROID2_OBJS += ./src/core/sound/MathAlgorithms.o
KIRIKIROID2_OBJS += ./src/core/sound/MIDIIntf.o
KIRIKIROID2_OBJS += ./src/core/sound/PhaseVocoderFilter.o #exclude
KIRIKIROID2_OBJS += ./src/core/sound/SoundBufferBaseIntf.o
KIRIKIROID2_OBJS += ./src/core/sound/WaveFormatConverter.o
KIRIKIROID2_OBJS += ./src/core/sound/WaveIntf.o
KIRIKIROID2_OBJS += ./src/core/sound/WaveLoopManager.o
KIRIKIROID2_OBJS += ./src/core/sound/WaveSegmentQueue.o

KIRIKIROID2_OBJS += ./src/core/sound/win32/CDDAImpl.o
KIRIKIROID2_OBJS += ./src/core/sound/win32/MIDIImpl.o
KIRIKIROID2_OBJS += ./src/core/sound/win32/SoundBufferBaseImpl.o
KIRIKIROID2_OBJS += ./src/core/sound/win32/tvpsnd.o
KIRIKIROID2_OBJS += ./src/core/sound/win32/WaveImpl.o
KIRIKIROID2_OBJS += ./src/core/sound/win32/WaveMixer.o

KIRIKIROID2_OBJS += ./src/core/msg/MsgIntf.o

KIRIKIROID2_OBJS += ./src/core/msg/win32/MsgImpl.o
# KIRIKIROID2_OBJS += ./src/core/msg/win32/MsgLoad.o
KIRIKIROID2_OBJS += ./src/core/msg/win32/OptionsDesc.o
# KIRIKIROID2_OBJS += ./src/core/msg/win32/ReadOptionDesc.o

KIRIKIROID2_OBJS += ./src/core/movie/krmovie.o

KIRIKIROID2_OBJS += ./src/core/extension/Extension.o

KIRIKIROID2_OBJS += ./src/core/environ/win32/SystemControl.o

KIRIKIROID2_OBJS += ./src/core/environ/android/AndroidUtils.o

KIRIKIROID2_OBJS += ./src/core/environ/cocos2d/AppDelegate.o
KIRIKIROID2_OBJS += ./src/core/environ/cocos2d/CustomFileUtils.o
KIRIKIROID2_OBJS += ./src/core/environ/cocos2d/MainScene.o
# KIRIKIROID2_OBJS += ./src/core/environ/cocos2d/YUVSprite.o #excluded

KIRIKIROID2_OBJS += ./src/core/environ/ConfigManager/GlobalConfigManager.o
KIRIKIROID2_OBJS += ./src/core/environ/ConfigManager/IndividualConfigManager.o
KIRIKIROID2_OBJS += ./src/core/environ/ConfigManager/LocaleConfigManager.o

KIRIKIROID2_OBJS += ./src/core/environ/linux/Platform.o

# KIRIKIROID2_OBJS += ./src/core/environ/sdl/tvpsdl.o

KIRIKIROID2_OBJS += ./src/core/environ/ui/BaseForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/ConsoleWindow.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/DebugViewLayerForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/FileSelectorForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/GameMainMenu.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/GlobalPreferenceForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/IndividualPreferenceForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/InGameMenuForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/MainFileSelectorForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/MessageBox.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/PreferenceForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/SeletListForm.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/TipsHelpForm.o

KIRIKIROID2_OBJS += ./src/core/environ/ui/extension/ActionExtension.o
KIRIKIROID2_OBJS += ./src/core/environ/ui/extension/UIExtension.o

# KIRIKIROID2_OBJS += ./src/core/environ/win32/xxxxxxxxx.o

KIRIKIROID2_OBJS += ./src/core/environ/Application.o
KIRIKIROID2_OBJS += ./src/core/environ/DetectCPU.o
KIRIKIROID2_OBJS += ./src/core/environ/DumpSend.o

KIRIKIROID2_OBJS += ./src/core/base/BinaryStream.o
KIRIKIROID2_OBJS += ./src/core/base/CharacterSet.o
KIRIKIROID2_OBJS += ./src/core/base/EventIntf.o
KIRIKIROID2_OBJS += ./src/core/base/PluginIntf.o
KIRIKIROID2_OBJS += ./src/core/base/ScriptMgnIntf.o
KIRIKIROID2_OBJS += ./src/core/base/StorageIntf.o
KIRIKIROID2_OBJS += ./src/core/base/SysInitIntf.o
KIRIKIROID2_OBJS += ./src/core/base/SystemIntf.o
KIRIKIROID2_OBJS += ./src/core/base/TARArchive.o
KIRIKIROID2_OBJS += ./src/core/base/TextStream.o
KIRIKIROID2_OBJS += ./src/core/base/UtilStreams.o
KIRIKIROID2_OBJS += ./src/core/base/XP3Archive.o
KIRIKIROID2_OBJS += ./src/core/base/ZIPArchive.o

KIRIKIROID2_OBJS += ./src/core/base/win32/EventImpl.o
KIRIKIROID2_OBJS += ./src/core/base/win32/FileSelector.o
KIRIKIROID2_OBJS += ./src/core/base/win32/NativeEventQueue.o
KIRIKIROID2_OBJS += ./src/core/base/win32/PluginImpl.o
KIRIKIROID2_OBJS += ./src/core/base/win32/ScriptMgnImpl.o
KIRIKIROID2_OBJS += ./src/core/base/win32/StorageImpl.o
KIRIKIROID2_OBJS += ./src/core/base/win32/SysInitImpl.o
KIRIKIROID2_OBJS += ./src/core/base/win32/SystemImpl.o

KIRIKIROID2_OBJS += ./src/plugins/ncbind/ncbind.o
KIRIKIROID2_OBJS += ./src/plugins/addFont.o
KIRIKIROID2_OBJS += ./src/plugins/csvParser.o
KIRIKIROID2_OBJS += ./src/plugins/dirlist.o
KIRIKIROID2_OBJS += ./src/plugins/fftgraph.o
KIRIKIROID2_OBJS += ./src/plugins/getabout.o
KIRIKIROID2_OBJS += ./src/plugins/getSample.o
KIRIKIROID2_OBJS += ./src/plugins/InternalPlugins.o
KIRIKIROID2_OBJS += ./src/plugins/LayerExBase.o
KIRIKIROID2_OBJS += ./src/plugins/layerExPerspective.o
KIRIKIROID2_OBJS += ./src/plugins/saveStruct.o
KIRIKIROID2_OBJS += ./src/plugins/varfile.o
KIRIKIROID2_OBJS += ./src/plugins/win32dialog.o
KIRIKIROID2_OBJS += ./src/plugins/wutcwf.o
KIRIKIROID2_OBJS += ./src/plugins/xp3filter.o

KIRIKIROID2_OBJS += ./Classes/AppDelegate_ori.o
KIRIKIROID2_OBJS += ./Classes/HelloWorldScene.o
#see KIRIKIROID2_OBJS += ./src/core/environ/cocos2d/AppDelegate.o

#vorbis
KIRIKIROID2_OBJS += ./src/core/sound/VorbisWaveDecoder.o
KIRIKIROID2_OBJS += ./src/core/external/libogg-1.1.3/src/bitwise.o
KIRIKIROID2_OBJS += ./src/core/external/libogg-1.1.3/src/framing.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/analysis.o
#KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/barkmel.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/bitrate.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/block.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/codebook.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/envelope.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/floor0.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/floor1.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/info.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/lookup.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/lpc.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/lsp.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/mapping0.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/mdct.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/psy.o
#KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/psytune.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/registry.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/res0.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/sharedbook.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/smallft.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/synthesis.o
#KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/tone.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/vorbisenc.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/vorbisfile.o
KIRIKIROID2_OBJS += ./src/core/external/libvorbis-1.2.0/lib/window.o

#kirikiroid2
#################



all : kirikiroid2

cocos2dx.a : $(OBJS)
	$(AR) $@ $(OBJS)
	$(RANLIB) $@

kirikiroid2: cocos2dx.a $(KIRIKIROID2_OBJS)
	$(CPP) ./proj.linux/main.cpp $(KIRIKIROID2_OBJS) cocos2dx.a -o $@ -I./Classes $(CPPFLAGS) $(LDFLAGS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.cc
	$(CPP) $(CPPFLAGS2) $(CPPFLAGS) -o $@ -c $<

%.o : %.c
	$(CC) $(CPPFLAGS) -o $@ -c $<
	
test:
	./kirikiroid2

debug:
	gdb ./kirikiroid2

#b Button::setTitleColor

#search Trying to read XP3, SystemInformationFrom
#b XP3Archive.cpp:359

#
##0  tTVPXP3Archive::Init
#    at src/core/base/XP3Archive.cpp:359
##1  tTVPXP3Archive::tTVPXP3Archive
#    at src/core/base/XP3Archive.cpp:561
##2  tTVPXP3Archive::Create
#    at src/core/base/XP3Archive.cpp:580
##3  TVPOpenArchive
#    at src/core/base/win32/StorageImpl.cpp:743
##4  TVPCheckArchive
#    at src/core/base/win32/StorageImpl.cpp:756
##5  TVPMainFileSelectorForm::onCellClicked (
#    at src/core/environ/ui/MainFileSelectorForm.cpp:214
#

#MainFileSelctorForm.cpp
#void TVPMainFileSelectorForm::doStartup(const std::string &path) {
#TVPMainScene::GetInstance()->startupFrom(path)

#b TVPMainScene::startupFrom

#
#void TVPMainFileSelectorForm::show() {
#after ListDir
#TVPMainScene::GetInstance()->scheduleOnce([this](float dt){
#		//startup("/mnt/SDCARD/Data.xp3");
#		TVPMainScene::GetInstance()->startupFrom("/mnt/SDCARD/Data.xp3");
#	}, 0, "hellodemo");
#

clean:
	$(RM) $(OBJS) $(KIRIKIROID2_OBJS) cocos2dx.a kirikiroid2

debug2:
	/home/wmt/work_trimui/aarch64-linux-gnu-7.5.0-linaro/bin/aarch64-linux-gnu-gdb

#sudo apt install libncurses-dev	
#sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so /usr/lib/x86_64-linux-gnu/libncurses.so.5
#gdb not usable


