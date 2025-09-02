# kirikiroid2lite
[WIP] kirikiroid2 lite version, without most dependencies.    
Adding audio function for Android and Windows since 2025/5/15 and 2024/12/21.     
NOTE: Kirikiroid2lite is a mod about kirikiroid2 and cocos2d-x framework,  
it's **NOT** a fork of kirikiri2 and kirikiriz, so it may be not what you want.  
It is just for hacking code. You had better use krkrsdl2 or other implementations instead: https://github.com/krkrsdl2/krkrsdl2   
```
  予谓菊，花之隐逸者也；  
  牡丹，花之富贵者也；  
  莲，花之君子者也。  
  噫！菊之爱，陶后鲜有闻。  
  莲之爱，同予者何人？  
  牡丹之爱，宜乎众矣！  
  ——周敦颐《爱莲说》    
```
```
  我自横刀向天笑，去留肝胆两昆仑
  ——谭嗣同《狱中题壁》
```

## Original code  
* https://github.com/zeas2/Kirikiroid2/tree/1.3.8  
* https://gitee.com/weimingtom/kirikiroid2_fork/blob/master/vendor/Kirikiroid2_1.3.8_github_20180625.rar   

## Status  
* Working in progress.    
* Windows support: Windows 7 success.  
* Android support: with many bugs now.    

## History  
* 2025-09-02 ：Merge kirikiroid2-miyoo-a30 sources and compile linux version successfully.
* 2025-05-15 : Adding Android audio function and migrate to NDK r25, but Windows version compiling status is unknown.  
* 2024-12-21 : Adding Windows audio function.  
* 2020-06-25 13:23 : first commit on github, migrated from gitee to github, first available version for Android NDK and Win7 VS2013        
* 2019-03-17 : (gitee) Merge pthread project  
* 2019-02-02 : (gitee) Merge OpenCV2 project  
* 2018-08-11 : (gitee) No boost depend, use pthread instead  
* 2018-07-25 : (gitee) First available version with boost and vs2010  

## How to build for Windows  
* WARNING !!! don't download source zip or tar.gz file on github web page, please use git clone to download source files.  
* If you download zip or tar.gz source file from github web page, you should use notepad++ to batch replace \n with \r\n.  
* Open project\win32\cocos2d-win32.vs2013.sln with vs2013.  

## How to build for Android  
* Modify project\android\console.bat, point to NDK path.  
* Run project\android\console.bat  
* Run ndk-build, or ndk-build -j8, or ndk-build NDK_DEBUG=1 V=1.   
* Import, compile and run with Android ADT, see project\android\ .project   
* Or import, compile and run with Android Studio, see project\androidstudio     
* (FIXME) Change android permission toast to dialog, like this
```
Toast.makeText(getApplicationContext(), "You grant write external storage permission. Please restart to continue.", Toast.LENGTH_LONG).show();
```

## (TODO) How to build for Linux (Xubuntu 20.04 64bit)
* (FIXME) Fix initWithRect and setDesignResolutionSize problems
* (FIXME) Two SDL windows, need to remove 'SDL2 joystick capture'
* $ sudo apt install libglfw3-dev libfreetype-dev
* $ make clean
* $ make -j8
* $ make test
* $ ./project/kirikiroid2

## Minimum version of Cocos2d-x  
* VS2013 version cocos2d-x 3.6, see   
https://github.com/weimingtom/cocos2dx36_vs2013  
```
cocos2d-x-3.6_hello_v11_migrate_finish.rar  
https://github.com/weimingtom/CrossApp_mingw  
```
* VC6 version cocos2d-x 2.2.6, see  
https://github.com/weimingtom/cocos2dx226_vc6  

## Test Data, and builtin file browser start path   
* https://gitee.com/weimingtom/kirikiroid2_fork/tree/master/test_data/atest3  
* see TVPGetDriverPath (???)  
```
Please Copy atest3 folder to F:\atest3
F: is set in tests\cpp-empty-test\Classes\core\environ\android\AndroidUtils.cpp
Search ret.push_back("F:/");
```
* Setup start folder C: shown when started     
Please copy test folder like atest3 to C:\atest3  
C: is set in src/core/environ/android/AndroidUtils.cpp  
Search ret.push_back("C:/");  
see https://github.com/weimingtom/kirikiroid2lite/blob/master/src/core/environ/android/AndroidUtils.cpp  
```
关于程序启动后开始时，文件浏览器的起始路径（盘符），是修改src\core\environ\android\AndroidUtils.cpp的
std::vector<std::string> TVPGetDriverPath()
方法里面的ret.push_back，例如这样：
std::vector<std::string> TVPGetDriverPath() {
	std::vector<std::string> ret;
	OutputDebugString(L"=========================>TVPGetDriverPath()\n");
	ret.push_back("C:/");
	return ret;
}
```

