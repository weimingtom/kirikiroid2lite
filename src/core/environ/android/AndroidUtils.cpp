#if 0
#include "AndroidUtils.h"
#include "minizip/unzip.h"
#include "zlib.h"
#include <map>
#include <string>
#include <vector>
#include "tjs.h"
#include "MsgIntf.h"
#include "md5.h"
#include "DebugIntf.h"
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "platform/android/jni/JniHelper.h"
#include <set>
#include <sstream>
#include "SysInitIntf.h"
#include "platform/CCFileUtils.h"
#include "ConfigManager/LocaleConfigManager.h"
#include "Platform.h"
#include "platform/CCCommon.h"
#include <EGL/egl.h>
#include <queue>
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include <unistd.h>
#include <fcntl.h>
#include <android/log.h>
#include "TickCount.h"
#include "StorageImpl.h"
#include "ConfigManager/IndividualConfigManager.h"
#include "EventIntf.h"
#include "RenderManager.h"
#include <sys/stat.h>

USING_NS_CC;

#define KR2ActJavaPath "org/tvp/kirikiri2/KR2Activity"
//#define KR2EntryJavaPath "org/tvp/kirikiri2/Kirikiroid2"

extern unsigned int __page_size = getpagesize();

void TVPPrintLog(const char *str) {
	__android_log_print(ANDROID_LOG_INFO, "kr2 debug info", "%s", str);
}

static tjs_uint32 _lastMemoryInfoQuery = 0;
static tjs_int _availMemory, _usedMemory;
static void _updateMemoryInfo() {
	if (TVPGetRoughTickCount32() - _lastMemoryInfoQuery > 3000) { // freq in 3s

		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "updateMemoryInfo", "()V"))
		{
			methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
		}

		if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "getAvailMemory", "()J"))
		{
			_availMemory = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID) / (1024 * 1024);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
		}

		if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "getUsedMemory", "()J"))
		{
			// in kB
			_usedMemory = methodInfo.env->CallStaticLongMethod(methodInfo.classID, methodInfo.methodID) / 1024;
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
		}

		_lastMemoryInfoQuery = TVPGetRoughTickCount32();
	}
}

tjs_int TVPGetSystemFreeMemory()
{
	_updateMemoryInfo();
	return _availMemory;
}

tjs_int TVPGetSelfUsedMemory()
{
	_updateMemoryInfo();
	return _usedMemory;
}

std::string TVPGetDeviceID()
{
    std::string ret;

    // use pure jni to avoid java code
// 	jclass classID = pEnv->FindClass(KR2EntryJavaPath);
// 	std::string strtmp("()L"); strtmp += KR2EntryJavaPath; strtmp += ";";
// 	jmethodID methodGetInstance = pEnv->GetStaticMethodID(classID, "GetInstance", strtmp.c_str());
// 	jobject sInstance = pEnv->CallStaticObjectMethod(classID, methodGetInstance);
// 	jmethodID getSystemService = pEnv->GetMethodID(classID, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
// 	jstring jstrPhone = pEnv->NewStringUTF("phone");
// 	jobject telephonyManager = pEnv->CallObjectMethod(sInstance, getSystemService, jstrPhone);
// 	pEnv->DeleteLocalRef(jstrPhone);
// 
// 	jclass clsTelephonyManager = pEnv->FindClass("android/telephony/TelephonyManager");
// 	jmethodID getDeviceId = pEnv->GetMethodID(clsTelephonyManager, "getDeviceId", "()Ljava/lang/String;");
// 	jstring jstrDevID = (jstring)pEnv->CallObjectMethod(telephonyManager, getDeviceId);
// 	if (jstrDevID) {
// 		const char *p = pEnv->GetStringUTFChars(jstrDevID, 0);
// 		if (p && *p) {
// 			ret = "DevID=";
// 			ret += p;
// 			pEnv->ReleaseStringUTFChars(jstrDevID, p);
// 		} else {
// 			if (p) {
// 				pEnv->ReleaseStringUTFChars(jstrDevID, p);
// 			}
// 			jmethodID getContentResolver = pEnv->GetMethodID(classID, "getContentResolver", "()Landroid/content/ContentResolver;");
// 			jobject contentResolver = pEnv->CallObjectMethod(sInstance, getContentResolver);
// 
// 			jclass clsSecure = pEnv->FindClass("android/provider/Settings/Secure");
// 			if (clsSecure) {
// 				jmethodID Secure_getString = pEnv->GetMethodID(clsSecure, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
// 				jstring jastrAndroid_ID = pEnv->NewStringUTF("android_id");
// 				jstring jstrAndroidID = (jstring)pEnv->CallStaticObjectMethod(clsSecure, Secure_getString, contentResolver, jastrAndroid_ID);
// 				if (jstrAndroidID) {
// 					const char *p = pEnv->GetStringUTFChars(jstrAndroidID, 0);
// 					if (p && strlen(p) > 8 && strcmp(p, "9774d56d682e549c")) {
// 						ret = "AndroidID=";
// 						ret += p;
// 					}
// 				}
// 				pEnv->ReleaseStringUTFChars(jstrAndroidID, p);
// 				pEnv->DeleteLocalRef(jastrAndroid_ID);
// 			}
// 		}
// 	}
// 	if (ret.empty())
	{
		JniMethodInfo methodInfo;
		if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "getDeviceId", "()Ljava/lang/String;"))
		{
			jstring result = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
			ret = JniHelper::jstring2string(result);
			methodInfo.env->DeleteLocalRef(result);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
			char *t = (char*)ret.c_str();
			while (*t) {
				if (*t == ':') {
					*t = '=';
					break;
				}
				t++;
			}
		}
	}

    return ret;
}

static jobject GetKR2ActInstance() {
	JniMethodInfo methodInfo;
	std::string strtmp("()L"); strtmp += KR2ActJavaPath; strtmp += ";";
	if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "GetInstance", strtmp.c_str())) {
		jobject ret = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return ret;
	}
	return 0;
}

static std::string GetApkStoragePath() {
	JniMethodInfo methodInfo;
	jobject sInstance = GetKR2ActInstance();
	if (!JniHelper::getMethodInfo(methodInfo, "android/content/Context", "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;")) {
		methodInfo.env->DeleteLocalRef(sInstance);
		return "";
	}
	jobject ApplicationInfo = methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID);
	jclass clsApplicationInfo = methodInfo.env->FindClass("android/content/pm/ApplicationInfo");
	jfieldID id_sourceDir = methodInfo.env->GetFieldID(clsApplicationInfo, "sourceDir", "Ljava/lang/String;");
	methodInfo.env->DeleteLocalRef(sInstance);
	return JniHelper::jstring2string((jstring)methodInfo.env->GetObjectField(ApplicationInfo, id_sourceDir));
}

