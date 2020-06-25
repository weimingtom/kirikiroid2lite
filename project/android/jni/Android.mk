LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cpp_empty_test_shared

LOCAL_MODULE_FILENAME := libcpp_empty_test

LOCAL_SRC_FILES := main.cpp


LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/algorithm.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/alloc.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/arithm.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/array.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/cmdparser.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/convert.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/copy.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/datastructs.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/drawing.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/dxt.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/gl_core_3_1.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/glob.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/gpumat.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/lapack.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/mathfuncs.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/matmul.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/matop.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/matrix.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/opengl_interop.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/opengl_interop_deprecated.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/out.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/parallel.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/persistence.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/rand.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/stat.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/system.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/core/src/tables_core.cpp


LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/avx/imgwarp_avx.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/avx2/imgwarp_avx2.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/accum.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/approx.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/canny.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/clahe.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/color.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/contours.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/convhull.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/corner.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/cornersubpix.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/deriv.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/distransform.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/emd.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/featureselect.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/filter.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/floodfill.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/gabor.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/generalized_hough.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/geometry.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/grabcut.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/histogram.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/hough.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/imgwarp.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/linefit.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/matchcontours.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/moments.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/morph.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/phasecorr.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/pyramids.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/rotcalipers.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/samplers.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/segmentation.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/shapedescr.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/smooth.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/subdivision2d.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/sumpixels.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/tables.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/templmatch.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/thresh.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/undistort.cpp
LOCAL_SRC_FILES += ../../../src/core/external/opencv-2.4.13/modules/imgproc/src/utils.cpp

LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/ascii.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/big5.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/cp1251.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/euc_jp.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/euc_kr.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/euc_tw.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/gb18030.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_1.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_2.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_3.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_4.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_5.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_6.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_7.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_8.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_9.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_10.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_11.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_13.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_14.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_15.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/iso8859_16.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/koi8.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/koi8_r.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/mktable.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/sjis.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/unicode.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/utf8.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/utf16_be.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/utf16_le.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/utf32_be.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/enc/utf32_le.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regcomp.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regenc.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regerror.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regexec.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regext.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/reggnu.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regparse.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regposerr.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regposix.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regsyntax.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regtrav.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/regversion.c
LOCAL_SRC_FILES += ../../../src/core/external/onig/st.c

LOCAL_SRC_FILES += ../../../src/core/visual/ARM/tvpgl_arm.cpp

LOCAL_SRC_FILES += ../../../src/core/visual/gl/blend_function.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/gl/ResampleImage.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/gl/WeightFunctor.cpp

LOCAL_SRC_FILES += ../../../src/core/visual/ogl/astcrt.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ogl/etcpak.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ogl/imagepacker.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ogl/pvrtc.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ogl/RenderManager_ogl.cpp


LOCAL_SRC_FILES += ../../../src/core/visual/win32/BasicDrawDevice.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/BitmapBitsAlloc.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/BitmapInfomation.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/DInputMgn.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/DrawDevice.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/win32/GDIFontRasterizer.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/GraphicsLoaderImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/LayerBitmapImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/LayerImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/MenuItemImpl.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/win32/NativeFreeTypeFace.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/PassThroughDrawDevice.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/TVPScreen.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/win32/TVPSysFont.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/VideoOvlImpl.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/win32/VSyncTimingThread.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/win32/WindowImpl.cpp

LOCAL_SRC_FILES += ../../../src/core/visual/argb.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/BitmapIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/BitmapLayerTreeOwner.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/CharacterData.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ComplexRect.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/FontImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/FontSystem.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/FreeType.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/FreeTypeFontRasterizer.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/GraphicsLoaderIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/GraphicsLoadThread.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/ImageFunction.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LayerBitmapIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LayerIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LayerManager.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LayerTreeOwnerImpl.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/LoadBPG.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LoadJPEG.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/LoadJXR.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LoadPNG.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/LoadPVRv3.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/LoadTLG.cpp
## LOCAL_SRC_FILES += ../../../src/core/visual/LoadWEBP.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/MenuItemIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/PrerenderedFont.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/RectItf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/RenderManager.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/SaveTLG5.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/SaveTLG6.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/TransIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/tvpgl.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/VideoOvlIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/visual/WindowIntf.cpp















LOCAL_SRC_FILES += ../../../src/core/utils/encoding/gbk2unicode.c
LOCAL_SRC_FILES += ../../../src/core/utils/encoding/jis2unicode.c

LOCAL_SRC_FILES += ../../../src/core/utils/minizip/ioapi.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/minizip/unzip.c
LOCAL_SRC_FILES += ../../../src/core/utils/minizip/zip.c

