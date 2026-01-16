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

## NOTE
* It should not be used to play any commercial Japanese kirikiri2 game, although it's not very possible so far. It's not recommended to do this, no reason.
* It's not my effort, for any code modified by me. I just want to hack this code base.
* For me, I just want to know more about kirikiri2, just this.

## Status  
* Working in progress. No windowEx.dll support (No System.getMonitorInfo implementation), just be able to run a very simple demo data.xp3 (see top folder)      
* Windows support: (current not tested) Windows 7 success.  
* Android support: tested in MuMu emulator (emulate android x86 ?) under Win11 (Android 6.0.1), with many bugs now.    
* Linux support: only tested under Xubuntu 20.04   

## Bugs
* Only support software renderer (软件渲染器) in the global settings, not support OpenGL renderer（OpenGL渲染器，实验性）  
* Android version: enter demo game->bottom bar->right button close->no response   
* Linux version: windowEx.dll System.getMonitorInfo is not found, it makes application crashing       
see https://github.com/weimingtom/kirikiroid2lite/issues/1  
https://github.com/krkrz/krkr2/blob/master/kirikiri2/trunk/kirikiri2/src/plugins/win32/windowEx/main.cpp  
* (TODO, not checked in msys2) _title->setTitleText("/"); //FIXME: don't display empty title button //FIXME: not test / in msys2
* (TODO) no cmake support
* src/core/tjs2/tjsUtils.cpp:struct tTJSCriticalSectionImpl written with pthread is too bad, need reimplementation

## History  
* 2025-10-11 : First running macos version on old MAC MINI successfully.  
* 2025-10-10 : Building and running VisionFive2 and BPI-F3 versions successfully with OpenGL ES hardware accelerated, not softpipe, just with -Os not -O2 and -O3.  
* 2025-10-09 : First running msys2 version successfully.
* 2025-09-02 : Merge kirikiroid2-miyoo-a30 sources and compile linux version successfully.
* 2025-05-15 : Adding Android audio function and migrate to NDK r25, but Windows version compiling status is unknown.  
* 2024-12-21 : Adding Windows audio function.  
* 2020-06-25 13:23 : first commit on github, migrated from gitee to github, first available version for Android NDK and Win7 VS2013        
* 2019-03-17 : (gitee) Merge pthread project  
* 2019-02-02 : (gitee) Merge OpenCV2 project  
* 2018-08-11 : (gitee) No boost depend, use pthread instead  
* 2018-07-25 : (gitee) First available version with boost and vs2010  

## (Please Buy Steam Game if possible) Tested and Playable Supported Games
* [Unencrypted] Spring Days, Spring Days.rar    
https://code.google.com/archive/p/godxq1986/downloads    
* [Unencrypted] 第九日-the 9th day-, 试玩版 V2.0 (20160102)  
9th-trial - Exe(2016.1.12).rar       
https://the9thday.net/works/w1/download.php  