static std::string GetPackageName() {
	JniMethodInfo methodInfo;
	jobject sInstance = GetKR2ActInstance();
	if (!JniHelper::getMethodInfo(methodInfo, "android/content/ContextWrapper", "getPackageName", "()Ljava/lang/String;")) {
		methodInfo.env->DeleteLocalRef(sInstance);
		return "";
	}
	return JniHelper::jstring2string((jstring)methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID));
}

// from unzip.cpp
#define FLAG_UTF8 (1<<11)
extern zlib_filefunc64_def TVPZlibFileFunc;
class ZipFile
{
	unzFile uf;
	bool utf8;

public:
	ZipFile() : uf(0) {
	}
	~ZipFile() {
		if (uf) {
			unzClose(uf);
			uf = NULL;
		}
	}
    bool Open(const char *filename) {
        if ((uf = unzOpen(filename)) == NULL) {
            ttstr msg = filename;
            msg += TJS_W(" can't open.");
            TVPThrowExceptionMessage(msg.c_str());
            return false;
        }
        // UTF8¤Ê¥Õ¥¡¥¤¥ëÃû¤«¤É¤¦¤«¤ÎÅÐ¶¨¡£×î³õ¤Î¥Õ¥¡¥¤¥ë¤Ç›Q¤á¤ë
        unzGoToFirstFile(uf);
        unz_file_info file_info;
        if (unzGetCurrentFileInfo(uf, &file_info, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
            utf8 = (file_info.flag & FLAG_UTF8) != 0;
            return true;
        }
        return false;
    }
	bool GetData(std::vector<unsigned char> &buff, const char *filename) {
		bool ret = false;
		if (unzLocateFile(uf, filename, 1) == UNZ_OK) {
			int result = unzOpenCurrentFile(uf);
			if (result == UNZ_OK) {
				unz_file_info info;
				unzGetCurrentFileInfo(uf, &info, NULL, 0, NULL, 0, NULL, 0);
				buff.resize(info.uncompressed_size);
				unsigned int size = unzReadCurrentFile(uf, &buff[0], info.uncompressed_size);
				if (size == info.uncompressed_size) ret = true;
				unzCloseCurrentFile(uf);
			}
		}
		return ret;
	}
    tjs_int64 GetMD5InZip(const char *filename) {
        std::vector<unsigned char> buff;
        if(!GetData(buff, filename)) return 0;
        md5_state_t state;
        md5_init(&state);
        md5_append(&state, (const md5_byte_t*)&buff[0], buff.size());
        union {
            tjs_int64 _s64[2];
            tjs_uint8 _u8[16];
        } digest;
        md5_finish(&state, digest._u8);
        return digest._s64[0] ^ digest._s64[1];
    }
private:
	unzFile zipFile;
};

std::string TVPGetDeviceLanguage() {
	// use pure jni to avoid java code
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, "java/util/Locale", "getDefault", "()Ljava/util/Locale;"))
		return "";
	jobject LocaleObj = methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
	if (!JniHelper::getMethodInfo(methodInfo, "java/util/Locale", "getLanguage", "()Ljava/lang/String;"))
		return "";
	jstring languageID = (jstring)methodInfo.env->CallObjectMethod(LocaleObj, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
	return JniHelper::jstring2string(languageID);
}

std::string TVPGetPackageVersionString() {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, KR2ActJavaPath, "GetVersion", "()Ljava/lang/String;")) {
		return JniHelper::jstring2string((jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID));
	}
	return "";
}

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

static std::string File_getAbsolutePath(jobject FileObj) {
	if (!FileObj) return "";
	JniMethodInfo methodInfo;
	if (!JniHelper::getMethodInfo(methodInfo, "java/io/File", "exists", "()Z")) return "";
	if (!methodInfo.env->CallBooleanMethod(FileObj, methodInfo.methodID)) return "";
	if (!JniHelper::getMethodInfo(methodInfo, "java/io/File", "getAbsolutePath", "()Ljava/lang/String;")) return "";
	jstring path = (jstring)methodInfo.env->CallObjectMethod(FileObj, methodInfo.methodID);
	std::string ret = cocos2d::JniHelper::jstring2string(path);
	return ret;
}

static std::string GetInternalStoragePath() {
	jobject sInstance = GetKR2ActInstance();
	JniMethodInfo methodInfo;
	if (!JniHelper::getMethodInfo(methodInfo, "android/content/ContextWrapper", "getFilesDir", "()Ljava/io/File;")) {
		return "";
	}
	jobject FileObj = methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID);
	return File_getAbsolutePath(FileObj);
}

std::string Android_GetDumpStoragePath() {
	return GetInternalStoragePath() + "/dump";
}

static int InsertFilepathInto(JNIEnv *env, std::vector<std::string>& vec, jobjectArray FileObjs) {
	int count = env->GetArrayLength(FileObjs);
	for (int i = 0; i < count; ++i) {
		jobject FileObj = env->GetObjectArrayElement(FileObjs, i);
		std::string path = File_getAbsolutePath(FileObj);
		if (!path.empty()) vec.push_back(path);
	}
	return count;
}

static int GetExternalStoragePath(std::vector<std::string> &ret) {
	int count = 0;
	JniMethodInfo methodInfo;
	jobject sInstance = GetKR2ActInstance();
// 	if (JniHelper::getMethodInfo(methodInfo, "android/content/Context", "getExternalMediaDirs", "()[Ljava/io/File;")) {
// 		jobjectArray FileObjs = (jobjectArray)methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID);
// 		if(FileObjs) count += InsertFilepathInto(methodInfo.env, ret, FileObjs);
// 	}
	if (JniHelper::getMethodInfo(methodInfo, "android/content/Context", "getExternalFilesDirs", "(Ljava/lang/String;)[Ljava/io/File;")) {
		jobjectArray FileObjs = (jobjectArray)methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID, nullptr);
		if (FileObjs) count += InsertFilepathInto(methodInfo.env, ret, FileObjs);
	} else if (JniHelper::getMethodInfo(methodInfo, "android/content/Context", "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;")) {
		jobject FileObj = methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID, nullptr);
		if (FileObj) {
			ret.push_back(File_getAbsolutePath(FileObj));
			++count;
		}
	}
	return count;
}

std::vector<std::string> TVPGetAppStoragePath() {
	std::vector<std::string> ret;
	ret.push_back(GetInternalStoragePath());
	GetExternalStoragePath(ret);
	return ret;
}

