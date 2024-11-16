# kirikiroid2lite
[WIP] kirikiroid2 lite version, without audio and most dependencies  
NOTE: Kirikiroid2lite is a mod about kirikiroid2 and cocos2d-x framework,  
it's **NOT** a fork of kirikiri2 or kirikiriz, so it may be not what your want.  

```
  予谓菊，花之隐逸者也；  
  牡丹，花之富贵者也；  
  莲，花之君子者也。  
  噫！菊之爱，陶后鲜有闻。  
  莲之爱，同予者何人？  
  牡丹之爱，宜乎众矣！  
  ——周敦颐《爱莲说》    
```

## Status  
* Working in progress.    
* Windows support: Windows 7 success.  
* Android support: with many bugs now.    

## History  
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
* Run ndk-build  
* Import, compile and run with Android ADT, see project\android\ .project   

## Minimum version of Cocos2d-x  
TODO  

## Test Data  
* https://gitee.com/weimingtom/kirikiroid2_fork/tree/master/test_data/atest3  
* see TVPGetDriverPath (???)  

## VS2013, Windows 7 development environment  
* Visual Studio 2013旗舰版  
* SDKDDKVer.h: No such file or directory  
Need to install windowsdk81.rar (Windows Kits 8.1)    

 