## (TODO, Please buy steam games) Not tested unencrypted or encrypted (need xp3filter.tjs) Japanese kirikiri2 or kirikiriz games  
* [Unencrypted] 恋がさくころ桜どき 体験版, 恋花绽放樱飞时 体验版, sakusaku_trial.zip, https://palette.clearrave.co.jp/product/sakusaku/download.html  
* [Unencrypted] from touchgal, 恋花绽放樱飞时.zip  
* [Encrypted] 《恋花绽放樱飞时》简体中文汉化补丁.exe  
* [Encrypted] LIKE×LOVE ～色川 鈴音～, LIKExLOVE2, LIKExLOVE2.zip, https://github.com/weimingtom/kirikiroid2lite/issues/1
* [Encrypted] リアル妹がいる大泉くんのばあい, 实妹相伴的大泉君, cheng141_with_xp3filter_complete_files.7z, cheng141_with_xp3filter_complete_files.7z, realsister_ons, (默示版, with realsister_chs.exe) [100528][ALcotハニカム]リアル妹がいる大泉くんのばあい [汉化硬盘版+RR3].rar, 日文版：2.31.2010.128, 2010/5/17, TVP(KIRIKIRI) 2 core / Scripting Platform for Win32, 中文版2010/8/24, (日文版, data.xp3 is same as 默示版) (18禁ゲーム) [100528] [ALcotハニカム] リアル妹がいる大泉くんのばあい  
* (TODO, 可能不能用体验版) 时停社, 嫁探, 不是体验版  
* [Encrypted] 恋课金普通版日文版, 我的未来是恋爱与氪金, with windowEx.dll, (TVP(KIRIKIRI) Z core / Scripting Platform for Win32, 1.3.3.7, 2019/2/21) 僕の未来は、恋と課金と。.exe, (日文版, (18禁ゲーム) [190222] [Sonora] 僕の未来は、恋と課金と。～Charge To The Future～) koikakin.7z      
* [Encrypted] 恋课金体验版: koikakinTR.zip, koikakinTRH.zip  
* [Unencrypted (Chinese patch is Encrypted exe) ] g弦上的魔王, g线上的魔王, (NOT FOUND) [あかべぇそふとつぅ]Ｇ線上の魔王, (ONLY PATCH, NOT GOOD, no data.xp3) [080529] [あかべぇそふとつぅ] G線上の魔王 中文补丁1.00＋SAVE.rar, (ONLY data.xp3) あかべぇそふとつぅ_Ｇ線上の魔王.rar (2008, TVP(KIRIKIRI) 2 core 2.29.9901.1022)  
```
http://bbs.2dgal.com/thread.php?fid=146
```    
* [Unencrypted] (已散佚, 中文版和日文版都未找到文件, 只找到一个未压缩的中文补丁版带日文原版文件) 谁杀死了知更鸟, (NOT FOUND) 誰が殺したコマドリを_zh, (NOT FOUND) [スマイル戦機]_誰が殺したコマドリを, (baiduyun) 誰が殺したコマドリを.7z with 是谁杀了知更鸟.exe and 誰が殺したコマドリをCSA版.exe (TVP(KIRIKIRI) 2 core 2.31.2011.615) and plugin/windowEx.dll, (touchgal, include patch.xp3汉化补丁, 是谁杀了知更鸟.exe is TVP(KIRIKIRI) 2 core / Scripting Platform for Win32, 2.31.2011.615, 2011/11/21) 是谁杀了知更鸟.zip, (touchgal, include 1.20简体中文化补丁 and patch.xp3, include 《是谁杀了知更鸟》1.20简体中文化补丁.rar) 【PCKR】是谁杀死了知更鸟.rar      
```
■原版说明

■お問い合わせ先
スマイル戦機ホームページ　http://smilesenki.com/
本作のお問い合わせは上記ホームページの
「サポート＆お問い合わせ」までご連絡お願いします。

■汉化版说明

■特别注意                                                           
1.预定移植。禁转ACGF（包括移植）。移植发布的时间由移植者定。
3.BUG回报请到http://www.moetea.com/forum-80-1.html或http://tieba.baidu.com/p/1962586170回报。
3.百合赛高。
```
* [Unencrypted] 恋爱0キロメートル中文版, (which version? different from Hazx version) 恋爱0公里.rar, (《恋爱0公里》Final（最终版） By Hazx) 《恋爱0公里》[本体+汉化补丁].rar, 恋爱0公里汉化补丁（除华线2h以外部分）完全汉化！.rar  
```
《恋爱0公里》Final（最终版） By Hazx
游戏版权（除补丁外）属日本游戏公司「ASa Project」所有。
请勿将游戏和补丁用于商业用途！
《恋爱0公里》资源整合：CG、音乐、歌曲、移植版、PSP版等等
http://chs.hmacg.net/game/renzero/
百度恋爱0公里贴吧：
http://tieba.baidu.com/f?kw=%C1%B5%B0%AE0%B9%AB%C0%EF
```
* [Encrypted (mobile version is unencrypted)] (rar and encrypted, with encrypted TenshinChs.exe and tenshinchs.chm, 2009/05/15, 2.31.2008.416, 2011/2/13 chs, plugin/windowEx.dll and tenshin.tpm and yuzuex.dll) 天神乱漫lucky or unlucky.mp4, (mobile version, unencrypted, small video.xp3 and voice.xp3, no exe, with TenshinChs.xp3, with patch.tjs) 天神乱漫.7z    
* [Unencrypted] (zip, 高压版, kirikiroid2版, mobile version, 天神乱漫 Happy Go Lucky!! GPT高压（逆天全年龄），注：我认为全年龄不逆天好吧, no exe, with plugin dlls) 天神乱漫 Happy Go Lucky!!  
* [Encrypted] LC, (with lovelyxcation_cn.exe) LOVELYxCATION_Ver0.9汉化补丁.rar  
```
【免责声明】

本游戏版权归原制作商所有，汉化部分版权归汉化成员共有。汉化版游戏仅供汉化研究教学之用，任何组织或个人不得以汉化版游戏用于任何形式的商业目的，对此产生的一切后果由使用方自负，汉化成员将不对此负任何责任。


组名啥的不需要
祝大家玩得愉快
2012.10.08
```
* ===NOTE, 以下仅记录===
* (TODO) [C82][CLIC_CLAC]田舎でシよう！京都編
* (TODO) [WAVESTAR] SPARKLE!!～ギリギリ家庭教師Lesson～
* (TODO) [WHEEL] ひのまるっ
* (TODO) [Wheel-Soft]大宇宙の誇り
* (TODO) lyricallisp_free-1.7.zip
* (TODO) KRKR制作范例.zip
* (TODO) A Dream of Summer.7z  
* (TODO) 東京封鎖 ～キミが隣にいた昨日～