LOCAL_SRC_FILES += ../../../src/core/utils/win32/ClipboardImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/win32/DebugImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/win32/PadImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/win32/ThreadImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/win32/TimerImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/win32/TVPTimer.cpp

LOCAL_SRC_FILES += ../../../src/core/utils/ClipboardIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/DebugIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/KAGParser.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/MathAlgorithms_Default.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/md5.c
LOCAL_SRC_FILES += ../../../src/core/utils/MiscUtility.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/PadIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/Random.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/RealFFT_Default.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/ThreadIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/TickCount.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/TimerIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/utils/VelocityTracker.cpp





















LOCAL_SRC_FILES += ../../../src/core/tjs2/tjs.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjs.tab.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsArray.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsBinarySerializer.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsByteCodeLoader.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsCompileControl.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsConfig.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsConstArrayData.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsDate.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsdate.tab.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsDateParser.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsDebug.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsDictionary.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsDisassemble.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsError.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsException.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsGlobalStringMap.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsInterCodeExec.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsInterCodeGen.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsInterface.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsLex.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsMath.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsMessage.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsMT19937ar-cok.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsNamespace.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsNative.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsObject.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsObjectExtendable.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsOctPack.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjspp.tab.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsRandomGenerator.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsRegExp.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsScriptBlock.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsScriptCache.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsString.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsUtils.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsVariant.cpp
LOCAL_SRC_FILES += ../../../src/core/tjs2/tjsVariantString.cpp

LOCAL_SRC_FILES += ../../../src/core/sound/CDDAIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/MathAlgorithms.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/MIDIIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/PhaseVocoderFilter.cpp #exclude
LOCAL_SRC_FILES += ../../../src/core/sound/SoundBufferBaseIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/WaveFormatConverter.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/WaveIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/WaveLoopManager.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/WaveSegmentQueue.cpp

LOCAL_SRC_FILES += ../../../src/core/sound/win32/CDDAImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/win32/MIDIImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/win32/SoundBufferBaseImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/win32/tvpsnd.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/win32/WaveImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/sound/win32/WaveMixer.cpp

LOCAL_SRC_FILES += ../../../src/core/msg/MsgIntf.cpp

LOCAL_SRC_FILES += ../../../src/core/msg/win32/MsgImpl.cpp
# LOCAL_SRC_FILES += ../../../src/core/msg/win32/MsgLoad.cpp
LOCAL_SRC_FILES += ../../../src/core/msg/win32/OptionsDesc.cpp
# LOCAL_SRC_FILES += ../../../src/core/msg/win32/ReadOptionDesc.cpp

LOCAL_SRC_FILES += ../../../src/core/movie/krmovie.cpp

LOCAL_SRC_FILES += ../../../src/core/extension/Extension.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/win32/SystemControl.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/android/AndroidUtils.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/cocos2d/AppDelegate.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/cocos2d/CustomFileUtils.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/cocos2d/MainScene.cpp
# LOCAL_SRC_FILES += ../../../src/core/environ/cocos2d/YUVSprite.cpp #excluded

LOCAL_SRC_FILES += ../../../src/core/environ/ConfigManager/GlobalConfigManager.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ConfigManager/IndividualConfigManager.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ConfigManager/LocaleConfigManager.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/linux/Platform.cpp

# LOCAL_SRC_FILES += ../../../src/core/environ/sdl/tvpsdl.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/ui/BaseForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/ConsoleWindow.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/DebugViewLayerForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/FileSelectorForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/GameMainMenu.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/GlobalPreferenceForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/IndividualPreferenceForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/InGameMenuForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/MainFileSelectorForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/MessageBox.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/PreferenceForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/SeletListForm.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/TipsHelpForm.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/ui/extension/ActionExtension.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/ui/extension/UIExtension.cpp

# LOCAL_SRC_FILES += ../../../src/core/environ/win32/xxxxxxxxx.cpp

LOCAL_SRC_FILES += ../../../src/core/environ/Application.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/DetectCPU.cpp
LOCAL_SRC_FILES += ../../../src/core/environ/DumpSend.cpp

LOCAL_SRC_FILES += ../../../src/core/base/BinaryStream.cpp
LOCAL_SRC_FILES += ../../../src/core/base/CharacterSet.cpp
LOCAL_SRC_FILES += ../../../src/core/base/EventIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/PluginIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/ScriptMgnIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/StorageIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/SysInitIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/SystemIntf.cpp
LOCAL_SRC_FILES += ../../../src/core/base/TARArchive.cpp
LOCAL_SRC_FILES += ../../../src/core/base/TextStream.cpp
LOCAL_SRC_FILES += ../../../src/core/base/UtilStreams.cpp
LOCAL_SRC_FILES += ../../../src/core/base/XP3Archive.cpp
LOCAL_SRC_FILES += ../../../src/core/base/ZIPArchive.cpp