std::vector<std::string> TVPGetDriverPath() {
	std::vector<std::string> ret;
	jobject sInstance = GetKR2ActInstance();
	JniMethodInfo methodInfo;
	if (JniHelper::getMethodInfo(methodInfo, KR2ActJavaPath, "getStoragePath", "()[Ljava/lang/String;")) {
		jobjectArray PathObjs = (jobjectArray)methodInfo.env->CallObjectMethod(sInstance, methodInfo.methodID);
		if (PathObjs) {
			int count = methodInfo.env->GetArrayLength(PathObjs);
			for (int i = 0; i < count; ++i) {
				jstring path = (jstring)methodInfo.env->GetObjectArrayElement(PathObjs, i);
				if (path) ret.push_back(cocos2d::JniHelper::jstring2string(path));
			}
		}
	}
	
	if (!ret.empty()) return ret;

	char buffer[256] = { 0 };

	// enum all mounted storages
	FILE *fp = fopen("/proc/mounts", "r");
	std::set<std::string> mounted;
	while (fgets(buffer, sizeof(buffer), fp)) {
		std::vector<std::string> tabs;
		split(buffer, ' ', tabs);
		if (tabs.size() < 3) continue;
		if (mounted.find(tabs[0]) != mounted.end()) continue;
		const std::string &path = tabs[1];
		if (tabs[3].find("rw,") != std::string::npos && (tabs[2] == "vfat" || path.find("/mnt") != std::string::npos)) {
			if (path.find("/mnt/secure") != std::string::npos ||
				path.find("/mnt/asec") != std::string::npos ||
				path.find("/mnt/mapper") != std::string::npos ||
				path.find("/mnt/obb") != std::string::npos ||
				tabs[0] == "tmpfs" || tabs[2] == "tmpfs") {
				continue;
			}
			mounted.insert(tabs[0]);
			ret.push_back(path);
		}
	}

	return ret;
}

// extern "C" int TVPCheckValidate()
// {
//     JNIEnv *pEnv = 0;
//     bool ret = true;
// 
//     if (! getEnv(&pEnv))
//     {
//         return false;
//     }
// 	{
// 		jclass classID = pEnv->FindClass(KR2EntryJavaPath);
// 		std::string strtmp("()L"); strtmp += KR2EntryJavaPath; strtmp += ";";
// 		jmethodID methodGetInstance = pEnv->GetStaticMethodID(classID, "GetInstance", strtmp.c_str());
// 		jobject sInstance = pEnv->CallStaticObjectMethod(classID, methodGetInstance);
// 		
// 		jclass clsPreferenceManager = pEnv->FindClass("android.preference.PreferenceManager");
// 		jmethodID getDefaultSharedPreferences = pEnv->GetMethodID(clsPreferenceManager, "getDefaultSharedPreferences", "(Landroid/content/Context;)Landroid.preference.PreferenceManager;");
// 		jobject PreferenceManager = pEnv->CallStaticObjectMethod(clsPreferenceManager, getDefaultSharedPreferences, sInstance);
// 		jmethodID getString = pEnv->GetMethodID(clsPreferenceManager, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
// 		jstring jstrConstAPPID = pEnv->NewStringUTF("APP_ID");
// 		jstring jstrNull = pEnv->NewStringUTF("");
// 		jstring jstrAPPID = (jstring)pEnv->CallObjectMethod(PreferenceManager, getString, jstrConstAPPID, jstrNull);
// 		pEnv->DeleteLocalRef(jstrConstAPPID);
// 		pEnv->DeleteLocalRef(jstrNull);
// 		const char *p = pEnv->GetStringUTFChars(jstrAPPID, 0);
// 		if(0x929e08af != adler32(0, (const Bytef*)p, strlen(p))) ret = false;
// 		pEnv->ReleaseStringUTFChars(jstrAPPID, p);
// 	}
// 
//     return ret;
// }
namespace kr2android {
	boost::condition_variable MessageBoxCond;
	boost::mutex MessageBoxLock;
	int MsgBoxRet = 0;
	std::string MessageBoxRetText;
}
using namespace kr2android;

int TVPShowSimpleMessageBox(const char *pszText, const char *pszTitle, unsigned int nButton, const char **btnText) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "ShowMessageBox", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V"))
	{
		jstring jstrTitle = methodInfo.env->NewStringUTF(pszTitle);
		jstring jstrText = methodInfo.env->NewStringUTF(pszText);
		jclass strcls = methodInfo.env->FindClass("java/lang/String");
		jobjectArray btns = methodInfo.env->NewObjectArray(nButton, strcls, nullptr);
		for (unsigned int i = 0; i < nButton; ++i) {
			jstring jstrBtn = methodInfo.env->NewStringUTF(btnText[i]);
			methodInfo.env->SetObjectArrayElement(btns, i, jstrBtn);
			methodInfo.env->DeleteLocalRef(jstrBtn);
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrTitle, jstrText, btns);

		methodInfo.env->DeleteLocalRef(jstrTitle);
		methodInfo.env->DeleteLocalRef(jstrText);
		methodInfo.env->DeleteLocalRef(btns);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		boost::unique_lock<boost::mutex> lk(MessageBoxLock);
		MessageBoxCond.wait(lk);
		return MsgBoxRet;
	}
	return -1;
}

int TVPShowSimpleMessageBox(const ttstr & text, const ttstr & caption, const std::vector<ttstr> &vecButtons) {
	tTJSNarrowStringHolder pszText(text.c_str());
	tTJSNarrowStringHolder pszTitle(caption.c_str());
	std::vector<const char *> btnText; btnText.reserve(vecButtons.size());
	std::vector<std::string> btnTextHold; btnTextHold.reserve(vecButtons.size());
	for (const ttstr &btn : vecButtons) {
		btnTextHold.push_back(btn.AsStdString());
		btnText.push_back(btnTextHold.back().c_str());
	}
	return TVPShowSimpleMessageBox(pszText, pszTitle, btnText.size(), &btnText[0]);
}

int TVPShowSimpleInputBox(ttstr &text, const ttstr &caption, const ttstr &prompt, const std::vector<ttstr> &vecButtons) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "ShowInputBox", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V"))
	{
		jstring jstrTitle = methodInfo.env->NewStringUTF(caption.AsStdString().c_str());
		jstring jstrText = methodInfo.env->NewStringUTF(text.AsStdString().c_str());
		jstring jstrPrompt = methodInfo.env->NewStringUTF(prompt.AsStdString().c_str());
		jclass strcls = methodInfo.env->FindClass("java/lang/String");
		jobjectArray btns = methodInfo.env->NewObjectArray(vecButtons.size(), strcls, nullptr);
		for (unsigned int i = 0; i < vecButtons.size(); ++i) {
			jstring jstrBtn = methodInfo.env->NewStringUTF(vecButtons[i].AsStdString().c_str());
			methodInfo.env->SetObjectArrayElement(btns, i, jstrBtn);
			methodInfo.env->DeleteLocalRef(jstrBtn);
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrTitle, jstrPrompt, jstrText, btns);

		methodInfo.env->DeleteLocalRef(jstrTitle);
		methodInfo.env->DeleteLocalRef(jstrText);
		methodInfo.env->DeleteLocalRef(jstrPrompt);
		methodInfo.env->DeleteLocalRef(btns);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		boost::unique_lock<boost::mutex> lk(MessageBoxLock);
		MessageBoxCond.wait(lk);
		text = MessageBoxRetText;
		return MsgBoxRet;
	}
	return -1;
}