## (TODO, Please buy steam games) steam games
* [Unencrypted] KamiYaba: Destiny on a Dicey Deadline, KamiYaba, 太依赖咒术的我未来堪忧。  
https://store.steampowered.com/app/1856480/KamiYaba_Destiny_on_a_Dicey_Deadline/  
* [Encrypted] Otome Domain, Otome * Domain, 少女 * 领域  
https://store.steampowered.com/app/2153330/Otome__Domain/  
* [Encrypted] GINKA  
https://store.steampowered.com/app/2536840/GINKA/  
* [Encrypted] Ninki Seiyuu: How to Make a Pop Voice Actress, Ninki Seiyuu, 人气声优的养成方式  
https://store.steampowered.com/app/1777750/Ninki_Seiyuu_How_to_Make_a_Pop_Voice_Actress/  
* [Encrypted] Kinkoi Golden Time, Kinkoi: Golden Time, 金辉恋曲四重奏 -Golden Time-  
https://store.steampowered.com/app/1985260/Kinkoi_Golden_Time/  
* [Unencrypted] Marco And The Galaxy Dragon Demo, 丸子与银河龙 DEMO, マルコと銀河竜 体験版, Marco & The Galaxy Dragon     
https://store.steampowered.com/app/1202540/Marco__The_Galaxy_Dragon/  
* [Encrypted] ATRI -My Dear Moments- Demo  
https://store.steampowered.com/app/1230140/ATRI_My_Dear_Moments/  
* [Encrypted?] Memory Oblivion Box, 回忆忘却之匣, mob  
https://store.steampowered.com/app/473460/Memory_Oblivion_Box/  
* [Encrypted] Saku Saku, Saku Saku: Love Blooms with the Cherry Blossoms  
https://store.steampowered.com/app/691150/Saku_Saku_Love_Blooms_with_the_Cherry_Blossoms/  

## (TODO, Please buy steam games) Not tested unencrypted or encrypted (need xp3filter.tjs) Chinese kirikiri2 or kirikiriz games  
* [Encrypted] 雾之本境twisty-destiny, twisty-destiny, (same as 默示version) 雾之本境正式版.iso, (黙示, 默示version) [110723][SP-time]雾之本境 [中文硬盘版+RR3].rar, (雾之本境DEMO) 雾之本境.zip     
```
   《雾之本境》是一款原创游戏，而非商业游戏，制作人分散在全国各地，请不要将她和其他商业游戏作品进行苛刻的比较。
    这个游戏制作时间长达一年多，其中经历了许多困难，我们没有投资，没有做商业宣传。就算这样，有的成员甚至为了制作而放弃
考试，为了团队进度而努力。经过一年的制作，无数昼夜的长期鏖战才有了今天您看到的一款小小的国产游戏。
     请尊重我们的劳动成果。
     请不要在网络中放出游戏下载，制作成盗版，以及破解改造。
     最后，谢谢您给予我们的这份支持，我们会继续努力，制作更多有趣的游戏。 
                                                                              SP-time制作组 主催  小仆         
官方博客：http://blog.sina.com.cn/sptime
官方微博：http://t.sina.com.cn/sptime
官方主页：www.sp-time.com
正式版订购地址：http://sptime.taobao.com

NOTE: Only tested with Crass-source.7z txxx=xxx
param under windows xp
and 雾之本境正式版.iso,
don't use old Crass version 0.4.10.1  
```
* (TODO) 星月夜EP3_DEMO.rar  
* [Encrypted] 虹色旋律～The Melody of Iris～demo2011.exe, 虹色旋律/demo.rar  
* ===NOTE, 以下仅记录===

