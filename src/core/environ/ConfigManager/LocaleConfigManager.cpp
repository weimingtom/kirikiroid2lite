#include "LocaleConfigManager.h"
#include "CCFileUtils.h"
#include "GlobalConfigManager.h"
#include "tinyxml2/tinyxml2.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"

LocaleConfigManager::LocaleConfigManager() {

}

std::string LocaleConfigManager::GetFilePath(int level) {
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::GetFilePath 001");
#endif
	std::string pathprefix = "locale/"; // constant file in app package
	std::string fullpath = pathprefix + currentLangCode + ".xml"; // exp. "local/en_us.xml"
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::GetFilePath 002");
#endif
	if (!cocos2d::FileUtils::getInstance()->isFileExist(fullpath)) {
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::GetFilePath 002-1");
#endif
		if (level < 1) {
			//FIXME:under Android, pthread_mutex_init error
			currentLangCode = "en_us"; // restore to default language config(must exist)
			return GetFilePath(1);
		} else {
			throw;
			return "";
		}
	}
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::GetFilePath 002-2");
#endif
	return cocos2d::FileUtils::getInstance()->fullPathForFilename(fullpath);
}

LocaleConfigManager* LocaleConfigManager::GetInstance() {
	static LocaleConfigManager instance;
	return &instance;
}

const std::string &LocaleConfigManager::GetText(const std::string &tid) {
	auto it = AllConfig.find(tid);
	if (it == AllConfig.end()) {
		AllConfig[tid] = tid;
		return AllConfig[tid];
	}
	return it->second;
}

void LocaleConfigManager::Initialize(const std::string &sysLang) {
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 001");
#endif
	// override by global configured lang
	currentLangCode = GlobalConfigManager::GetInstance()->GetValue<std::string>("user_language", "");
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 002");
#endif
	if (currentLangCode.empty()) currentLangCode = sysLang;
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 003");
#endif
	AllConfig.clear();
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 004");
#endif
	tinyxml2::XMLDocument doc;
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 005");
#endif
	std::string path = GetFilePath(0);
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s, %s", "*** *** LocaleConfigManager::Initialize 005-1", path.c_str());
#endif
	cocos2d::FileUtils *utils = cocos2d::FileUtils::getInstance();
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 005-2");
#endif
	std::string xmlData = utils->getStringFromFile(path);
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 006");
#endif
	bool _writeBOM = false;
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 007");
#endif
	const char* p = xmlData.c_str();
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 008");
#endif
	p = tinyxml2::XMLUtil::ReadBOM(p, &_writeBOM);
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 009");
#endif
	doc.ParseDeep((char*)p, nullptr);
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 010");
#endif
	tinyxml2::XMLElement *rootElement = doc.RootElement();
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize 011");
#endif
	if (rootElement) {
		for (tinyxml2::XMLElement *item = rootElement->FirstChildElement(); item; item = item->NextSiblingElement()) {
			const char *key = item->Attribute("id");
			const char *val = item->Attribute("text");
			if (key && val) {
				AllConfig[key] = val;
			}
		}
	}
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "LocaleConfigManager.cpp", "%s", "*** *** LocaleConfigManager::Initialize end");
#endif
}

bool LocaleConfigManager::initText(cocos2d::ui::Text *ctrl) {
	if (!ctrl) return false;
	return initText(ctrl, ctrl->getString());
}

bool LocaleConfigManager::initText(cocos2d::ui::Button *ctrl)
{
	if (!ctrl) return false;
	return initText(ctrl, ctrl->getTitleText());
}

bool LocaleConfigManager::initText(cocos2d::ui::Text *ctrl, const std::string &tid) {
	if (!ctrl) return false;

	std::string txt = GetText(tid);
	if (txt.empty()) {
		ctrl->setColor(cocos2d::Color3B::RED);
		return false;
	}

	ctrl->setString(txt);
	return true;
}

bool LocaleConfigManager::initText(cocos2d::ui::Button *ctrl, const std::string &tid) {
	if (!ctrl) return false;

	std::string txt = GetText(tid);
	if (txt.empty()) {
		ctrl->setTitleColor(cocos2d::Color3B::RED);
		return false;
	}

	ctrl->setTitleText(txt);
	return true;
}