extern std::string Android_ShowInputDialog(const char* pszTitle, const char *pszInitText);
extern std::string Android_ShowFileBrowser(const char* pszTitle, const char *pszInitDir, bool showEditor);
extern ttstr TVPGetAppPath();
extern ttstr TVPGetLocallyAccessibleName(const ttstr &name);

std::vector<ttstr> Android_GetExternalStoragePath() {
	static std::vector<ttstr> ret;
	if (ret.empty()) {
		std::vector<std::string> pathlist;
		GetExternalStoragePath(pathlist);
		for (const std::string &path : pathlist) {
			std::string strPath = "file://.";
			strPath += path;
			ret.push_back(strPath);
		}
	}
	return ret;
}

ttstr Android_GetInternalStoragePath() {
	static ttstr strPath;
	if (strPath.IsEmpty()) {
		strPath = "file://.";
		strPath += GetInternalStoragePath();
	}
	return strPath;
}

ttstr Android_GetApkStoragePath() {
	static ttstr strPath;
	if (strPath.IsEmpty()) {
		strPath = "file://.";
		strPath += GetApkStoragePath();
	}
	return strPath;
}

struct _eventQueueNode {
	std::function<void()> func;
	_eventQueueNode *prev;
	_eventQueueNode *next;
};

static std::atomic<_eventQueueNode*> _lastQueuedEvents(nullptr);
static void _processEvents(float) {
	_eventQueueNode* q = _lastQueuedEvents.exchange(nullptr);
	if (q) {
		q->next = nullptr;
		while (q->prev) {
			q->prev->next = q;
			q = q->prev;
		}
	}
	while (q) {
		q->func();
		_eventQueueNode *nq = q->next;
		delete q;
		q = nq;
	}
}

void Android_PushEvents(const std::function<void()> &func) {
	_eventQueueNode *node = new _eventQueueNode;
	node->func = func;
	node->next = nullptr;
	node->prev = nullptr;
	while (!_lastQueuedEvents.compare_exchange_weak(node->prev, node)) {}
}

void TVPCheckAndSendDumps(const std::string &dumpdir, const std::string &packageName, const std::string &versionStr);
bool TVPCheckStartupArg() {
	// check dump
	TVPCheckAndSendDumps(Android_GetDumpStoragePath(), GetPackageName(), TVPGetPackageVersionString());

	// register event dispatcher
	cocos2d::Director *director = cocos2d::Director::getInstance();
	class HackForScheduler : public cocos2d::Scheduler {
	public:
		void regProcessEvents() {
			schedulePerFrame(_processEvents, &_lastQueuedEvents, -1, false);
		}
	};
	static_cast<HackForScheduler*>(director->getScheduler())->regProcessEvents();

	return false;
}

void TVPControlAdDialog(int adType, int arg1, int arg2) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "MessageController", "(III)V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, adType, arg1, arg2);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void TVPForceSwapBuffer() {
	eglSwapBuffers(eglGetCurrentDisplay(), eglGetCurrentSurface(EGL_DRAW));
}

static int _GetAndroidSDKVersion() {
	JNIEnv *pEnv = JniHelper::getEnv();
	jclass classID = pEnv->FindClass("android/os/Build$VERSION");
	jfieldID idSDK_INT = pEnv->GetStaticFieldID(classID, "SDK_INT", "I");
	return pEnv->GetStaticIntField(classID, idSDK_INT);
}
static int GetAndroidSDKVersion() {
	static int result = _GetAndroidSDKVersion();
	return result;
}

static bool IsLollipop() {
	return GetAndroidSDKVersion() >= 21;
}

static bool IsOreo() {
	return GetAndroidSDKVersion() >= 26;
}

void TVPFetchSDCardPermission() {
	if (!IsLollipop())
		return;
	std::vector<std::string> paths;
	GetExternalStoragePath(paths);
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "requireLEXA", "(Ljava/lang/String;)V")) {
		jstring jstrPath = methodInfo.env->NewStringUTF(paths.back().c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstrPath);
		methodInfo.env->DeleteLocalRef(jstrPath);
		return;
	}
	return;
}

bool TVPCheckStartupPath(const std::string &path) {
	// check writing permission first
	int pos = path.find_last_of('/');
	if (pos == path.npos) return false;
	std::string parent = path.substr(0, pos);
	JniMethodInfo methodInfo;
	bool success = false;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "isWritableNormalOrSaf", "(Ljava/lang/String;)Z")) {
		jstring jstrPath = methodInfo.env->NewStringUTF(parent.c_str());
		success = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstrPath);
		methodInfo.env->DeleteLocalRef(jstrPath);
		if (success) {
			parent += "/savedata";
			if (!TVPCheckExistentLocalFolder(parent)) {
				TVPCreateFolders(parent);
			}
			jstrPath = methodInfo.env->NewStringUTF(parent.c_str());
			success = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstrPath);
			methodInfo.env->DeleteLocalRef(jstrPath);
		}
	}

	if (!success) {
		std::vector<std::string> paths;
		paths.push_back(GetInternalStoragePath());
		GetExternalStoragePath(paths);
		std::string msg = LocaleConfigManager::GetInstance()->GetText("use_internal_path");
		if (paths.size() > 0) {
			size_t pos = msg.find("%1");
			if (pos != msg.npos) {
				msg = msg.replace(msg.begin() + pos, msg.begin() + pos + 2, paths.back());
			}
		}
		std::vector<ttstr> btns;
		btns.push_back(LocaleConfigManager::GetInstance()->GetText("continue_run"));
		bool isLOLLIPOP = IsLollipop();
		if (isLOLLIPOP)
			btns.push_back(LocaleConfigManager::GetInstance()->GetText("get_sdcard_permission"));
		else
			btns.push_back(LocaleConfigManager::GetInstance()->GetText("cancel"));
		int result = TVPShowSimpleMessageBox(msg, LocaleConfigManager::GetInstance()->GetText("readonly_storage"), btns);
		if (isLOLLIPOP && result == 1) {
			TVPFetchSDCardPermission();
		}
		if (result != 0)
			return false;
	}

	// check adreno GPU issue
// 	if (IndividualConfigManager::GetInstance()->GetValue<std::string>("renderer", "software") == "opengl") {
// 		TVPOnOpenGLRendererSelected(false);
// 	}
	return true;
}