LOCAL_SRC_FILES += ../../../src/core/base/win32/EventImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/FileSelector.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/NativeEventQueue.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/PluginImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/ScriptMgnImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/StorageImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/SysInitImpl.cpp
LOCAL_SRC_FILES += ../../../src/core/base/win32/SystemImpl.cpp

LOCAL_SRC_FILES += ../../../src/plugins/ncbind/ncbind.cpp
LOCAL_SRC_FILES += ../../../src/plugins/addFont.cpp
LOCAL_SRC_FILES += ../../../src/plugins/csvParser.cpp
LOCAL_SRC_FILES += ../../../src/plugins/dirlist.cpp
LOCAL_SRC_FILES += ../../../src/plugins/fftgraph.cpp
LOCAL_SRC_FILES += ../../../src/plugins/getabout.cpp
LOCAL_SRC_FILES += ../../../src/plugins/getSample.cpp
LOCAL_SRC_FILES += ../../../src/plugins/InternalPlugins.cpp
LOCAL_SRC_FILES += ../../../src/plugins/LayerExBase.cpp
LOCAL_SRC_FILES += ../../../src/plugins/layerExPerspective.cpp
LOCAL_SRC_FILES += ../../../src/plugins/saveStruct.cpp
LOCAL_SRC_FILES += ../../../src/plugins/varfile.cpp
LOCAL_SRC_FILES += ../../../src/plugins/win32dialog.cpp
LOCAL_SRC_FILES += ../../../src/plugins/wutcwf.cpp
LOCAL_SRC_FILES += ../../../src/plugins/xp3filter.cpp

LOCAL_SRC_FILES += ../../Classes/AppDelegate_ori.cpp
LOCAL_SRC_FILES += ../../Classes/HelloWorldScene.cpp
#see LOCAL_SRC_FILES += ../../../src/core/environ/cocos2d/AppDelegate.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../../../extensions \
                    $(LOCAL_PATH)/../../../.. \
                    $(LOCAL_PATH)/../../../../cocos/editor-support \
					$(LOCAL_PATH)/../../../vendor/libgdiplus/src \
					$(LOCAL_PATH)/../../../vendor/google_breakpad/current/src \
					$(LOCAL_PATH)/../../../vendor/google_breakpad/current/src/common/android/include \
					$(LOCAL_PATH)/../../../src/core/environ \
					$(LOCAL_PATH)/../../../src/core/environ/android \
					$(LOCAL_PATH)/../../../src/core/tjs2 \
					$(LOCAL_PATH)/../../../src/core/base \
					$(LOCAL_PATH)/../../../src/core/visual \
					$(LOCAL_PATH)/../../../src/core/visual/win32 \
					$(LOCAL_PATH)/../../../src/core/sound \
					$(LOCAL_PATH)/../../../src/core/sound/win32 \
					$(LOCAL_PATH)/../../../src/core/utils \
					$(LOCAL_PATH)/../../../src/plugins \
					$(LOCAL_PATH)/../../../src/core/base/win32 \
					$(LOCAL_PATH)/../../../src/core/msg \
					$(LOCAL_PATH)/../../../src/core/msg/win32 \
					$(LOCAL_PATH)/../../../src/core/utils/win32 \
					$(LOCAL_PATH)/../../../src/core/environ/win32 \
					$(LOCAL_PATH)/../../../src/core/extension \
					$(LOCAL_PATH)/../../../src/core \
					$(LOCAL_PATH)/../../../src/core/external/onig \
					$(LOCAL_PATH)/../../../src/core/external/opencv-2.4.13/modules/core/include \
					$(LOCAL_PATH)/../../../src/core/external/opencv-2.4.13/modules/dynamicuda/include \
					$(LOCAL_PATH)/../../../src/core/external/opencv-2.4.13/modules/imgproc/include \
					$(LOCAL_PATH)/../../../src/core/external/opencv-2.4.13 \
					$(LOCAL_PATH)/../../../src/core/external/freetype-2.5.0.1/include \
					
					

					


					
# FIXME: src/core/extension, two extension.h
# TODO: modify main.cpp				

LOCAL_CPPFLAGS := -DTJS_TEXT_OUT_CRLF 
#-DONIG_EXTERN=extern -DNOT_RUBY -DEXPORT
LOCAL_CFLAGS := -DTJS_TEXT_OUT_CRLF 
#-DONIG_EXTERN=extern -DNOT_RUBY -DEXPORT

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
