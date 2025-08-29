#define USE_HELLO 0
#define USE_HELLO_DESIGN_RES 1

#include "AppDelegate.h"
#include "MainScene.h"
#include "ui/MainFileSelectorForm.h"
#include "ui/extension/UIExtension.h"
#include "cocostudio/FlatBuffersSerialize.h"
#include "ConfigManager/LocaleConfigManager.h"
#include "ConfigManager/GlobalConfigManager.h"
#include "Application.h"
#include "Platform.h"
#include "ui/MessageBox.h"
#include "ui/GlobalPreferenceForm.h"
#include "CustomFileUtils.h"

USING_NS_CC;

static Size designResolutionSize(960, 640);
bool TVPCheckStartupArg();
std::string TVPGetCurrentLanguage();

void TVPAppDelegate::applicationWillEnterForeground() {
	::Application->OnActivate();
	Director::getInstance()->startAnimation();
}

void TVPAppDelegate::applicationDidEnterBackground() {
	::Application->OnDeactivate();
	Director::getInstance()->stopAnimation();
}

bool TVPAppDelegate::applicationDidFinishLaunching() {
	cocos2d::log("applicationDidFinishLaunching");
	// initialize director
	FileUtils::setDelegate(TVPCreateCustomFileUtils());
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 001\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 001");
#endif
#endif
	if (!glview) {
		glview = GLViewImpl::create("kirikiri2 frame");
		director->setOpenGLView(glview);
#if CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM
		glview->setFrameSize(960, 640);
#endif
	}
#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 002\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 002");
#endif
#endif

#if USE_HELLO_DESIGN_RES
	//FIXME: why have to use design solution ? would change it back
	glview->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);
#else
	// Set the design resolution
	Size screenSize = glview->getFrameSize();
	Size designSize = designResolutionSize;
	designSize.height = designSize.width * screenSize.height / screenSize.width;
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
#endif

	Size frameSize = glview->getFrameSize();

	std::vector<std::string> searchPath;

	// In this demo, we select resource according to the frame's height.
	// If the resource size is different from design resolution size, you need to set contentScaleFactor.
	// We use the ratio of resource's height to the height of design resolution,
	// this can make sure that the resource's height could fit for the height of design resolution.
	searchPath.push_back("res");

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 003\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 003");
#endif
#endif

	std::string skinpath = GlobalConfigManager::GetInstance()->GetValue<std::string>("skin_path", "");
	if (!skinpath.empty()) {
		if (!FileUtils::getInstance()->isFileExist(skinpath)) {
			GlobalConfigManager::GetInstance()->SetValue("skin_path", "");
		} else {

#if defined(ANDROID) || defined(LINUX)

			throw;
#elif !defined(_MSC_VER)
			TVPAddAutoSearchArchive(skinpath);
#else
			__debugbreak(); throw;
#endif
		}
	}

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004");
#endif
#endif

	// set searching path
	FileUtils::getInstance()->setSearchPaths(searchPath);

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-1\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-1");
#endif
#endif

	// turn on display FPS
	director->setDisplayStats(false);

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-2\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-2");
#endif
#endif

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-3\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-3");
#endif
#endif

	TVPInitUIExtension();

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-4\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-4");
#endif
#endif

	// initialize something
	LocaleConfigManager::GetInstance()->Initialize(TVPGetCurrentLanguage());

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-5\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-5");
#endif
#endif

	// create a scene. it's an autorelease object
	TVPMainScene *scene = TVPMainScene::CreateInstance();

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 004-6\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 004-6");
#endif
#endif

	// run
	director->runWithScene(scene);

	//director->getConsole()->listenOnTCP(16006);

#if defined(ANDROID) || defined(LINUX)
#if defined(LINUX)
	printf("*** *** applicationDidFinishLaunching 005\n");
#else
	__android_log_print(ANDROID_LOG_ERROR, "AppDelegate.cpp", "%s", "*** *** applicationDidFinishLaunching 005");
#endif
#endif

	scene->scheduleOnce([](float dt){
		TVPMainScene::GetInstance()->unschedule("launch");
		TVPGlobalPreferenceForm::Initialize();
		if (!TVPCheckStartupArg())
		{
// 			std::string lastpath;
// 			if (GlobalConfigManager::GetInstance()->GetValueBool("remember_last_path", true)) {
// 				std::string lastpathfile = TVPGetInternalPreferencePath() + "lastpath.txt";
// 				lastpath = FileUtils::getInstance()->getStringFromFile(lastpathfile);
// 				if (!lastpath.empty() && TVPCheckExistentLocalFile(lastpath.c_str())) {
// 					LocaleConfigManager *localeMgr = LocaleConfigManager::GetInstance();
// #if 1
// 					if (TVPShowSimpleMessageBoxYesNo(lastpath.c_str(), localeMgr->GetText("use_last_path")) == 0) {
// 						if (TVPMainScene::GetInstance()->startupFrom(lastpath))
// 							return;
// 					} else {
// 						remove(lastpathfile.c_str());
// // 						FILE* fp = fopen(lastpathfile.c_str(), "wt");
// // 						if (fp) { // clear last path file
// // 							fclose(fp);
// // 						}
// 					}
// #else
// 					TVPMessageBoxForm::showYesNo(localeMgr->GetText("use_last_path"), lastpath,
// 						[lastpath, lastpathfile](int n) {
// 						if (n == 0) {
// 							if (TVPMainScene::GetInstance()->startupFrom(lastpath));
// 						} else {
// 							TVPMainScene::GetInstance()->pushUIForm(TVPMainFileSelectorForm::create(lastpath));
// 							FILE* fp = fopen(lastpathfile.c_str(), "wt");
// 							if (fp) { // clear last path file
// 								fclose(fp);
// 							}
// 						}
// 					});
// 					return;
// #endif
// 				}
// 			}
			TVPMainScene::GetInstance()->pushUIForm(TVPMainFileSelectorForm::create());
		}
	}, 0, "launch");

	return true;
}

void TVPAppDelegate::initGLContextAttrs() {
	GLContextAttrs glContextAttrs = {
		8, 8, 8, 8, 24, 8
	};
	GLView::setGLContextAttrs(glContextAttrs);
}

void TVPAppDelegate::applicationScreenSizeChanged(int newWidth, int newHeight)
{
// 	auto director = Director::getInstance();
// 	director->getOpenGLView()->setFrameSize(newWidth, newHeight);
}

void TVPOpenPatchLibUrl() {
	cocos2d::Application::getInstance()->openURL("https://zeas2.github.io/Kirikiroid2_patch/patch");
}