bool TVPCreateFolders(const ttstr &folder)
{
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "CreateFolders", "(Ljava/lang/String;)Z")) {
		jstring jstr = methodInfo.env->NewStringUTF(folder.AsStdString().c_str());
		bool ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstr);
		methodInfo.env->DeleteLocalRef(jstr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return ret;
	}
	return false;
}

static bool TVPWriteDataToFileJava(const std::string &filename, const void* data, unsigned int size) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "WriteFile", "(Ljava/lang/String;[B)Z")) {
		cocos2d::FileUtils *fileutil = cocos2d::FileUtils::getInstance();
		bool ret = false;
		int retry = 3;
		do {
			jstring jstr = methodInfo.env->NewStringUTF(filename.c_str());
			jbyteArray arr = methodInfo.env->NewByteArray(size);
			methodInfo.env->SetByteArrayRegion(arr, 0, size, (jbyte*)data);
			ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstr, arr);
			methodInfo.env->DeleteLocalRef(arr);
			methodInfo.env->DeleteLocalRef(jstr);
			methodInfo.env->DeleteLocalRef(methodInfo.classID);
		} while (!fileutil->isFileExist(filename) && --retry);
		return ret;
	}
	return false;
}

bool TVPWriteDataToFile(const ttstr &filepath, const void *data, unsigned int size) {
	std::string filename = filepath.AsStdString();
	cocos2d::FileUtils *fileutil = cocos2d::FileUtils::getInstance();
	while (fileutil->isFileExist(filename)) {
		// for number filename suffix issue
		time_t t = time(nullptr);
		std::vector<char> buffer;
		buffer.resize(filename.size() + 32);
		sprintf(&buffer.front(), "%s.%d.bak", filename.c_str(), (int)t);
		std::string tempname = &buffer.front();
		if (rename(filename.c_str(), tempname.c_str()) == 0) {
			// file api is OK
			FILE *fp = fopen(filename.c_str(), "wb");
			if (fp) {
				bool ret = fwrite(data, 1, size, fp) == size;
				fclose(fp);
				remove(tempname.c_str());
				return ret;
			}
		}
		bool ret = TVPWriteDataToFileJava(filename, data, size);
		if (fileutil->isFileExist(tempname.c_str())) {
			TVPDeleteFile(tempname);
		}
		return ret;
	}
	FILE *fp = fopen(filename.c_str(), "wb");
	if (fp) {
		// file api is OK
		int writed = fwrite(data, 1, size, fp);
		fclose(fp);
		return writed == size;
	}
	return TVPWriteDataToFileJava(filename, data, size);
}

std::string TVPGetCurrentLanguage() {
	JniMethodInfo t;
	std::string ret("");
	
	if (JniHelper::getStaticMethodInfo(t, "org/tvp/kirikiri2/KR2Activity", "getLocaleName", "()Ljava/lang/String;")) {
		jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		ret = JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}

	return ret;
}

void TVPExitApplication(int code) {
	TVPDeliverCompactEvent(TVP_COMPACT_LEVEL_MAX);
	if (!TVPIsSoftwareRenderManager())
		iTVPTexture2D::RecycleProcess();
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "org/tvp/kirikiri2/KR2Activity", "exit", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	exit(code);
}

void TVPHideIME() {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "hideTextInput", "()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	}
}

void TVPShowIME(int x, int y, int w, int h) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "showTextInput", "(IIII)V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, x, y, w, h);
	}
}

void TVPProcessInputEvents() {}

bool TVPDeleteFile(const std::string &filename) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "DeleteFile", "(Ljava/lang/String;)Z")) {
		jstring jstr = methodInfo.env->NewStringUTF(filename.c_str());
		bool ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstr);
		methodInfo.env->DeleteLocalRef(jstr);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return ret;
	}
	return false;
}

bool TVPRenameFile(const std::string &from, const std::string &to) {
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "org/tvp/kirikiri2/KR2Activity", "RenameFile", "(Ljava/lang/String;Ljava/lang/String;)Z")) {
		jstring jstr = methodInfo.env->NewStringUTF(from.c_str());
		jstring jstr2 = methodInfo.env->NewStringUTF(to.c_str());
		bool ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID, jstr, jstr2);
		methodInfo.env->DeleteLocalRef(jstr);
		methodInfo.env->DeleteLocalRef(jstr2);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return ret;
	}
	return false;
}

tjs_uint32 TVPGetRoughTickCount32()
{
	tjs_uint32 uptime = 0;
	struct timespec on;
	if (clock_gettime(CLOCK_MONOTONIC, &on) == 0)
		uptime = on.tv_sec * 1000 + on.tv_nsec / 1000000;
	return uptime;
}

bool TVP_stat(const tjs_char *name, tTVP_stat &s) {
	tTJSNarrowStringHolder holder(name);
	return TVP_stat(holder, s);
}

#undef st_atime
#undef st_ctime
#undef st_mtime
//int stat64(const char* __path, struct stat64* __buf) __INTRODUCED_IN(21); // force link it !
bool TVP_stat(const char *name, tTVP_stat &s) {
	struct stat t;
	// static_assert(sizeof(t.st_size) == 4, "");
	static_assert(sizeof(t.st_size) == 8, "");
	bool ret = !stat(name, &t);
	s.st_mode = t.st_mode;
	s.st_size = t.st_size;
	s.st_atime = t.st_atim.tv_sec;
	s.st_mtime = t.st_mtim.tv_sec;
	s.st_ctime = t.st_ctim.tv_sec;
	return ret;
}

void TVPSendToOtherApp(const std::string &filename) {

}
#elif defined(_MSC_VER)

#include "cocos2d.h"

#ifndef _MSC_VER
#include "AndroidUtils.h"
#else
#include <Windows.h>
#include "../../../../project/win32/resource.h"
#include <string>
std::string TVPGetDeviceID();
#endif
#include "minizip/unzip.h"
#include "zlib.h"
#include <map>
#include <string>
#include <vector>
#include "tjs.h"
//#include "MsgIntf.h"
//#include "md5.h"
//#include "DebugIntf.h"
#ifndef _MSC_VER
#include "platform/android/jni/JniHelper.h"
#endif
#include <set>
#include <sstream>
#include "SysInitIntf.h"
//#include "platform/CCFileUtils.h"
#include "ConfigManager/LocaleConfigManager.h"
//#include "Platform.h"
//#include "platform/CCCommon.h"
#ifndef _MSC_VER
#include <EGL/egl.h>
#endif
//#include <queue>
//#include "base/CCDirector.h"
//#include "base/CCScheduler.h"
#ifndef _MSC_VER
#include <unistd.h>
#else
#include <windows.h>
#endif
//#include <fcntl.h>
#ifndef _MSC_VER
#include <android/log.h>
#endif
//#include "TickCount.h"
//#include "StorageImpl.h"
//#include "ConfigManager/IndividualConfigManager.h"
#include "EventIntf.h"
#include "RenderManager.h"
#include <sys/stat.h>
#include "Platform.h"
#include <locale>         // std::wstring_convert
#include <string>
#include <codecvt>
#include <wchar.h>