## How to build for Windows  
* WARNING !!! don't download source zip or tar.gz file on github web page, please use git clone to download source files.  
* If you download zip or tar.gz source file from github web page, you should use notepad++ to batch replace \n with \r\n.  
* Open project\win32\cocos2d-win32.vs2013.sln with vs2013.  

## How to build for Android  
* Modify project\android\console.bat, point to NDK path. Recommended NDK r25 (you can also try NDK r10e)    
* Run project\android\console.bat  
```
::@set PATH=D:\android-ndk-r10e;%PATH%
@set PATH=D:\home\soft\android_studio_sdk\ndk\25.2.9519653;%PATH%
::@set PATH=D:\home\soft\android_studio_sdk\ndk\20.0.5594570;%PATH%
::@set PATH=D:\home\soft\android_studio_sdk\ndk\20.1.5948944;%PATH%
@set NDK_MODULE_PATH=%CD%\..\..\cocos2d-x-3.6\cocos;%CD%\..\..\cocos2d-x-3.6\external;%CD%\..\..\cocos2d-x-3.6
@cmd
```
* Run ndk-build, or ndk-build -j8, or ndk-build NDK_DEBUG=1 V=1.   
* Import, compile and run with Android ADT, see project\android\ .project   
* Or import, compile and run with Android Studio, see project\androidstudio     
* (FIXME) Change android permission toast to dialog, like this
```
Toast.makeText(getApplicationContext(), "You grant write external storage permission. Please restart to continue.", Toast.LENGTH_LONG).show();
```

## (TODO) How to build for Linux (Xubuntu 20.04 64bit, Xubuntu 25.04 64bit)
* (FIXME) Fix initWithRect and setDesignResolutionSize problems
* (FIXME) Two SDL windows, need to remove 'SDL2 joystick capture'
* $ sudo apt update
* $ sudo apt install libpng-dev zlib1g-dev libjpeg-dev libbz2-dev
* $ sudo apt install libsdl2-dev libglfw3-dev libglew-dev libfreetype-dev
* $ make clean
* $ make -j8
* $ make test
* $ ./project/kirikiroid2
* $ make debug

## (TODO) How to build for Linux (Ubuntu 16.04 32bit, require libglfw3-dev support)
* (FIXME) Fix initWithRect and setDesignResolutionSize problems
* (FIXME) Two SDL windows, need to remove 'SDL2 joystick capture'
* $ sudo apt update
* $ sudo apt install libpng-dev zlib1g-dev libjpeg-dev libbz2-dev
* $ sudo apt install libsdl2-dev libglfw3-dev libglew-dev libfreetype6-dev libbz2-dev
* $ make clean
* $ make -j8
* $ make test
* $ ./project/kirikiroid2
* $ make debug

## (TODO) How to build for VisionFive2 Debian RISC-V 64 bit TFCard boot, just -Os
* You need a CPU fan to make sure the CPU temperature is not over 85 degrees, see also https://www.waveshare.net/wiki/VisionFive2   
* $ sudo apt update
* $ sudo apt install libpng-dev zlib1g-dev libjpeg-dev libbz2-dev
* $ sudo apt install libsdl2-dev libglfw3-dev libglew-dev libfreetype6-dev libbz2-dev
* $ make -f Makefile.visionfive2 clean
* $ make -f Makefile.visionfive2 -j3
* $ make -f Makefile.visionfive2 test

## (TODO) How to build for BPI-F3 Bianbu 2.0 RISC-V 64 bit eMMC boot, actually almost same as VisionFive2, just -Os
* $ sudo apt update
* $ sudo apt install libpng-dev zlib1g-dev libjpeg-dev libbz2-dev
* $ sudo apt install libsdl2-dev libglfw3-dev libglew-dev libfreetype6-dev libbz2-dev
* $ make -f Makefile.bpif3 clean
* $ make -f Makefile.bpif3 -j3
* $ make -f Makefile.bpif3 test

