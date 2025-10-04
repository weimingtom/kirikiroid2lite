#pragma once
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "platform/win32/CCFileUtils-win32.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <Foundation/NSBundle.h>
#import "platform/apple/CCFileUtils-apple.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
#include "platform/linux/CCFileUtils-linux.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
//don't use #import <Foundation/NSBundle.h>
//#import "platform/apple/CCFileUtils-apple.h"
#endif
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#ifndef _MSC_VER
#include "external/unzip/unzip.h"
#else
#include "unzip/unzip.h"
#endif
#endif
#include <unordered_map>
#include <pthread.h>
#if defined(ANDROID)
#include <android/log.h>
#endif

NS_CC_BEGIN


/**
* @addtogroup platform
* @{
*/

//! @brief  Helper class to handle file operations
class FileUtilsAndroid_mod : public FileUtils
{
	friend class FileUtils;
public: //FIXME: modify here
	FileUtilsAndroid_mod();
	/**
	* @js NA
	* @lua NA
	*/
	virtual ~FileUtilsAndroid_mod();

//	static void setassetmanager(AAssetManager* a);
//	static AAssetManager* getAssetManager() { return assetmanager; }

	/* override funtions */
	bool init();

	virtual std::string getNewFilename(const std::string &filename) const override;

	/** @deprecated Please use FileUtils::getDataFromFile or FileUtils::getStringFromFile instead. */
	virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t * size) override;

	/**
	*  Gets string from a file.
	*/
	virtual std::string getStringFromFile(const std::string& filename) override;

	/**
	*  Creates binary data from a file.
	*  @return A data object.
	*/
	virtual Data getDataFromFile(const std::string& filename) override;

	virtual std::string getWritablePath() const;
	virtual bool isAbsolutePath(const std::string& strPath) const;

//private: //FIXME: modify here
protected:
	virtual bool isFileExistInternal(const std::string& strFilePath) const;
	Data getData(const std::string& filename, bool forString);

//	static AAssetManager* assetmanager;
};

// end of platform group
/// @}

/**
 * @addtogroup platform
 * @{
 */

//! @brief  Helper class to handle file operations
class FileUtilsWin32_mod : public FileUtils
{
public: //FIXME: modify here
    friend class FileUtils;
    FileUtilsWin32_mod();
    /* override funtions */
    bool init();
    virtual std::string getWritablePath() const;
    virtual bool isAbsolutePath(const std::string& strPath) const;
protected:

    virtual bool isFileExistInternal(const std::string& strFilePath) const;
    
    /**
     *  Gets resource file data
     *
     *  @param[in]  filename    The resource file name which contains the path.
     *  @param[in]  mode        The read mode of the file.
     *  @param[out] size        If the file read operation succeeds, it will be the data size, otherwise 0.
     *  @return Upon success, a pointer to the data is returned, otherwise NULL.
     *  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
     */
    virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t * size) override;

    /**
     *  Gets string from a file.
     */
    virtual std::string getStringFromFile(const std::string& filename) override;
    
    /**
     *  Creates binary data from a file.
     *  @return A data object.
     */
    virtual Data getDataFromFile(const std::string& filename) override;

    /**
     *  Gets full path for filename, resolution directory and search path.
     *
     *  @param filename The file name.
     *  @param resolutionDirectory The resolution directory.
     *  @param searchPath The search path.
     *  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
     */
    virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const override;

    /**
     *  Gets full path for the directory and the filename.
     *
     *  @note Only iOS and Mac need to override this method since they are using
     *        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
     *        Other platforms will use the default implementation of this method.
     *  @param directory The directory contains the file we are looking for.
     *  @param filename  The name of the file.
     *  @return The full path of the file, if the file can't be found, it will return an empty string.
     */
    virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const override;

};

//! @brief  Helper class to handle file operations
class CC_DLL FileUtilsLinux_mod : public FileUtils
{
public: //FIXME: modify here
    friend class FileUtils;
    FileUtilsLinux_mod();
    std::string _writablePath;
public:
    /* override funtions */
    bool init();
//    virtual std::string getWritablePath() const;
    std::string getWritablePath() const;
//private: //FIXME: modify here
//protected:
//    virtual bool isFileExistInternal(const std::string& strFilePath) const;
     bool isFileExistInternal(const std::string& strFilePath) const;
};

class CC_DLL FileUtilsApple_mod : public FileUtils
{
public: //FIXME: modify here
    friend class FileUtils;
    FileUtilsApple_mod();
    std::string _writablePath;
public:
    /* override funtions */
    bool init();
//    virtual std::string getWritablePath() const;
    std::string getWritablePath() const;
//private: //FIXME: modify here
//protected:
//    virtual bool isFileExistInternal(const std::string& strFilePath) const;
     bool isFileExistInternal(const std::string& strFilePath) const;
};

typedef
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
FileUtilsWin32_mod
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
FileUtilsApple
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
FileUtilsAndroid_mod
#elif CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
FileUtilsLinux_mod
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
FileUtilsApple_mod
#endif
FileUtilsInherit;

class CustomFileUtils : public FileUtilsInherit
{
public:
	CustomFileUtils() {
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "CustomFileUtils.h", "%s", "*** *** CustomFileUtils");
#endif
		pthread_mutex_init(&_lock, NULL);
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "CustomFileUtils.h", "%s", "*** *** CustomFileUtils 2");
#endif
	}
	
	virtual ~CustomFileUtils() {
		pthread_mutex_destroy(&_lock);
	}

	void addAutoSearchArchive(const std::string& path);
	virtual std::string fullPathForFilename(const std::string &filename) const override;
	virtual std::string getStringFromFile(const std::string& filename) override;
	virtual Data getDataFromFile(const std::string& filename) override;
	virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t *size) override;
	virtual bool isFileExistInternal(const std::string& strFilePath) const override;
	virtual bool isDirectoryExistInternal(const std::string& dirPath) const override;
	virtual bool init() /*override*/ {
		return FileUtilsInherit::init();
	}

private:
	unsigned char* getFileDataFromArchive(const std::string& filename, ssize_t *size);

	std::unordered_map<std::string, std::pair<unzFile, unz_file_pos> > _autoSearchArchive;
	pthread_mutex_t _lock;
};

NS_CC_END

extern cocos2d::CustomFileUtils *TVPCreateCustomFileUtils();