//https://www.cnblogs.com/findumars/p/6206177.html
const std::wstring UTF82WCS(const char* szU8)
{
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	std::wstring strW(wszString);
	delete[] wszString;
	return strW;
}

const std::string WCS2UTF8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	std::string strM(szUtf8);
	free(szUtf8);
	return strM;
}

int TVPShowSimpleMessageBox(const ttstr & text, const ttstr & caption, const std::vector<ttstr> &vecButtons)
{
#if 0
	__debugbreak(); throw;
	return 0;
#else
	::MessageBoxW(NULL, UTF82WCS(text.AsStdString().c_str()).c_str(), UTF82WCS(caption.AsStdString().c_str()).c_str(), MB_OK | MB_ICONINFORMATION);
	return 0;
#endif
}

std::string TVPGetPackageVersionString() {
#if 0
	__debugbreak(); throw;
	return "";
#else
	return "ver 1.0.0";
#endif
}
static std::wstring strInputBoxTitle;
static std::wstring strInputBoxPrompt;
static std::wstring strInputBoxText;
BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
			SetWindowText(hwnd, strInputBoxTitle.c_str());
			SetDlgItemText(hwnd, IDC_STATIC1, strInputBoxPrompt.c_str());
			SetDlgItemText(hwnd, IDC_EDIT1, strInputBoxText.c_str());

			RECT rc, rc1, rctomove;
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			rc.left = 0;
			rc.top = 0;
			rc.right = width;
			rc.bottom = height;
			GetClientRect(hwnd, &rc1);

			rctomove.left = (rc.right - rc.left) / 2 - (rc1.right - rc1.left) / 2;
			rctomove.right = (rc.right - rc.left) / 2 + (rc1.right - rc1.left) / 2;
			rctomove.top = (rc.bottom - rc.top) / 2 - (rc1.bottom - rc1.top) / 2;
			rctomove.bottom = (rc.bottom - rc.top) / 2 + (rc1.bottom - rc1.top) / 2;
			::SetWindowPos(hwnd, HWND_TOPMOST, rctomove.left, rctomove.top, rc1.right - rc1.left, rc1.bottom - rc1.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: {
				 wchar_t szcaption[1024] = { 0 };
				 GetDlgItemText(hwnd, IDC_EDIT1, szcaption, sizeof(szcaption));
				 strInputBoxText = szcaption;
				 EndDialog(hwnd, 0);	 
			}
			return FALSE;

		case IDCANCEL:
			EndDialog(hwnd, 0);
			return FALSE;
		}
		return FALSE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return FALSE;

	default:
		return FALSE;
	}
}

//#define IDD_DIALOG1 101

int TVPShowSimpleInputBox(ttstr &text, const ttstr &caption, const ttstr &prompt, const std::vector<ttstr> &vecButtons) {
#if 0
	__debugbreak(); throw;
	return 0;
#else
	strInputBoxText = UTF82WCS(text.AsStdString().c_str()).c_str();
	strInputBoxTitle = UTF82WCS(caption.AsStdString().c_str()).c_str();
	strInputBoxPrompt = UTF82WCS(prompt.AsStdString().c_str()).c_str();
	::DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	text = WCS2UTF8(strInputBoxText.c_str());
	return 0;
#endif
}


bool TVPCheckExistentLocalFolder(const ttstr &name);
bool TVPCreateFolders(const ttstr &folder);
static bool _TVPCreateFolders(const ttstr &folder)
{
	// create directories along with "folder"
	if (folder.IsEmpty()) return true;

	if (TVPCheckExistentLocalFolder(folder))
		return true; // already created

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if (p[i] == TJS_W(':')) return true;

	while (i >= 0 && (p[i] == TJS_W('/') || p[i] == TJS_W('\\'))) i--;

	if (i >= 0 && p[i] == TJS_W(':')) return true;

	for (; i >= 0; i--)
	{
		if (p[i] == TJS_W(':') || p[i] == TJS_W('/') ||
			p[i] == TJS_W('\\'))
			break;
	}

	ttstr parent(p, i + 1);
	if (!TVPCreateFolders(parent)) return false;

	return !_wmkdir(folder.c_str());

}
bool TVPCreateFolders(const ttstr &folder)
{
#if 0
	__debugbreak(); throw;
	return false;
#else
	if (folder.IsEmpty()) return true;

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if (p[i] == TJS_W(':')) return true;

	if (p[i] == TJS_W('/') || p[i] == TJS_W('\\')) i--;

	return _TVPCreateFolders(ttstr(p, i + 1));
#endif
}

//see environ/win32/Platform.cpp
tjs_uint32 TVPGetRoughTickCount32()
{
	return timeGetTime();
}

bool TVP_stat(const tjs_char *name, tTVP_stat &s) {
	struct _stat64 t;
	bool ret = !_wstat64(name, &t);
	s.st_mode = t.st_mode;
	s.st_size = t.st_size;
	s.st_atime = t.st_atime;
	s.st_mtime = t.st_mtime;
	s.st_ctime = t.st_ctime;
	return ret;
}

bool TVP_stat(const char *name, tTVP_stat &s) {
	ttstr filename(name);
	return TVP_stat(filename.c_str(), s);
}

bool TVPWriteDataToFile(const ttstr &filepath, const void *data, unsigned int size) {
#if 0
	__debugbreak(); throw;
	return false;
#else
	OutputDebugString(L"====================>TVPWriteDataToFile: ");
	OutputDebugString(filepath.c_str());
	OutputDebugString(L"\n");
	FILE* handle = _wfopen(filepath.c_str(), L"wb");
	if (handle) {
		bool ret = fwrite(data, 1, size, handle) == size;
		fclose(handle);
		return ret;
	}
	return false;
#endif
}

void TVPExitApplication(int code) {
#if 0
	__debugbreak(); throw;
#else
	TVPDeliverCompactEvent(TVP_COMPACT_LEVEL_MAX);
	if (!TVPIsSoftwareRenderManager())
		iTVPTexture2D::RecycleProcess();
	exit(code);
#endif
}

tjs_int TVPGetSystemFreeMemory()
{
	__debugbreak(); throw;
	return 0;
}

int TVPShowSimpleMessageBox(const char *pszText, const char *pszTitle, unsigned int nButton, const char **btnText) {
#if 0
	__debugbreak(); throw;
	return 0;
#else
	::MessageBoxW(NULL, UTF82WCS(pszText).c_str(),
		UTF82WCS(pszTitle).c_str(), MB_OK | MB_ICONINFORMATION);
	return 0;
#endif
}