## (TODO) How to build for macOS old MAC MINI Intel version
* You need to install Xcode and Xcode Command Line Tools (I use macos big sur 11 + xcode 11.7 + Command Line Tools 12)
* $ make -f Makefile.macos clean
* $ make -f Makefile.macos -j8
* $ make -f Makefile.macos test

## (TODO) How to build for msys2, on Windows 11 64bit
* Make sure msys2 dev packages headers are installed to c:/msys64/mingw64/include, otherwise need to modify Makefile.msys2
* Launch MSYS2 MSYS shell
* $ pacman -Sy
* $ pacman -S mingw-w64-x86_64-libpng
* $ pacman -S mingw-w64-x86_64-libjpeg
* $ pacman -S mingw-w64-x86_64-libwebp
* $ pacman -S mingw-w64-x86_64-zlib
* $ pacman -S mingw-w64-x86_64-glew
* $ pacman -S mingw-w64-x86_64-glfw
* $ pacman -S mingw-w64-x86_64-gdb
* $ mingw32-make.exe -f Makefile.msys2 clean
* $ mingw32-make.exe -f Makefile.msys2 -j8
* $ mingw32-make.exe -f Makefile.msys2 test

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

## macos
* if !defined( _MSC_VER) && !defined(LINUX)  
Widget *cell = dynamic_cast<Widget*>(reader.Load("ui/ListItem.csb"));  
* include <freetype/freetype.h>, make two folders freetype and freetype2 in external_macos
* -I./src/core/external/freetype-2.5.0.1/include, not exists
* adding -Iexternal_macos/include/GLFW
* remove if !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX) && !defined(__APPLE__)
* remove defined(ANDROID) || defined(LINUX) || defined(__APPLE__)
* remove !defined(_MSC_VER) && !defined(ANDROID) && !defined(LINUX) && !defined(__APPLE__)
* 1 //defined(__MACOS__)
* cannot be captured because it does not have automatic storage duration
* ./src/core/environ/cocos2d/CustomFileUtils.cpp
* rw |= O_BINARY;
* duplicate symbol 'DeinterleaveApplyingWindow(float**, float const*, float*, int, unsigned long, unsigned long)' in:
*    ./src/core/utils/MathAlgorithms_Default.o
*    ./src/core/sound/MathAlgorithms.o
* duplicate symbol 'InterleaveOverlappingWindow(float*, float const* const*, float*, int, unsigned long, unsigned long)' in:
*    ./src/core/utils/MathAlgorithms_Default.o
*    ./src/core/sound/MathAlgorithms.o
* readlink("/proc/self/exe", this is useless under macos
* check other places: //if (length <= 0) {
* += appPath.substr, need check pos param
* include <OpenGLES/ES2/gl.h>
* if(ret && ret->initWithRect(viewName, Rect(0, 0, 640, 480), 1)) {
* m.MemTotal = 4000 * 1000 * 1024 / 1024;
* include <jerror.h>
* TVPGetMemoryInfo
```
现在我终于可以编译运行macos版的kirikiroid2lite（旧mac mini），
虽然似乎有一些问题，例如我把窗口弄小了，还有游戏屏幕有时候会定住不动，
但其实还能响应鼠标事件的，可能显示有问题。应该基本上大部分计划都完成了，
已经可以新移植到macos和vf2和bpi-f3，这三种新的硬件上，
目标只有ios版还没移植成功（大概要下一年）
```

## TODO, msys2  
* (TODO) Compare to kirikiroid2_fork3   
* (TODO) Check kirikiroid2_fork3 sound impl, src/core/sound/win32/WaveMixer.cpp
* static_assert(sizeof(t.st_size) == 8, "");
* TVPAddAutoSearchArchive(skinpath);
* getcwd(buf, sizeof(buf) / sizeof(buf[0]));
* av_dirname, FIXME: return std::string((char*)FileName.c_str()); not dirname

## TODO
* _spriteFramesAliases.reserve(20);
* define CC_USE_PHYSICS 0 //1
* _data.reserve(capacity);
* std::vector<Node*> findChildren(const Node &node, const std::string &name)
* std::vector<Node*> CC_DLL findChildren(const Node &node, const std::string &name);
* if(ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1)) {
* if CC_USE_TIFF
* if CC_USE_WEBP
* if defined(CC_USE_WIC)
* elif defined(CC_USE_TIFF)
* include "physics/CCPhysicsBody.h", add macro check
* //for EGL/egl.h, and EGL/eglplatform.h, undef Bool
* FIXME: I don't know why crash in cocos2d-x 3.6
* selectedEvent();

## TODO, visionfive2
* (TODO) build with -g0 -O3

## FIXME:
* some cocosstudio files go to here, but not merged    
https://github.com/weimingtom/kirikiroid2-miyoo-a30/tree/master/cocosstudio/kr2  
* android build failed: CallFunc::create([&ui_] {  

## gles and glfw3 test code
* testgles_20241128.tar.gz
* kirikiroid2lite_gles_failed_001.tar.gz and glfw-3.4_gles_failed_001.tar.gz
* https://github.com/nickdesaulniers/opengles2-book
* https://github.com/tsukumijima/libmali-rockchip
* https://github.com/radxa-pkg/rockchip-prebuilt
* https://github.com/lihuang1111/wayland-glesv2-demo

## Planned supported devices  
* Steam Deck LCD version, with GPU driver AMD Custom GPU 0405 (radeonsi, vangogh, LLVM 19.1.7, DRM 3.61, 6.14.0-15-generic) by AMD  
ubuntu-25.04-desktop-amd64.iso, LiveCD mode  
* Intel(R) Core(TM) i3-10100 CPU @ 3.60GHz, GPU driver NVIDIA GeForce RTX 2080/PCIe/SSE2 by NVIDIA Corporation  
ubuntu-25.04-desktop-amd64.iso    
xubuntu-25.04-desktop-amd64.iso  
xubuntu-20.04-desktop-amd64.iso  
* GIGABYTE GB-BACE-3150，N3150, GPU driver Intel HD Graphics 400 by Intel  
ubuntu-25.04-desktop-amd64.iso    
xubuntu-25.04-desktop-amd64.iso  
xubuntu-20.04-desktop-amd64.iso  
* Vision Five2, with GPU GLESv2 driver PowerVR B-Series BXE-4-32 by Imagination Technologies and software GPU driver softpipe (TOO SLOW 1 second per frame)    
starfive-jh7110-202409-SD-minimal-desktop-wayland.img.bz2  
* BPI-F3, need eMMC boot (don't use TF Card boot, bacause it will fail when second boot), with GPU GLESv2 driver PowerVR (PVR) B-Series BXE-2-32 by Imagination Technologies  
titantools_for_windows-2.0.7-Rc.exe  
bianbu-24.04-desktop-k1-v2.0-release-20241021195251.zip   
* BPI-F3, use Armbian noble   
(???) 2024-08-12-Armbian-bpi-SpacemiT_24.5.0-trunk_Bananapif3_noble_legacy_6.1.15_xfce_desktop.img.xz  
* LCPI-H3, a.k.a Orangepi PC Plus, armv7l, with GPU driver Mali400 by Lima, the temperature should not be too high and over 100 degrees     
LCPI-H3_2.2.2_ubuntu_focal_desktop_linux5.4.65.img    
* Radxa Dragon Q6A, QCS6490, aarch64, with GPU driver FD643 by notfreedreno  
radxa-dragon-q6a_noble_kde_t5.output_512.img.xz  
* Orangepi 5, aarch64, official OS with llvmpipe and libmali GLESv2     
Orangepi5_1.2.2_ubuntu_jammy_desktop_xfce_linux5.10.160.7z  
* Orangepi 5, aarch64, ubuntu-rockchip with Panfrost  
ubuntu-24.04-preinstalled-desktop-arm64-orangepi-5.img.xz  
(Joshua-Riek/ubuntu-rockchip, don't use 24.10 because it's not LTS apt update may fail)   
* Orangepi 5, aarch64, Armbian with Panfrost     
Armbian_25.11.1_Orangepi5_noble_current_6.12.58_xfce_desktop.img.xz  
* (TODO) RPI3, Raspberry Pi 3B and CM3    
2020-05-27-raspios-buster-armhf.img  
* (TODO) RPI4, Raspberry Pi 4B and CM4  
2023-05-03-raspios-bullseye-arm64.img   
2023-05-03-raspios-bullseye-armhf.img  
* (TODO) OGA 1.1, ODROID-GO Advance 1.1 black version     
ubuntu-18.04.3-4.4-es-odroid-goA-20200923.img  