## VS2013, Windows 7 development environment  
* Visual Studio 2013旗舰版, windowssdk81.rar    
* SDKDDKVer.h: No such file or directory  
Need to install windowssdk81.rar (Windows Kits 8.1)    

## Sound playing function   
* tTJSNI_WaveSoundBuffer::Open  
see src\core\sound\win32\WaveImpl.cpp  
* TVPCreateWaveDecoder   
```
kirikiroid2研究，最近打算重新改这份代码（我非原作者），
把声音功能重新加回去（因为我上次kirikiroid2lite把声音去掉了或者没调通）。
播放声音的后退调用堆栈很深，不过可以猜测到它的大概位置，
例如我断点断在tTJSNI_WaveSoundBuffer::Open这个方法里面
（或者搜索TVPCreateWaveDecoder），当我滑动鼠标到主菜单按钮上，
就会调用这个方法，传入参数storagename是ogg文件名，
至于怎么播放，我后面再研究

kirikiroid2研究。从我在所有try-catch输出日志（我魔改了代码）可以看到抛了C++异常，
然后在VS2013中菜单-异常-把第一列和第二列的断点单选框全部勾上，然后重新进入调试模式，
发现断点断在TVPCreateWaveDecoder这个函数，就是说在tTJSNI_WaveSoundBuffer::Open的开头
TVPEnsurePrimaryBufferPlay找不到ogg的解码器，所以就没办法播放声音了。
为什么要用VS的异常断点呢？因为krkr的代码用了rethrow，所以没办法从日志中清楚看到调用堆栈，
然而第一次触发异常的地方才是最接近于出问题的地方。后续想办法解决，
看能否修改到可以调用到Play和StartPlay方法

search baidupan, libogg, libvorbis
kirikiroid2研究。上次的ogg问题，可以简单加入libogg和libvorbis代码，
我直接就不用VS2013创建子工程了，直接加入到cpp-empty-test工程中。
然后VorbisWaveDecoderCreator和vorbisWaveDecoderCreator相关的代码重新打开即可，
还有VorbisWaveDecoder.cpp（除了opus和opusfile库）。运行的话依旧无法播放ogg声音，
但可以看到Play和StartPlay已经能调用了，
只是我的代码还没填充TVPInitDirectSound和TVPCreateSoundBuffer的实现，
只要找时间补充代码到core\sound\win32\WaveMixer.cpp即可
src\core\sound\VorbisWaveDecoder.cpp
VorbisWaveDecoderCreator
vorbisWaveDecoderCreator
src\core\sound\win32\WaveMixer.cpp
==================>TVPInitDirectSound not imp
==================>TVPCreateSoundBuffer not imp

kirikiroid2研究。关于TVPInitDirectSound和TVPCreateSoundBuffer的实现填充问题，
它本来应该是通过DirectSound实现（DirectX），然后它kirikiroid2原版是通过SDL或者Oboe实现。
对我而言oboe和dsound应该不用考虑了，比较可行应该是SDL或者通过SDL的底层
（例如waveOutWrite，winmm，或者安卓的AudioTrack）去实现播放，
当然我应该是先研究windows版的，但SDL播放声音并不是很熟悉，所以可能要先琢磨较长时间。
至于krkrsdl2是怎么播放声音？它可能也是SDL2，或者可能是通过openal和FAudio之类方法实现播放，
具体以后会深入研究，但我不会用太复杂的写法，勉强能用就算了，可能只保留最简单的实现方法

VideoProject_v2_play_audio_success.7z
https://github.com/rururutan/np2s/blob/master/np2/sdl/win32s/SDLaudio.c

kirikiroid2研究。打算先从SDL2入手研究声音播放。我猜测声音问题（或者说运行原理）应该不会很难，
可能就是格式有区别或者存在转换问题罢了（例如采样率，码率，单声道之类）。
为什么是SDL2而不是SDL1.2呢？因为我发现它用的是SDL_OpenAudioDevice而不是SDL_OpenAudio，
那很有可能它用的是更兼容安卓的SDL2，甚至可能直接用它播放声音都可以，
而不只是拿来转换格式或者采样率

kirikiroid2研究。我的天，我把声音功能弄出来了，代码看得我汗流浃背——
其实kirikiroid2本身已经实现了SDL2声音播放，
只是它弄得很长且复杂——难道是从哪里得到的代码，连ARM特定代码都来了，
用于混音。不过跳过不执行这些看不懂的代码
（src/core/sound/ARM/wavemix_arm.c），在windows下仍然是可以播放的，
具体流程我后面再研究分析

2024-12-21
kirikiroid2研究。我把关于声音部分的修改放到gh上：weimingtom/kirikiroid2lite，
目前的解决办法是用原版的SDL2播放（不是我想出来的，是原版就有的功能），
然后去掉一些可以省略的过程——其实我怀疑SDL2本身就有混音功能，似乎没必要写得那么复杂，不过我不清楚具体细节。
注意kirikiroid2是半开源的，有很多问题，然后我又继续往里面改，所以有更多的问题和bug，
所以这目前没什么实用，而且我也还没有把声音问题的解决用安卓跑通，
现在只是windows版的大概能听到声音和背景音乐罢了，还有非常多的运行期报错。
可能暂时先改到这里，以后再完善安卓端的声音功能

大概把kirikiroid2lite的安卓版的声音输出跑通了
（上一次大修改，未加声音输出的版本是五年前）。当然，
还有别的很多bug，效果如下。我的修改方法是：
把Cocos2dxActivity的基类改成SDLActivity，否则SDL_Init会失败；
把目标平台改成安卓6：targetSdk 23，这样就能在安卓12上访问到sd卡
（不过安卓13以上应该不行，这个问题暂时先不管），
我测试过用ADT和用Android Studio都能编译，
我很快会把两个工程都开源出去

我把kirikiroid2lite的NDK兼容能力从r10e提升到r25的clang
（最新的NDK移除了gcc只保留clang），改得头大，但可以改，
我暂时测试这两个NDK版本。要同时兼容新旧NDK需要修改很多地方，
包括cocos2d-x的代码和kirikiroid2的代码用clang编译都有问题，
还需要分静态库才能链接动态库（NDK的bug，命令行不能太长），
如果是最新版本的NDK好像是不需要考虑拆分出静态库来避免
长命令行bug的，不过我还是拆分了。
这些修改我稍后也会分开开源出去
```
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveImpl.cpp:3065  
void tTJSNI_WaveSoundBuffer::Open(const ttstr & storagename)  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:437   
void TVPInitDirectSound(int freq)  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:424  
static iTVPAudioRenderer *CreateAudioRenderer()  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:405  
bool tTVPAudioRendererSDL::Init() override  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveImpl.cpp:2998  
void tTJSNI_WaveSoundBuffer::Play()  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:437  
void TVPInitDirectSound(int freq)  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveImpl.cpp:2926  
void tTJSNI_WaveSoundBuffer::StartPlay()  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:437   
void TVPInitDirectSound(int freq)  
* C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:458    
iTVPSoundBuffer* TVPCreateSoundBuffer(tTVPWaveFormat &fmt, int bufcount)  
* extern "C" void TVPWaveMixer_ASM_Init(FAudioMix **func16, FAudioMix **func32); not used  
see C:\work\krkr\kirikiroid2lite-master\src\core\sound\win32\WaveMixer.cpp:267   
TVPWaveMixer_ASM_Init(_AudioMixS16, _AudioMixF32);    
see C:\work\krkr\kirikiroid2lite-master\src\core\sound\ARM\wavemix_arm.c:98  
void TVPWaveMixer_ASM_Init(FAudioMix **func16, FAudioMix **func32)  
* key code:
```
WaveMixer.cpp:
_playback_id = SDL_OpenAudioDevice(nullptr, false, &_spec, &_spec, SDL_AUDIO_ALLOW_ANY_CHANGE);
SDL_PauseAudioDevice(_playback_id, false);
DoMixAudio(dst, src, samples, vol);
_spec.callback = [](void *p, Uint8 *s, int l) {
((iTVPAudioRenderer*)p)->FillBuffer(s, l);

3 entries:
void TVPInitDirectSound(int freq)
void TVPUninitDirectSound()
iTVPSoundBuffer* TVPCreateSoundBuffer(tTVPWaveFormat &fmt, int bufcount)

//(done) TODO:
//* (done, skip) TVPWaveMixer_ASM_Init not used
//* (done, use pthread_mutex_t/pthread_mutex_init/pthread_mutex_lock/pthread_mutex_unlock) 
//      boost::lock_guard<boost::mutex> not used
//
```

## How to link SDL2 Android  
* krkrsdl2_ndk_v4_run_success.7z  

## Bugs
* Only support software renderer (软件渲染器) in the global settings, not support OpenGL renderer（OpenGL渲染器，实验性）

## SDCard permission check
* If you want to skip permission check, you need to add android:requestLegacyExternalStorage, 
* but not recommended, I use both

## (TODO) Linux Windows Size, if need 640x480, change to USE_APP_WIDTH
* cocos2d-x-3.6/cocos/platform/desktop/CCGLViewImpl-desktop.cpp, see #if 0
```
GLViewImpl* GLViewImpl::create(const std::string& viewName)
{
    auto ret = new (std::nothrow) GLViewImpl;
#if 0 //defined(USE_APP_WIDTH) && defined(USE_APP_HEIGHT)
    if(ret && ret->initWithRect(viewName, Rect(0, 0, USE_APP_WIDTH, USE_APP_HEIGHT), 1)) {
#else      
    if(ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1)) {
    //if(ret && ret->initWithRect(viewName, Rect(0, 0, 640, 480), 1)) {
#endif    
        ret->autorelease();
        return ret;
    }

    return nullptr;
}
```