bool TVPCheckStartupArg() {
#if 0
	__debugbreak(); throw;
	return false;
#else
	return false;
#endif
}

std::string TVPGetCurrentLanguage() {
#if 0
	__debugbreak(); throw;
	return "";
#else
	LANGID lid = GetUserDefaultUILanguage();
	const LCID locale_id = MAKELCID(lid, SORT_DEFAULT);
	char code[10] = { 0 };
	char country[10] = { 0 };
	GetLocaleInfoA(locale_id, LOCALE_SISO639LANGNAME, code, sizeof(code));
	GetLocaleInfoA(locale_id, LOCALE_SISO3166CTRYNAME, country, sizeof(country));
	std::string ret = code;
	if (country[0]) {
		for (int i = 0; i < sizeof(country) && country[i]; ++i) {
			char c = country[i];
			if (c <= 'Z' && c >= 'A') {
				country[i] += 'a' - 'A';
			}
		}
		ret += "_";
		ret += country;
	}
	return ret;
#endif
}

void TVPProcessInputEvents() {}

void TVPControlAdDialog(int adType, int arg1, int arg2) {
#if 0
	__debugbreak(); throw;
#else
	CCLOG("===================%d, %d, %d", adType, arg1, arg2);
#endif
}

std::vector<std::string> TVPGetDriverPath() {
#if 0
	__debugbreak(); throw;
	std::vector<std::string> ret;
	return ret;
#else
	std::vector<std::string> ret;
#ifndef _MSC_VER
	char drv[4] = { 'C', ':', '/', 0 };
	for (char c = 'C'; c <= 'Z'; ++c) {
		drv[0] = c;
		switch (GetDriveTypeA(drv)) {
		case DRIVE_REMOVABLE:
		case DRIVE_FIXED:
		case DRIVE_REMOTE:
			ret.push_back(drv);
			break;
		}
	}
#else
	OutputDebugString(L"=========================>TVPGetDriverPath()\n");
	ret.push_back("C:/");
#endif
	return ret; 
#endif
}

bool TVPCheckStartupPath(const std::string &path) {
#if 0
	__debugbreak(); throw;
	return false;
#else
	OutputDebugString(L"=========================>TVPCheckStartupPath()\n");
	return true;
#endif
}

tjs_int TVPGetSelfUsedMemory()
{
	__debugbreak(); throw;
	return 0;
}

void TVPForceSwapBuffer() {
#if 0
	__debugbreak(); throw;
#else
	OutputDebugString(L"=========================>TVPForceSwapBuffer() not imp\n");
#endif
}

bool TVPDeleteFile(const std::string &filename) {
	__debugbreak(); throw;
	return false;
}

bool TVPRenameFile(const std::string &from, const std::string &to) {
	__debugbreak(); throw;
	return false;
}

void TVPSendToOtherApp(const std::string &filename) {
	__debugbreak(); throw;
}

void TVPFetchSDCardPermission() {
	__debugbreak(); throw;
}

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
std::string TVPGetDefaultFileDir() {
	wchar_t buf[MAX_PATH];
	_wgetcwd(buf, sizeof(buf) / sizeof(buf[0]));
	wchar_t *p = buf;
	while (*p) {
		if (*p == '\\') *p = '/';
		++p;
	}
	return converter.to_bytes(buf);
}
std::vector<std::string> TVPGetAppStoragePath() {
#if 0
	__debugbreak(); throw;
	std::vector<std::string> ret;
	return ret;
#else
	std::vector<std::string> ret;
	ret.push_back(TVPGetDefaultFileDir());
	return ret;
#endif
}

void TVPOutputDebugString(const char *str) {
	::OutputDebugStringA(str);
}


















#elif defined(ANDROID) || defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)


#include "cocos2d.h"
#include <string>
std::string TVPGetDeviceID();
#include "minizip/unzip.h"
#include "zlib.h"
#include <map>
#include <string>
#include <vector>
#include "tjs.h"
//#include "MsgIntf.h"
//#include "md5.h"
//#include "DebugIntf.h"

//???
//#ifndef _MSC_VER
//#include "platform/android/jni/JniHelper.h"
//#endif


#include <set>
#include <sstream>
#include "SysInitIntf.h"
//#include "platform/CCFileUtils.h"
#include "ConfigManager/LocaleConfigManager.h"
//#include "Platform.h"
//#include "platform/CCCommon.h"

//???
//#ifndef _MSC_VER
//#include <EGL/egl.h>
//#endif


//#include <queue>
//#include "base/CCDirector.h"
//#include "base/CCScheduler.h"
#include <unistd.h>
//#include <fcntl.h>
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
#include <stdio.h>
#else
#include <android/log.h>
#endif
//#include "TickCount.h"
//#include "StorageImpl.h"
//#include "ConfigManager/IndividualConfigManager.h"
#include "EventIntf.h"
#include "RenderManager.h"
#include <sys/stat.h>
#include "Platform.h"
#include <locale>         // std::wstring_convert
#include <string>
#include <wchar.h>

# define LOG_TAG "AndroidUtils"
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
# define LOGE(...) ((void)printf(__VA_ARGS__),(void)printf("\n"))
#else
# define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#endif

int TVPShowSimpleMessageBox(const ttstr & text, const ttstr & caption, const std::vector<ttstr> &vecButtons)
{
	LOGE("%s, %s", text.AsStdString().c_str(), caption.AsStdString().c_str());
	return 0;
}

std::string TVPGetPackageVersionString() {
	return "ver 1.0.0";
}
static std::wstring strInputBoxTitle;
static std::wstring strInputBoxPrompt;
static std::wstring strInputBoxText;

int TVPShowSimpleInputBox(ttstr &text, const ttstr &caption, const ttstr &prompt, const std::vector<ttstr> &vecButtons) {
	LOGE("TVPShowSimpleInputBox: %s, %s", text.AsStdString().c_str(), caption.AsStdString().c_str());
#if !defined(LINUX)	&& !defined(__APPLE__) && !defined(__MINGW32__)
	throw;
#endif	
	return 0;
}


