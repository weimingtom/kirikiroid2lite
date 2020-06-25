/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "cocostudio/CCComAudio.h"
#if 0
#include "audio/include/SimpleAudioEngine.h"
#endif
#include "platform/CCFileUtils.h"

namespace cocostudio {

IMPLEMENT_CLASS_COMPONENT_INFO(ComAudio)
ComAudio::ComAudio(void)
: _filePath("")
, _loop(false)
{
    _name = "CCComAudio";
}

ComAudio::~ComAudio(void)
{
    
}

bool ComAudio::init()
{
    return true;
}

void ComAudio::onEnter()
{
}

void ComAudio::onExit()
{
    stopBackgroundMusic(true);
    stopAllEffects();
}

bool ComAudio::isEnabled() const
{
    return _enabled;
}

void ComAudio::setEnabled(bool b)
{
    _enabled = b;
}


bool ComAudio::serialize(void* r)
{
    bool ret = false;
	do
	{
		CC_BREAK_IF(r == nullptr);
		SerData *serData = (SerData *)(r);
		const rapidjson::Value *v = serData->_rData;
		stExpCocoNode *cocoNode = serData->_cocoNode;
        CocoLoader *cocoLoader = serData->_cocoLoader;
		const char *className = nullptr;
		const char *comName = nullptr;
		const char *file = nullptr;
		std::string filePath;
		int resType = 0;
		bool loop = false;
		if (v != nullptr)
		{
			className = DICTOOL->getStringValue_json(*v, "classname");
			CC_BREAK_IF(className == nullptr);
			comName = DICTOOL->getStringValue_json(*v, "name");
			const rapidjson::Value &fileData = DICTOOL->getSubDictionary_json(*v, "fileData");
			CC_BREAK_IF(!DICTOOL->checkObjectExist_json(fileData));
			file = DICTOOL->getStringValue_json(fileData, "path");
			CC_BREAK_IF(file == nullptr);
			resType = DICTOOL->getIntValue_json(fileData, "resourceType", -1);
			CC_BREAK_IF(resType != 0);
			loop = DICTOOL->getIntValue_json(*v, "loop") != 0? true:false;
		}
		else if (cocoNode != nullptr)
		{
			className = cocoNode[1].GetValue(cocoLoader);
			CC_BREAK_IF(className == nullptr);
			comName = cocoNode[2].GetValue(cocoLoader);
			stExpCocoNode *pfileData = cocoNode[4].GetChildArray(cocoLoader);
			CC_BREAK_IF(!pfileData);
			file = pfileData[0].GetValue(cocoLoader);
			CC_BREAK_IF(file == nullptr);
			resType = atoi(pfileData[2].GetValue(cocoLoader));
			CC_BREAK_IF(resType != 0);
			loop = atoi(cocoNode[5].GetValue(cocoLoader)) != 0? true:false;
			ret = true;
		}
		if (comName != nullptr)
		{
			setName(comName);
		}
		else
		{
			setName(className);
		}
		if (file != nullptr)
		{
            if (strcmp(file, "") == 0)
            {
                continue;
            }
			filePath.assign(cocos2d::FileUtils::getInstance()->fullPathForFilename(file));
		}
		if (strcmp(className, "CCBackgroundAudio") == 0)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			std::string::size_type pos = filePath.find(".mp3");
			if (pos  == filePath.npos)
			{
				continue;
			}
			filePath.replace(pos, filePath.length(), ".wav");
#endif
			preloadBackgroundMusic(filePath.c_str());
			setLoop(loop);
			playBackgroundMusic(filePath.c_str(), loop);
		}
		else if(strcmp(className, "CCComAudio") == 0)
		{
			preloadEffect(filePath.c_str());
		}
		else
		{
			CC_BREAK_IF(true);
		}
		ret = true;
	}while (0);
	return ret;
}

ComAudio* ComAudio::create(void)
{
    ComAudio * pRet = new (std::nothrow) ComAudio();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

void ComAudio::end()
{
#if 0
   CocosDenshion::SimpleAudioEngine::end();
#endif
}

void ComAudio::preloadBackgroundMusic(const char* pszFilePath)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(pszFilePath);
#endif
	setFile(pszFilePath);
	setLoop(false);
}

void ComAudio::playBackgroundMusic(const char* pszFilePath, bool loop)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, loop);
#endif    
}

void ComAudio::playBackgroundMusic(const char* pszFilePath)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath);
#endif
}

void ComAudio::playBackgroundMusic()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_filePath.c_str(), _loop);
#endif
}

void ComAudio::stopBackgroundMusic(bool bReleaseData)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
#endif
}

void ComAudio::stopBackgroundMusic()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#endif
}

void ComAudio::pauseBackgroundMusic()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
}

void ComAudio::resumeBackgroundMusic()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
}

void ComAudio::rewindBackgroundMusic()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->rewindBackgroundMusic();
#endif
}

bool ComAudio::willPlayBackgroundMusic()
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->willPlayBackgroundMusic();
#else
	return false;
#endif
}

bool ComAudio::isBackgroundMusicPlaying()
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
#else
	return false;
#endif
}

float ComAudio::getBackgroundMusicVolume()
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
#else
	return 0;
#endif
}

void ComAudio::setBackgroundMusicVolume(float volume)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
#endif
}

float ComAudio::getEffectsVolume()
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();
#else
	return 0;
#endif
}

void ComAudio::setEffectsVolume(float volume)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
#endif
}

unsigned int ComAudio::playEffect(const char* pszFilePath, bool loop)
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, loop);
#else
	return 0;
#endif
}

unsigned int ComAudio::playEffect(const char* pszFilePath)
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath);
#else
	return 0;
#endif
}

unsigned int ComAudio::playEffect()
{
#if 0
	return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_filePath.c_str(), _loop);
#else
	return 0;
#endif
}

void ComAudio::pauseEffect(unsigned int nSoundId)
{
#if 0
    return CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(nSoundId);
#endif
}

void ComAudio::pauseAllEffects()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void ComAudio::resumeEffect(unsigned int nSoundId)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(nSoundId);
#endif
}

void ComAudio::resumeAllEffects()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

void ComAudio::stopEffect(unsigned int nSoundId)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(nSoundId);
#endif
}

void ComAudio::stopAllEffects()
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
#endif
}

void ComAudio::preloadEffect(const char* pszFilePath)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(pszFilePath);
#endif
    setFile(pszFilePath);
    setLoop(false);
}

void ComAudio::unloadEffect(const char *pszFilePath)
{
#if 0
    CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(pszFilePath);
#endif
}

void ComAudio::setFile(const char* pszFilePath)
{
	_filePath.assign(pszFilePath);
}

void ComAudio::setLoop(bool loop)
{
	_loop = loop;
}

const char* ComAudio::getFile()
{
	return _filePath.c_str();
}

bool ComAudio::isLoop()
{
	return _loop;
}

}
