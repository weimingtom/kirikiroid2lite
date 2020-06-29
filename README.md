# kirikiroid2lite
[WIP] kirikiroid2 lite version, without audio and most dependencies


  予谓菊，花之隐逸者也；  
  牡丹，花之富贵者也；  
  莲，花之君子者也。  
  噫！菊之爱，陶后鲜有闻。  
  莲之爱，同予者何人？  
  牡丹之爱，宜乎众矣！  
  ——周敦颐《爱莲说》    


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