bool TVPCheckExistentLocalFolder(const ttstr &name);
bool TVPCreateFolders(const ttstr &folder);
static bool _TVPCreateFolders(const ttstr &folder)
{
	// create directories along with "folder"
	if (folder.IsEmpty()) return true;

	if (TVPCheckExistentLocalFolder(folder))
		return true; // already created

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if (p[i] == TJS_W(':')) return true;

	while (i >= 0 && (p[i] == TJS_W('/') || p[i] == TJS_W('\\'))) i--;

	if (i >= 0 && p[i] == TJS_W(':')) return true;

	for (; i >= 0; i--)
	{
		if (p[i] == TJS_W(':') || p[i] == TJS_W('/') ||
			p[i] == TJS_W('\\'))
			break;
	}

	ttstr parent(p, i + 1);
	if (!TVPCreateFolders(parent)) return false;

#if defined(__MINGW32__)
	return !mkdir(folder.AsStdString().c_str());
#else
	return !mkdir(folder.AsStdString().c_str(), S_IRWXU|S_IRGRP|S_IROTH);
#endif

}
bool TVPCreateFolders(const ttstr &folder)
{
	if (folder.IsEmpty()) return true;

	const tjs_char *p = folder.c_str();
	tjs_int i = folder.GetLen() - 1;

	if (p[i] == TJS_W(':')) return true;

	if (p[i] == TJS_W('/') || p[i] == TJS_W('\\')) i--;

	return _TVPCreateFolders(ttstr(p, i + 1));
}

//see environ/win32/Platform.cpp
tjs_uint32 TVPGetRoughTickCount32()
{
	tjs_uint32 uptime = 0;
	struct timespec on;
	if (clock_gettime(CLOCK_MONOTONIC, &on) == 0)
		uptime = on.tv_sec * 1000 + on.tv_nsec / 1000000;
	return uptime;
}


bool TVP_stat(const tjs_char *name, tTVP_stat &s) {
	tTJSNarrowStringHolder holder(name);
	return TVP_stat(holder, s);
}

//#undef st_atime
//#undef st_ctime
//#undef st_mtime
//int stat64(const char* __path, struct stat64* __buf) __INTRODUCED_IN(21); // force link it !
bool TVP_stat(const char *name, tTVP_stat &s) {
	struct stat t;
	// static_assert(sizeof(t.st_size) == 4, "");
#if defined(__MINGW32__)
#else	
	static_assert(sizeof(t.st_size) == 8, "");
#endif	
	bool ret = !stat(name, &t);
	s.st_mode = t.st_mode;
	s.st_size_ = t.st_size;
	s.st_atime_ = t.st_atime;
	s.st_mtime_ = t.st_mtime;
	s.st_ctime_ = t.st_ctime;
	return ret;
}

bool TVPWriteDataToFile(const ttstr &filepath, const void *data, unsigned int size) {
	LOGE("====================>TVPWriteDataToFile: %s", filepath.AsStdString().c_str());
	FILE* handle = fopen(filepath.AsStdString().c_str(), "wb");
	if (handle) {
		bool ret = fwrite(data, 1, size, handle) == size;
		fclose(handle);
		return ret;
	}
	return false;
}

void TVPExitApplication(int code) {
	TVPDeliverCompactEvent(TVP_COMPACT_LEVEL_MAX);
	if (!TVPIsSoftwareRenderManager())
		iTVPTexture2D::RecycleProcess();
	exit(code);
}

tjs_int TVPGetSystemFreeMemory()
{
	throw;
	return 0;
}

int TVPShowSimpleMessageBox(const char *pszText, const char *pszTitle, unsigned int nButton, const char **btnText) {
	LOGE("TVPShowSimpleMessageBox: %s, %s", pszText, pszTitle);
	return 0;
}

bool TVPCheckStartupArg() {
	return false;
}

std::string TVPGetCurrentLanguage() {
	//throw;
	return "zh_cn";
}

void TVPProcessInputEvents() {}

void TVPControlAdDialog(int adType, int arg1, int arg2) {
	CCLOG("===================%d, %d, %d", adType, arg1, arg2);
}

std::vector<std::string> TVPGetDriverPath() {
	//throw;
	std::vector<std::string> ret;
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
	ret.push_back("/mnt/SDCARD");
#else		
	ret.push_back("/mnt/sdcard");
#endif	
	return ret;
}

bool TVPCheckStartupPath(const std::string &path) {
	LOGE("=========================>TVPCheckStartupPath()");
	return true;
}

tjs_int TVPGetSelfUsedMemory()
{
	throw;
	return 0;
}

void TVPForceSwapBuffer() {
	LOGE("=========================>TVPForceSwapBuffer() not imp");
}

bool TVPDeleteFile(const std::string &filename) {
	throw;
	return false;
}

bool TVPRenameFile(const std::string &from, const std::string &to) {
	throw;
	return false;
}

void TVPSendToOtherApp(const std::string &filename) {
	throw;
}

void TVPFetchSDCardPermission() {
	throw;
}

std::string TVPGetDefaultFileDir() {
	char buf[255];
#if defined(__MINGW32__)	
	GetCurrentDirectoryA(sizeof(buf) / sizeof(buf[0]), buf);	
#else	
	getcwd(buf, sizeof(buf) / sizeof(buf[0]));
#endif	
	char *p = buf;
	while (*p) {
		if (*p == '\\') *p = '/';
		++p;
	}
	return std::string(buf);
}
std::vector<std::string> TVPGetAppStoragePath() {
	std::vector<std::string> ret;
	ret.push_back(TVPGetDefaultFileDir());
	return ret;
}

void TVPOutputDebugString(const char *str) {
	LOGE("%s", str);
}



//---------------------------------------------
//NOTE:added
static int GetExternalStoragePath(std::vector<std::string> &ret) {
	int count = 0;
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
	ret.push_back(std::string("/mnt/SDCARD"));
#else	
	ret.push_back(std::string("/mnt/sdcard"));
#endif	
	++count;
	return count;
}
static std::string GetInternalStoragePath() {
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
	return std::string("/mnt/SDCARD/");
#else	
	return std::string("/mnt/sdcard/");
#endif
}
static std::string GetApkStoragePath() {
#if defined(LINUX) || defined(__APPLE__) || defined(__MINGW32__)
	return std::string("/mnt/SDCARD/");
#else	
	return std::string("/mnt/sdcard/");
#endif
}

std::vector<ttstr> Android_GetExternalStoragePath() {
	static std::vector<ttstr> ret;
	if (ret.empty()) {
		std::vector<std::string> pathlist;
		GetExternalStoragePath(pathlist);
		for (const std::string &path : pathlist) {
			std::string strPath = "file://.";
			strPath += path;
			ret.push_back(strPath);
		}
	}
	return ret;
}

ttstr Android_GetInternalStoragePath() {
	static ttstr strPath;
	if (strPath.IsEmpty()) {
		strPath = "file://.";
		strPath += GetInternalStoragePath();
	}
	return strPath;
}

ttstr Android_GetApkStoragePath() {
	static ttstr strPath;
	if (strPath.IsEmpty()) {
		strPath = "file://.";
		strPath += GetApkStoragePath();
	}
	return strPath;
}

void TVPHideIME() {
	LOGE("%s", "TVPHideIME");
}

void TVPShowIME(int x, int y, int w, int h) {
	LOGE("%s", "TVPShowIME");
}





















#else
#error unknown platform
#endif