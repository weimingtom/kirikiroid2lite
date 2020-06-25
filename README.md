# kirikiroid2lite
[WIP] kirikiroid2 lite version, without audio and most dependencies

## Status  
* Working in progress.    
* Windows support: Windows 7 success.  
* Android support: with many bugs now.    

## History  
* 13:23 2020-6-25: first commit  

## How to build for Windows  
* WARNING !!! don't download source zip or tar.gz file on github web page, please use git clone to download source files.  
* If you download zip or tar.gz source file, you should use notepad++ to batch replace \n with \r\n.  
* Open project\win32\cocos2d-win32.vs2013.sln with vs2013.  

## How to build for Android  
* Modify project\android\console.bat, point to NDK path.  
* Run project\android\console.bat  
* Run ndk-build  
* Import, compile and run with Android ADT, see project\android\ .project   
