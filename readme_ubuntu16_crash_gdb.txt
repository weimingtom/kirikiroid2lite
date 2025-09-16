gdb, x/hs 0x000xxx

====
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
=========================>TVPForceSwapBuffer() not imp
11:26:53 SaveWindow.ks : 
11:26:53 SaveWindow.ks : [macro name=setsavethum]
11:26:53 SaveWindow.ks : [eval exp="kag.fore.base.imageStorage=mp.storage"]
11:26:53 SaveWindow.ks : [endmacro]
11:26:53 macro : setsavethum : [eval exp="kag.fore.base.imageStorage=mp.storage"]
11:26:53 SaveWindow.ks : 
11:26:53 SaveWindow.ks : [return]
11:26:53 ================================================================================
11:26:53 Scenario loaded : first.ks
11:26:53 first.ks : returned to : (start) line offset 51
11:26:53 first.ks : call stack depth after returning : 0
11:26:53 first.ks : [call storage="ConfigWindow.ks"]
11:26:53 first.ks : call stack depth before calling : 0
11:26:53 ================================================================================
11:26:53 Scenario loaded : ConfigWindow.ks
11:26:53 ConfigWindow.tjs を読み込みました(23ms)
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasc.ksd
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasu.ksd
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
=========================>TVPForceSwapBuffer() not imp
11:26:53 ==== An exception occured at configwindow.tjs(433)[(function) updateScreenMode], VM ip = 17 ====
11:26:53 -- Disassembled VM code --
<<<<<<<2.2 w == 0
<<<<<<<3 w == 0
<<<<<<<pl == 0, p == 0, sum == 0
<<<<<<<2.2 w == 0
<<<<<<<3 w == 9
<<<<<<<pl == 0, p == 9, sum == 9
<<<<<<<2.2 w == 0
<<<<<<<3 w == 0
<<<<<<<pl == 0, p == 0, sum == 0
11:26:53 #(145155870) 
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<<================= 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 6
<<<<<<<pl == 0, p == 6, sum == 6
<<<<<<<2.2 w == 8
<<<<<<<3 w == 8
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<<================= 7
<<<<<<<2.2 w == 0
Makefile:1061: recipe for target 'test' failed
make: *** [test] Segmentation fault (core dumped)
wmt@wmt-virtual-machine:~/kirikiroid2lite$ make debug
gdb ./project/kirikiroid2
GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.5) 7.11.1
Copyright (C) 2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./project/kirikiroid2...done.
(gdb) r
Starting program: /home/wmt/kirikiroid2lite/project/kirikiroid2 
warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/ld-2.23.so" does not match "/lib/ld-linux.so.2" (CRC mismatch).

[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/i386-linux-gnu/libthread_db.so.1".
warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/libm-2.23.so" does not match "/lib/i386-linux-gnu/libm.so.6" (CRC mismatch).

warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/libc-2.23.so" does not match "/lib/i386-linux-gnu/libc.so.6" (CRC mismatch).

warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/libdl-2.23.so" does not match "/lib/i386-linux-gnu/libdl.so.2" (CRC mismatch).

warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/librt-2.23.so" does not match "/lib/i386-linux-gnu/librt.so.1" (CRC mismatch).

warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/libnsl-2.23.so" does not match "/lib/i386-linux-gnu/libnsl.so.1" (CRC mismatch).

warning: the debug information found in "/usr/lib/debug//lib/i386-linux-gnu/libresolv-2.23.so" does not match "/lib/i386-linux-gnu/libresolv.so.2" (CRC mismatch).

applicationDidFinishLaunching
*** *** applicationDidFinishLaunching 001
[New Thread 0xa877eb40 (LWP 5935)]
INFO: There are 0 game controller(s) attached (0 joystick(s))
ERROR: 0 is an invalid joystick index.

{
	gl.supports_vertex_array_object: true
	gl.supports_BGRA8888: false
	gl.supports_ATITC: false
	gl.supports_S3TC: true
	gl.supports_PVRTC: false
	cocos2d.x.build_type: DEBUG
	gl.supports_discard_framebuffer: false
	cocos2d.x.compiled_with_profiler: false
	cocos2d.x.version: cocos2d-x 3.6
	gl.renderer: SVGA3D; build: RELEASE;  LLVM;
	gl.supports_ETC1: false
	cocos2d.x.compiled_with_gl_state_cache: true
	gl.version: 3.0 Mesa 18.0.5
	gl.supports_NPOT: true
	gl.max_texture_units: 48
	gl.vendor: VMware, Inc.
	gl.max_texture_size: 16384
}


<<<<setProjection(Projection::_3D)
*** *** applicationDidFinishLaunching 002
<<<<setProjection(Projection::_3D)
*** *** applicationDidFinishLaunching 003
*** *** applicationDidFinishLaunching 004
*** *** applicationDidFinishLaunching 004-1
*** *** applicationDidFinishLaunching 004-2
*** *** applicationDidFinishLaunching 004-3
*** *** applicationDidFinishLaunching 004-4
*** *** applicationDidFinishLaunching 004-5
*** *** applicationDidFinishLaunching 004-6
*** *** applicationDidFinishLaunching 005
appPath: /home/wmt/kirikiroid2lite
suffix = csb
textureSize = 0
classname = Node
size = 3
classname = ListView
callBackName  cannot be found
size = 0
child = 0x962bcb8
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9635388
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9635bb0
suffix = csb
textureSize = 0
classname = Node
size = 4
classname = Panel
callBackName  cannot be found
size = 0
child = 0x95e9c20
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9636828
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9655570
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96651b8
suffix = csb
textureSize = 0
classname = Node
size = 1
classname = Panel
callBackName  cannot be found
size = 0
child = 0x966ff00
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = ScrollView
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 1
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9678f18
child = 0x9678450
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 0
child = 0x968c688
child = 0x9685520
child = 0x967ccb8
child = 0x9671220
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9690fd0
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9693c80
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9697a58
classname = Panel
callBackName  cannot be found
size = 0
child = 0x969ae70
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = ScrollView
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 1
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96a2800
child = 0x96a1d38
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 0
child = 0x96ae008
child = 0x96a9ba8
child = 0x96a5550
child = 0x96726c0
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96b28e8
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x966f420
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96b8a18
classname = Panel
callBackName  cannot be found
size = 0
child = 0x96bb670
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = ScrollView
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 1
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96c4238
child = 0x96c3770
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 1
classname = Text
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
callBackName  cannot be found
size = 0
child = 0x96cfc98
child = 0x96cb770
child = 0x96c6f88
child = 0x96bc7c8
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96d44d8
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96d7188
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x96d9e18
classname = Panel
callBackName  cannot be found
size = 0
child = 0x96dca70
<<<<CCDevice-linux.cpp FT_New_Face : /home/wmt/kirikiroid2lite/project/FreeSerif.ttf
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x96ddf28
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9722b38
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9723508
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x972d488
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9731518
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9731f00
child = 0x9730f98
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x9736558
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9739bb8
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x973a600
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x9743d68
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9745dc0
classname = Panel
callBackName  cannot be found
size = 0
child = 0x97467c8
child = 0x9745818
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x97471b0
classname = Panel
callBackName  cannot be found
size = 0
child = 0x974a9b0
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x974b380
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x9754ae8
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9756b40
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9757548
child = 0x9756598
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x96b54c8
classname = Panel
callBackName  cannot be found
size = 0
child = 0x975c728
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x975d0f8
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x9766860
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x96be9f0
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9769b40
child = 0x9768310
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x976a4b0
classname = Panel
callBackName  cannot be found
size = 0
child = 0x976dcb0
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x976e680
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x9777de8
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9779e40
classname = Panel
callBackName  cannot be found
size = 0
child = 0x977a848
child = 0x9779898
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x977b478
classname = Panel
callBackName  cannot be found
size = 0
child = 0x977ec20
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x977f600
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x9788d68
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x978adc0
classname = Panel
callBackName  cannot be found
size = 0
child = 0x978b7c8
child = 0x978a818
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x977cc08
classname = Panel
callBackName  cannot be found
size = 0
child = 0x9790038
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x9790a08
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x979a170
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x979c1c8
classname = Panel
callBackName  cannot be found
size = 0
child = 0x979cbd0
child = 0x979bc20
suffix = csb
textureSize = 0
classname = Node
size = 5
classname = Text
callBackName  cannot be found
size = 0
child = 0x979d800
classname = Panel
callBackName  cannot be found
size = 0
child = 0x97a0fc8
classname = Button
<<< ButtonReader: button->setTitleText == 
callBackName  cannot be found
size = 0
child = 0x97a1a70
classname = CheckBox
callBackName  cannot be found
size = 0
child = 0x97ab1c8
classname = Panel
callBackName  cannot be found
size = 2
classname = Panel
callBackName  cannot be found
size = 0
child = 0x97ad220
classname = Panel
callBackName  cannot be found
size = 0
child = 0x97adc28
child = 0x97acc78
===================65538, 1, 0
Inner height <= scrollview height, it will be force sized!
<<< data.xp3 not found: /home/wmt/kirikiroid2lite/project/data.xp3
=========================>TVPCheckStartupPath()
===================65538, 0, 0
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 40
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
11:27:40 (info) Data path : file://./home/wmt/likexlove2/savedata/
11:27:40 Program started on Linux (Linux)
[New Thread 0xa70ecb40 (LWP 5937)]
11:27:40 (info) Rebuilding Auto Path Table ...
11:27:40 (info) Total 0 file(s) found, 0 file(s) activated. (0ms)
[New Thread 0xa66ffb40 (LWP 5938)]
11:27:40 (info) Total physical memory : 3698417664
11:27:40 (info) Specified option(s) (earlier item has more priority) : (none)
=====================TVPGL_ASM_Init not imp
11:27:40 (info) Trying to read XP3 virtual file system information from : file://./home/wmt/likexlove2/data.xp3
11:27:40 (info) Done. (contains 6503 file(s), 6503 segment(s))
11:27:40 (info) Loading startup script : file://./home/wmt/likexlove2/data.xp3>startup.tjs
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
=========================>TVPForceSwapBuffer() not imp
11:27:40 (info) Rebuilding Auto Path Table ...
11:27:40 (info) Total 6479 file(s) found, 6479 file(s) activated. (11ms)
11:27:40 OS : Linux (Linux)
11:27:40 KAG : 3.32 stable rev. 2
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
11:27:40 Kirikiri : 2.2.2.2
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 41
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
11:27:41 Config.tjs を読み込みました(14ms)
11:27:41 UpdateConfig.tjs を読み込みました(3ms)
11:27:41 KAG System スクリプトを読み込んでいます...
11:27:41 Utils.tjs を読み込みました(3ms)
11:27:41 KAGLayer.tjs を読み込みました(10ms)
11:27:41 HistoryLayer.tjs を読み込みました(18ms)
11:27:41 BGM.tjs を読み込みました(9ms)
11:27:41 SE.tjs を読み込みました(5ms)
11:27:41 Movie.tjs を読み込みました(6ms)
11:27:41 Conductor.tjs を読み込みました(9ms)
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
=========================>TVPForceSwapBuffer() not imp
11:27:41 AnimationLayer.tjs を読み込みました(22ms)
11:27:41 GraphicLayer.tjs を読み込みました(12ms)
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
=========================>TVPForceSwapBuffer() not imp
11:27:41 MessageLayer_ori.tjs を読み込みました(32ms)
11:27:41 MessageLayer.tjs を読み込みました(85ms)
11:27:41 Menus.tjs を読み込みました(2ms)
11:27:41 DefaultMover.tjs を読み込みました(6ms)
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
=========================>TVPForceSwapBuffer() not imp
11:27:41 MainWindow.tjs を読み込みました(103ms)
11:27:41 スクリプトの読み込みに 322ms かかりました
[New Thread 0xa5cf6b40 (LWP 5939)]
====================> _createStaticTexture2D not set 
11:27:41 (info) Use malloc for Bitmap
11:27:41 Detected CPU core(s): 1
[New Thread 0xa3de1b40 (LWP 5940)]
[New Thread 0xa33ffb40 (LWP 5941)]
[New Thread 0xa29ffb40 (LWP 5942)]
[New Thread 0xa1fffb40 (LWP 5943)]
[New Thread 0xa15ffb40 (LWP 5944)]
[New Thread 0xa0bfeb40 (LWP 5945)]
[New Thread 0xa01ffb40 (LWP 5946)]
[New Thread 0x9f7ffb40 (LWP 5947)]
[New Thread 0x9edffb40 (LWP 5948)]
[New Thread 0x9e3ffb40 (LWP 5949)]
[New Thread 0x9d9ffb40 (LWP 5950)]
[New Thread 0x9cfffb40 (LWP 5951)]
[New Thread 0x9c5ffb40 (LWP 5952)]
[New Thread 0x9bdfeb40 (LWP 5953)]
[New Thread 0x9b5fdb40 (LWP 5954)]
[New Thread 0x9adfcb40 (LWP 5955)]
[New Thread 0x9a5fbb40 (LWP 5956)]
[New Thread 0x99dfab40 (LWP 5957)]
[New Thread 0x995f9b40 (LWP 5958)]
[New Thread 0x98df8b40 (LWP 5959)]
[New Thread 0x985f7b40 (LWP 5960)]
[New Thread 0x97df6b40 (LWP 5961)]
[New Thread 0x975f5b40 (LWP 5962)]
[New Thread 0x96df4b40 (LWP 5963)]
[New Thread 0x965f3b40 (LWP 5964)]
[New Thread 0x95df2b40 (LWP 5965)]
[New Thread 0x955f1b40 (LWP 5966)]
[New Thread 0x94df0b40 (LWP 5967)]
[New Thread 0x945efb40 (LWP 5968)]
[New Thread 0x93deeb40 (LWP 5969)]
[New Thread 0x935edb40 (LWP 5970)]
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasc.ksd
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasu.ksd
11:27:41 KAGMainWindow のコンストラクタで 91ms かかりました
11:27:41 ================================================================================
11:27:41 Scenario loaded : first.ks
11:27:41 処理を開始します
11:27:41 (info) Startup script ended.
11:27:41 rootnuko2019 System Version : 1.00 (release.2020-08-28)
11:27:41 first.ks : 
11:27:41 first.ks : 
11:27:41 first.ks : [current layer=message0 page=fore]
11:27:41 first.ks : [font size=12 face=MPLUS1C12][mappfont storage=MPLUS1C12.tft]
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:41 first.ks : [font size=14 face=MPLUS1C14][mappfont storage=MPLUS1C14.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 42
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:42 first.ks : [font size=16 face=MPLUS1C16][mappfont storage=MPLUS1C16.tft]
=========================>TVPForceSwapBuffer() not imp
11:27:42 first.ks : [font size=18 face=MPLUS1C18][mappfont storage=MPLUS1C18.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 43
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:43 first.ks : [font size=20 face=MPLUS1C20][mappfont storage=MPLUS1C20.tft]
=========================>TVPForceSwapBuffer() not imp
11:27:43 first.ks : [font size=22 face=MPLUS1C22][mappfont storage=MPLUS1C22.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 44
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
=========================>TVPForceSwapBuffer() not imp
11:27:44 first.ks : [font size=24 face=MPLUS1C24][mappfont storage=MPLUS1C24.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 45
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:45 first.ks : [font size=26 face=MPLUS1C26][mappfont storage=MPLUS1C26.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 46
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:46 first.ks : [font size=28 face=MPLUS1C28][mappfont storage=MPLUS1C28.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 47
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:47 first.ks : [font size=30 face=MPLUS1C30][mappfont storage=MPLUS1C30.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 48
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
=========================>TVPForceSwapBuffer() not imp
11:27:48 first.ks : [font size=32 face=MPLUS1C32][mappfont storage=MPLUS1C32.tft]
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 49
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
=========================>TVPForceSwapBuffer() not imp
11:27:49 first.ks : 
11:27:49 first.ks : 
11:27:49 first.ks : [call storage="Defines.ks"]
11:27:49 first.ks : call stack depth before calling : 0
11:27:49 ================================================================================
11:27:49 Scenario loaded : Defines.ks
11:27:49 Defines.ks : 
11:27:49 Defines.ks : [return]
11:27:49 ================================================================================
11:27:49 Scenario loaded : first.ks
11:27:49 first.ks : returned to : (start) line offset 44
11:27:49 first.ks : call stack depth after returning : 0
11:27:49 first.ks : [call storage="Volume.ks"]
11:27:49 first.ks : call stack depth before calling : 0
11:27:49 ================================================================================
11:27:49 Scenario loaded : Volume.ks
11:27:49 Volume.tjs を読み込みました(8ms)
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
[New Thread 0x8908ab40 (LWP 5971)]
INFO: Audio Device: pulseaudio
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
[New Thread 0x88889b40 (LWP 5972)]
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPInitDirectSound not imp, implemented 20241220 sound good
==================>TVPCreateSoundBuffer not imp, implemented 20241220 sound good
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
=========================>TVPForceSwapBuffer() not imp
11:27:49 Volume.ks : 
11:27:49 Volume.ks : [return]
11:27:49 ================================================================================
11:27:49 Scenario loaded : first.ks
11:27:49 first.ks : returned to : (start) line offset 45
11:27:49 first.ks : call stack depth after returning : 0
11:27:49 first.ks : [call storage="AskDialog.ks"]
11:27:49 first.ks : call stack depth before calling : 0
11:27:49 ================================================================================
11:27:49 Scenario loaded : AskDialog.ks
11:27:49 AskDialog.tjs を読み込みました(12ms)
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
=========================>TVPForceSwapBuffer() not imp
11:27:49 AskDialog.ks : 
11:27:49 AskDialog.ks : [macro name=dialog]
11:27:49 AskDialog.ks : [eval exp="kag.diagAsk.storage = mp.storage"]
11:27:49 AskDialog.ks : [eval exp="kag.diagAsk.targetYes = mp.targetyes"]
11:27:49 AskDialog.ks : [eval exp="kag.diagAsk.targetNo = mp.targetno"]
11:27:49 AskDialog.ks : [eval exp="kag.diagAsk.targetCancel = mp.targetcancel"]
11:27:49 AskDialog.ks : [eval exp="kag.diagAsk.show(%[callback : function(result) { kag.process(storage, result == global.DIALOG_RESULT.YES ? targetYes : (result == global.DIALOG_RESULT.NO ? targetNo : targetCancel)); } incontextof kag.diagAsk, message0 : mp.message0, message1: mp.message1]);"]
11:27:49 AskDialog.ks : [endmacro]
11:27:49 macro : dialog : [eval exp="kag.diagAsk.storage = mp.storage"][eval exp="kag.diagAsk.targetYes = mp.targetyes"][eval exp="kag.diagAsk.targetNo = mp.targetno"][eval exp="kag.diagAsk.targetCancel = mp.targetcancel"][eval exp="kag.diagAsk.show(%[callback : function(result) { kag.process(storage, result == global.DIALOG_RESULT.YES ? targetYes : (result == global.DIALOG_RESULT.NO ? targetNo : targetCancel)); } incontextof kag.diagAsk, message0 : mp.message0, message1: mp.message1]);"]
11:27:49 AskDialog.ks : 
11:27:49 AskDialog.ks : [return]
11:27:49 ================================================================================
11:27:49 Scenario loaded : first.ks
11:27:49 first.ks : returned to : (start) line offset 46
11:27:49 first.ks : call stack depth after returning : 0
11:27:49 first.ks : [call storage="ChoiseDialog.ks"]
11:27:49 first.ks : call stack depth before calling : 0
11:27:49 ================================================================================
11:27:49 Scenario loaded : ChoiseDialog.ks
11:27:49 ChoiseDialog.tjs を読み込みました(9ms)
11:27:49 Plugin.tjs を読み込みました(2ms)
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 50
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
11:27:50 ChoiseDialog.ks : 
11:27:50 ChoiseDialog.ks : [macro name=blink]
11:27:50 ChoiseDialog.ks : [eval exp="kag.diagChoice.addButtons(mp.left, mp.top, mp.image, mp.x, mp.y, mp.width, mp.height, mp.text, mp.storage, mp.target, mp.enabled, mp.id, mp.blinkcount, mp.hidetime, mp.seoncursor, mp.sechoice, mp.deflt)"]
11:27:50 ChoiseDialog.ks : [endmacro]
11:27:50 macro : blink : [eval exp="kag.diagChoice.addButtons(mp.left, mp.top, mp.image, mp.x, mp.y, mp.width, mp.height, mp.text, mp.storage, mp.target, mp.enabled, mp.id, mp.blinkcount, mp.hidetime, mp.seoncursor, mp.sechoice, mp.deflt)"]
11:27:50 ChoiseDialog.ks : 
11:27:50 ChoiseDialog.ks : [macro name=blinkshow]
11:27:50 ChoiseDialog.ks : [eval exp="kag.diagChoice.show(mp.showtime, mp.updatehistory, mp.autoselect, mp.parentbase)"]
11:27:50 ChoiseDialog.ks : [endmacro]
11:27:50 macro : blinkshow : [eval exp="kag.diagChoice.show(mp.showtime, mp.updatehistory, mp.autoselect, mp.parentbase)"]
11:27:50 ChoiseDialog.ks : 
11:27:50 ChoiseDialog.ks : [return]
11:27:50 ================================================================================
11:27:50 Scenario loaded : first.ks
11:27:50 first.ks : returned to : (start) line offset 47
11:27:50 first.ks : call stack depth after returning : 0
11:27:50 first.ks : [call storage="BackGroundVoice.ks"]
11:27:50 first.ks : call stack depth before calling : 0
11:27:50 ================================================================================
11:27:50 Scenario loaded : BackGroundVoice.ks
11:27:50 BackGroundVoice.tjs を読み込みました(2ms)
11:27:50 BackGroundVoice.ks : 
11:27:50 BackGroundVoice.ks : [macro name=bgv]
11:27:50 BackGroundVoice.ks : [eval exp="kag.bgv.play(mp.storage, mp.loop, mp.escape, mp.fadetime, mp.chara);"]
11:27:50 BackGroundVoice.ks : [endmacro]
11:27:50 macro : bgv : [eval exp="kag.bgv.play(mp.storage, mp.loop, mp.escape, mp.fadetime, mp.chara);"]
11:27:50 BackGroundVoice.ks : 
11:27:50 BackGroundVoice.ks : [macro name=stopbgv]
11:27:50 BackGroundVoice.ks : [eval exp="kag.bgv.stop(mp.fadetime);"]
11:27:50 BackGroundVoice.ks : [endmacro]
11:27:50 macro : stopbgv : [eval exp="kag.bgv.stop(mp.fadetime);"]
11:27:50 BackGroundVoice.ks : 
11:27:50 BackGroundVoice.ks : [return]
11:27:50 ================================================================================
11:27:50 Scenario loaded : first.ks
11:27:50 first.ks : returned to : (start) line offset 48
11:27:50 first.ks : call stack depth after returning : 0
11:27:50 first.ks : [call storage="MessageWindowController.ks"]
11:27:50 first.ks : call stack depth before calling : 0
11:27:50 ================================================================================
11:27:50 Scenario loaded : MessageWindowController.ks
11:27:50 MessageWindowController.tjs を読み込みました(21ms)
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [137] to [184].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [184] to [246].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [137] to [184].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [184] to [246].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
=========================>TVPForceSwapBuffer() not imp
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [laycount messages=1]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [position layer=message0 page=fore frame="" left=0 top=440 width=1280 height=280 marginl=210 margint=125 marginr=200 marginb=25]
11:27:50 MessageWindowController.ks : [position layer=message0 page=back frame="" left=0 top=440 width=1280 height=280 marginl=210 margint=125 marginr=200 marginb=25]
11:27:50 MessageWindowController.ks : [layopt layer=message0 page=fore visible=false]
11:27:50 MessageWindowController.ks : [layopt layer=message0 page=back visible=false]
11:27:50 MessageWindowController.ks : [current layer=message0 page=back][deffont size=26 face=MPLUS1C26 color=0xFFFFFF edge=true edgecolor=0x000000 shadow=true shadowcolor=0x000000][resetfont][defstyle linespacing=14][resetstyle]
11:27:50 MessageWindowController.ks : [current layer=message0 page=fore][deffont size=26 face=MPLUS1C26 color=0xFFFFFF edge=true edgecolor=0x000000 shadow=true shadowcolor=0x000000][resetfont][defstyle linespacing=14][resetstyle]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [current layer=message0]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [macro name=msgwin]
11:27:50 MessageWindowController.ks : [if exp="mp.page == 'no'"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.hide()"]
11:27:50 MessageWindowController.ks : [else]
11:27:50 MessageWindowController.ks : [layopt layer=message page=fore visible=false]
11:27:50 MessageWindowController.ks : [layopt layer=message page=back visible=false]
11:27:50 MessageWindowController.ks : [current layer=&"'message'+mp.page"]
11:27:50 MessageWindowController.ks : [if exp="mp.page == 0 || mp.page === void"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.show()"]
11:27:50 MessageWindowController.ks : [else]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.hide()"]
11:27:50 MessageWindowController.ks : [layopt layer=message page=fore visible=true]
11:27:50 MessageWindowController.ks : [layopt layer=message page=back visible=true]
11:27:50 MessageWindowController.ks : [endif]
11:27:50 MessageWindowController.ks : [endif]
11:27:50 MessageWindowController.ks : [endmacro]
11:27:50 macro : msgwin : [if exp="mp.page == 'no'"][eval exp="kag.msgWin.hide()"][else][layopt layer=message page=fore visible=false][layopt layer=message page=back visible=false][current layer=&"'message'+mp.page"][if exp="mp.page == 0 || mp.page === void"][eval exp="kag.msgWin.show()"][else][eval exp="kag.msgWin.hide()"][layopt layer=message page=fore visible=true][layopt layer=message page=back visible=true][endif][endif]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [macro name=msgname]
11:27:50 MessageWindowController.ks : [if exp="global.NAME_LIST_KEYS.find(mp.name !== void ? mp.name.toUpperCase() : '') >= 0 || mp.name === void"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.setName(mp)"]
11:27:50 MessageWindowController.ks : [else]
11:27:50 MessageWindowController.ks : [eval exp="System.inform('[msgname]タグエラー：\n未定義のネームIDです。\n\nname=' + mp.name)"]
11:27:50 MessageWindowController.ks : [endif]
11:27:50 MessageWindowController.ks : [label]
11:27:50 MessageWindowController.ks : [endmacro]
11:27:50 macro : msgname : [if exp="global.NAME_LIST_KEYS.find(mp.name !== void ? mp.name.toUpperCase() : '') >= 0 || mp.name === void"][eval exp="kag.msgWin.setName(mp)"][else][eval exp="System.inform('[msgname]タグエラー：\n未定義のネームIDです。\n\nname=' + mp.name)"][endif][label]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [macro name=cv]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.setCVVolume(mp.cvname !== void ? mp.cvname : mp.name)"]
11:27:50 MessageWindowController.ks : [playse buf=&global.SOUND_BUFFER.VOICE storage=&cvstr(mp) cond="kag.skipMode == 0 || kag.forceEffect"]
11:27:50 MessageWindowController.ks : [eval exp="kag.bgv.autopause(kag.se[global.SOUND_BUFFER.VOICE].totalTime)" cond="kag.se[global.SOUND_BUFFER.VOICE].status == 'play'"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.repeatVoice = cvstr(mp)"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.repeatVoiceID = mp.cvname !== void ? mp.cvname : mp.name"]
11:27:50 MessageWindowController.ks : [if exp="global.NAME_LIST_KEYS.find(mp.name !== void ? mp.name.toUpperCase() : '') >= 0 || mp.name === void"]
11:27:50 MessageWindowController.ks : [eval exp="kag.msgWin.setName(%[name : mp.name, action : cvstr(mp), cvname : mp.cvname, serifColor : mp.serifcolor])"]
11:27:50 MessageWindowController.ks : [else]
11:27:50 MessageWindowController.ks : [eval exp="System.inform('[cv]タグエラー：\n未定義のネームIDです。\n\nname=' + mp.name)"]
11:27:50 MessageWindowController.ks : [endif]
11:27:50 MessageWindowController.ks : [endmacro]
11:27:50 macro : cv : [eval exp="kag.msgWin.setCVVolume(mp.cvname !== void ? mp.cvname : mp.name)"][playse buf=&global.SOUND_BUFFER.VOICE storage=&cvstr(mp) cond="kag.skipMode == 0 || kag.forceEffect"][eval exp="kag.bgv.autopause(kag.se[global.SOUND_BUFFER.VOICE].totalTime)" cond="kag.se[global.SOUND_BUFFER.VOICE].status == 'play'"][eval exp="kag.msgWin.repeatVoice = cvstr(mp)"][eval exp="kag.msgWin.repeatVoiceID = mp.cvname !== void ? mp.cvname : mp.name"][if exp="global.NAME_LIST_KEYS.find(mp.name !== void ? mp.name.toUpperCase() : '') >= 0 || mp.name === void"][eval exp="kag.msgWin.setName(%[name : mp.name, action : cvstr(mp), cvname : mp.cvname, serifColor : mp.serifcolor])"][else][eval exp="System.inform('[cv]タグエラー：\n未定義のネームIDです。\n\nname=' + mp.name)"][endif]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [macro name=transnowin]
11:27:50 MessageWindowController.ks : [msgwin page=no]
11:27:50 MessageWindowController.ks : [trans *][wt canskip=true]
11:27:50 MessageWindowController.ks : [msgwin]
11:27:50 MessageWindowController.ks : [endmacro]
11:27:50 macro : transnowin : [msgwin page=no][trans *][wt canskip=true][msgwin]
11:27:50 MessageWindowController.ks : 
11:27:50 MessageWindowController.ks : [return]
11:27:50 ================================================================================
11:27:50 Scenario loaded : first.ks
11:27:50 first.ks : returned to : (start) line offset 49
11:27:50 first.ks : call stack depth after returning : 0
11:27:50 first.ks : [call storage="CountDown.ks"]
11:27:50 first.ks : call stack depth before calling : 0
11:27:50 ================================================================================
11:27:50 Scenario loaded : CountDown.ks
11:27:50 CountDown.tjs を読み込みました(1ms)
11:27:50 CountDown.ks : 
11:27:50 CountDown.ks : [macro name=countdown]
11:27:50 CountDown.ks : [eval exp="kag.countDown.start(mp.count, mp.visible)" cond="kag.countDown !== void"]
11:27:50 CountDown.ks : [endmacro]
11:27:50 macro : countdown : [eval exp="kag.countDown.start(mp.count, mp.visible)" cond="kag.countDown !== void"]
11:27:50 CountDown.ks : 
11:27:50 CountDown.ks : [macro name=countdownnext]
11:27:50 CountDown.ks : [eval exp="kag.countDown.next()" cond="kag.countDown !== void"]
11:27:50 CountDown.ks : [endmacro]
11:27:50 macro : countdownnext : [eval exp="kag.countDown.next()" cond="kag.countDown !== void"]
11:27:50 CountDown.ks : 
11:27:50 CountDown.ks : [return]
11:27:50 ================================================================================
11:27:50 Scenario loaded : first.ks
11:27:50 first.ks : returned to : (start) line offset 50
11:27:50 first.ks : call stack depth after returning : 0
11:27:50 first.ks : [call storage="SaveWindow.ks"]
11:27:50 first.ks : call stack depth before calling : 0
11:27:50 ================================================================================
11:27:50 Scenario loaded : SaveWindow.ks
11:27:50 SaveWindow.tjs を読み込みました(10ms)
<<<<<<<<<<<snprintf 2, 11
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 51
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [137] to [184].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [184] to [246].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
=========================>TVPForceSwapBuffer() not imp
11:27:51 SaveWindow.ks : 
11:27:51 SaveWindow.ks : [macro name=setsavethum]
11:27:51 SaveWindow.ks : [eval exp="kag.fore.base.imageStorage=mp.storage"]
11:27:51 SaveWindow.ks : [endmacro]
11:27:51 macro : setsavethum : [eval exp="kag.fore.base.imageStorage=mp.storage"]
11:27:51 SaveWindow.ks : 
11:27:51 SaveWindow.ks : [return]
11:27:51 ================================================================================
11:27:51 Scenario loaded : first.ks
11:27:51 first.ks : returned to : (start) line offset 51
11:27:51 first.ks : call stack depth after returning : 0
11:27:51 first.ks : [call storage="ConfigWindow.ks"]
11:27:51 first.ks : call stack depth before calling : 0
11:27:51 ================================================================================
11:27:51 Scenario loaded : ConfigWindow.ks
11:27:51 ConfigWindow.tjs を読み込みました(17ms)
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasc.ksd
====================>TVPWriteDataToFile: /home/wmt/LIKExLOVE2/rootnuko/likexlove2/savedata/savedatasu.ksd
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [102] to [137].
=========================>TVPForceSwapBuffer() not imp
11:27:51 ==== An exception occured at configwindow.tjs(433)[(function) updateScreenMode], VM ip = 17 ====
11:27:51 -- Disassembled VM code --
<<<<<<<2.2 w == 0
<<<<<<<3 w == 0
<<<<<<<pl == 0, p == 0, sum == 0
<<<<<<<2.2 w == 0
<<<<<<<3 w == 9
<<<<<<<pl == 0, p == 9, sum == 9
<<<<<<<2.2 w == 0
<<<<<<<3 w == 0
<<<<<<<pl == 0, p == 0, sum == 0
11:27:51 #(145155870) 
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 1
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<<================= 1
<<<<<<<2.2 w == 0
<<<<<<<3 w == 6
<<<<<<<pl == 0, p == 6, sum == 6
<<<<<<<2.2 w == 8
<<<<<<<3 w == 8
<<<<<<<pl == 0, p == 1, sum == 1
<<<<<<<<================= 7
<<<<<<<2.2 w == 0

Thread 1 "kirikiroid2" received signal SIGSEGV, Segmentation fault.
0x08547670 in TJS::printf_core (f=0xbfffc870, fmt=0x8a6fbf8 u"%08d %ls", ap=0xbfffc9ec "\006", nl_arg=0xbfffc8a4, 
    nl_type=0xbfffc87c) at src/core/tjs2/tjsConfig.cpp:1924
1924	            while(*z) z++;
(gdb) bt
#0  0x08547670 in TJS::printf_core (f=0xbfffc870, fmt=0x8a6fbf8 u"%08d %ls", ap=0xbfffc9ec "\006", nl_arg=0xbfffc8a4, 
    nl_type=0xbfffc87c) at src/core/tjs2/tjsConfig.cpp:1924
#1  0x08547a66 in TJS::_vsnprintf (s=0x9d31950 u"00000006 \xadbǈ\x9d4ᥐ\x9d3!", n=43, fmt=0x8a6fbf8 u"%08d %ls", 
    ap=0xbfffc9ec "\006") at src/core/tjs2/tjsConfig.cpp:1991
#2  0x085494b7 in TJS::TJS_vsnprintf (string=0x9d31950 u"00000006 \xadbǈ\x9d4ᥐ\x9d3!", count=43, format=0x8a6fbf8 u"%08d %ls", 
    ap=0xbfffc9ec "\006") at src/core/tjs2/tjsConfig.cpp:2769
#3  0x085494f1 in TJS::TJS_snprintf (s=0x9d31950 u"00000006 \xadbǈ\x9d4ᥐ\x9d3!", count=43, format=0x8a6fbf8 u"%08d %ls")
    at src/core/tjs2/tjsConfig.cpp:2777
#4  0x0856a2ac in TJS::tTJSInterCodeContext::_output_func (msg=0xa71b27c0 u"const %1, *1", comment=0xa71c10a0 u"*0 = (null)", 
    addr=6, codestart=0xb454944, size=3, data=0xbfffcd44) at src/core/tjs2/tjsDisassemble.cpp:586
#5  0x08569f55 in TJS::tTJSInterCodeContext::Disassemble (this=0xb44a6d8, 
    output_func=0x856a24a <TJS::tTJSInterCodeContext::_output_func(char16_t const*, char16_t const*, int, int const*, int, void*)>, 
    output_func_src=0x856a356 <TJS::tTJSInterCodeContext::_output_func_src(char16_t const*, char16_t const*, int, void*)>, 
    data=0xbfffcd44, start=6, end=18) at src/core/tjs2/tjsDisassemble.cpp:565
#6  0x0856a4ac in TJS::tTJSInterCodeContext::Disassemble (this=0xb44a6d8, 
    output_func=0x85b188e <TJS::tTJSScriptBlock::ConsoleOutput(char16_t const*, void*)>, data=0xaab1ea8, start=6, end=18)
    at src/core/tjs2/tjsDisassemble.cpp:650
#7  0x0856a4ec in TJS::tTJSInterCodeContext::Disassemble (this=0xb44a6d8, start=6, end=18) at src/core/tjs2/tjsDisassemble.cpp:655
#8  0x0856a525 in TJS::tTJSInterCodeContext::DisassembleSrcLine (this=0xb44a6d8, codepos=17) at src/core/tjs2/tjsDisassemble.cpp:661
#9  0x0856f1e0 in TJS::tTJSInterCodeContext::DisplayExceptionGeneratedCode (this=0xb44a6d8, codepos=17, ra=0x969df38)
    at src/core/tjs2/tjsInterCodeExec.cpp:953
#10 0x08570fed in TJS::tTJSInterCodeContext::ExecuteCode (this=0xb44a6d8, ra_org=0x969df38, startip=0, args=0xbfffd14c, numargs=1, 
    result=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:1509
#11 0x0856ef5c in TJS::tTJSInterCodeContext::ExecuteAsFunction (this=0xb44a6d8, objthis=0xb3a5c70, args=0xbfffd14c, numargs=1, 
    result=0x0, start_ip=0) at src/core/tjs2/tjsInterCodeExec.cpp:884
#12 0x085770c4 in TJS::tTJSInterCodeContext::FuncCall (this=0xb44a6d8, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=1, 
    param=0xbfffd14c, objthis=0xb3a5c70) at src/core/tjs2/tjsInterCodeExec.cpp:3575
#13 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x0, numparams=1, param=0xbfffd14c, objthis=0xb3a5c70)
    at src/core/tjs2/tjsObject.cpp:1352
#14 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0xb3a5c70, flag=0, membername=0xa5d114a8 u"updateScreenMode", 
    hint=0xa5d114dc, result=0x0, numparams=1, param=0xbfffd14c, objthis=0xb3a5c70) at src/core/tjs2/tjsObject.cpp:1409
#15 0x0840e822 in TJS::tTJSVariantClosure::FuncCall (this=0xbfffd144, flag=0, membername=0xa5d114a8 u"updateScreenMode", 
    hint=0xa5d114dc, result=0x0, numparams=1, param=0xbfffd14c, objthis=0xb3a5c70) at ./src/core/tjs2/tjsVariant.h:230
#16 0x08574714 in TJS::tTJSInterCodeContext::CallFunctionDirect (this=0xa420c90, ra=0x969dea8, code=0xb3f7aac, args=0x0, numargs=0)
    at src/core/tjs2/tjsInterCodeExec.cpp:2883
#17 0x08570a0a in TJS::tTJSInterCodeContext::ExecuteCode (this=0xa420c90, ra_org=0x969dea8, startip=0, args=0x0, numargs=0, 
    result=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:1330
#18 0x0856ef5c in TJS::tTJSInterCodeContext::ExecuteAsFunction (this=0xa420c90, objthis=0x97aaca0, args=0x0, numargs=0, result=0x0, 
    start_ip=0) at src/core/tjs2/tjsInterCodeExec.cpp:884
#19 0x085770a6 in TJS::tTJSInterCodeContext::FuncCall (this=0xa420c90, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=0, 
    param=0x0, objthis=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:3568
#20 0x085b1dd2 in TJS::tTJSScriptBlock::ExecuteTopLevelScript (this=0xaab7350, result=0x0, context=0x0)
---Type <return> to continue, or q <return> to quit---
    at src/core/tjs2/tjsScriptBlock.cpp:418
#21 0x085b1a83 in TJS::tTJSScriptBlock::SetText (this=0xaab7350, result=0x0, 
    text=0xb3f68b4 u"KAGLoadScript('ConfigWindow.tjs');\r\nkag.configWindow = new ConfigWindow();\r\nif(typeof kag.sflags.config == UNDEFINED)\r\n{\r\nkag.sflags.config = kag.configWindow.getDefaultSetting();\r\nkag.sflags.LastWind"..., context=0x0, 
    isexpression=false) at src/core/tjs2/tjsScriptBlock.cpp:328
#22 0x085b8f9a in TJS::tTJSScriptCache::ExecScript (this=0x969b940, script=..., result=0x0, context=0x0, name=0xbfffd498, lineofs=1)
    at src/core/tjs2/tjsScriptCache.cpp:76
#23 0x08518ac5 in TJS::tTJS::ExecScript (this=0x9678320, script=..., result=0x0, context=0x0, name=0xbfffd498, lineofs=1)
    at src/core/tjs2/tjs.cpp:506
#24 0x08695903 in tTJSNC_Scripts::tTJSNC_Scripts()::NCM_exec::Process(TJS::tTJSVariant*, int, TJS::tTJSVariant**, TJS::iTJSDispatch2*) () at src/core/base/ScriptMgnIntf.cpp:1465
#25 0x0859d2f9 in TJS::tTJSNativeClassMethod::FuncCall (this=0x96b0188, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=3, 
    param=0xbfffd6ec, objthis=0xa2270d8) at src/core/tjs2/tjsNative.cpp:101
#26 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x0, numparams=3, param=0xbfffd6ec, objthis=0xa2270d8)
    at src/core/tjs2/tjsObject.cpp:1352
#27 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0x96aff20, flag=0, membername=0xb6d24348 u"exec", hint=0xb6d2437c, 
    result=0x0, numparams=3, param=0xbfffd6ec, objthis=0xa2270d8) at src/core/tjs2/tjsObject.cpp:1409
#28 0x0859df5c in TJS::tTJSNativeClass::FuncCall (this=0x96aff20, flag=0, membername=0xb6d24348 u"exec", hint=0xb6d2437c, 
    result=0x0, numparams=3, param=0xbfffd6ec, objthis=0xa2270d8) at src/core/tjs2/tjsNative.cpp:346
#29 0x0840e822 in TJS::tTJSVariantClosure::FuncCall (this=0xbfffd6e4, flag=0, membername=0xb6d24348 u"exec", hint=0xb6d2437c, 
    result=0x0, numparams=3, param=0xbfffd6ec, objthis=0xa2270d8) at ./src/core/tjs2/tjsVariant.h:230
#30 0x08574714 in TJS::tTJSInterCodeContext::CallFunctionDirect (this=0x9e47a98, ra=0x969de3c, code=0x9e48cd8, args=0xbfffdebc, 
    numargs=3) at src/core/tjs2/tjsInterCodeExec.cpp:2883
#31 0x08570a0a in TJS::tTJSInterCodeContext::ExecuteCode (this=0x9e47a98, ra_org=0x969de3c, startip=3, args=0xbfffdebc, numargs=3, 
    result=0x969dde8) at src/core/tjs2/tjsInterCodeExec.cpp:1330
#32 0x08571272 in TJS::tTJSInterCodeContext::ExecuteCodeInTryBlock (this=0x9e47a98, ra=0x969de3c, startip=3, args=0xbfffdebc, 
    numargs=3, result=0x969dde8, catchip=18, exobjreg=1) at src/core/tjs2/tjsInterCodeExec.cpp:1563
#33 0x08570d13 in TJS::tTJSInterCodeContext::ExecuteCode (this=0x9e47a98, ra_org=0x969de3c, startip=0, args=0xbfffdebc, numargs=3, 
    result=0x969dde8) at src/core/tjs2/tjsInterCodeExec.cpp:1427
#34 0x0856ef5c in TJS::tTJSInterCodeContext::ExecuteAsFunction (this=0x9e47a98, objthis=0xa2270d8, args=0xbfffdebc, numargs=3, 
    result=0x969dde8, start_ip=0) at src/core/tjs2/tjsInterCodeExec.cpp:884
#35 0x085770c4 in TJS::tTJSInterCodeContext::FuncCall (this=0x9e47a98, flag=0, membername=0x0, hint=0x0, result=0x969dde8, 
    numparams=3, param=0xbfffdebc, objthis=0xa2270d8) at src/core/tjs2/tjsInterCodeExec.cpp:3575
#36 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x969dde8, numparams=3, param=0xbfffdebc, objthis=0xa2270d8)
    at src/core/tjs2/tjsObject.cpp:1352
#37 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0xa2270d8, flag=0, membername=0xa5da5e30 u"onConductorScript", 
    hint=0xa5da5e64, result=0x969dde8, numparams=3, param=0xbfffdebc, objthis=0xa2270d8) at src/core/tjs2/tjsObject.cpp:1409
#38 0x0840e822 in TJS::tTJSVariantClosure::FuncCall (this=0xbfffdb84, flag=0, membername=0xa5da5e30 u"onConductorScript", 
    hint=0xa5da5e64, result=0x969dde8, numparams=3, param=0xbfffdebc, objthis=0xa2270d8) at ./src/core/tjs2/tjsVariant.h:230
#39 0x08574714 in TJS::tTJSInterCodeContext::CallFunctionDirect (this=0x9d0f4d0, ra=0x969ddd0, code=0x9d11b24, args=0xbfffdebc, 
    numargs=3) at src/core/tjs2/tjsInterCodeExec.cpp:2883
#40 0x08570a0a in TJS::tTJSInterCodeContext::ExecuteCode (this=0x9d0f4d0, ra_org=0x969ddd0, startip=0, args=0xbfffdebc, numargs=3, 
---Type <return> to continue, or q <return> to quit---
    result=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:1330
#41 0x0856ef5c in TJS::tTJSInterCodeContext::ExecuteAsFunction (this=0x9d0f4d0, objthis=0xa323ed8, args=0xbfffdebc, numargs=3, 
    result=0x0, start_ip=0) at src/core/tjs2/tjsInterCodeExec.cpp:884
#42 0x085770c4 in TJS::tTJSInterCodeContext::FuncCall (this=0x9d0f4d0, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=3, 
    param=0xbfffdebc, objthis=0xa323ed8) at src/core/tjs2/tjsInterCodeExec.cpp:3575
#43 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x0, numparams=3, param=0xbfffdebc, objthis=0xa323ed8)
    at src/core/tjs2/tjsObject.cpp:1352
#44 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0xa323ed8, flag=0, membername=0xa71a61bc u"onScript", hint=0xa71a61f0, 
    result=0x0, numparams=3, param=0xbfffdebc, objthis=0xa323ed8) at src/core/tjs2/tjsObject.cpp:1409
#45 0x08502215 in tTJSNI_KAGParser::SkipCommentOrLabel (this=0xa324048) at src/core/utils/KAGParser.cpp:1221
#46 0x08503598 in tTJSNI_KAGParser::_GetNextTag (this=0xa324048) at src/core/utils/KAGParser.cpp:1472
#47 0x08507aeb in tTJSNI_KAGParser::GetNextTag (this=0xa324048) at src/core/utils/KAGParser.cpp:2288
#48 0x08507f20 in NCM_getNextTag::Process (result=0x969dd4c, numparams=0, param=0xbfffe2ec, objthis=0xa323ed8)
    at src/core/utils/KAGParser.cpp:2357
#49 0x0859d2f9 in TJS::tTJSNativeClassMethod::FuncCall (this=0x96a6d28, flag=0, membername=0x0, hint=0x0, result=0x969dd4c, 
    numparams=0, param=0xbfffe2ec, objthis=0xa323ed8) at src/core/tjs2/tjsNative.cpp:101
#50 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x969dd4c, numparams=0, param=0xbfffe2ec, objthis=0xa323ed8)
    at src/core/tjs2/tjsObject.cpp:1352
#51 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0xa323ed8, flag=0, membername=0xb6d1f2c8 u"getNextTag", hint=0xb6d1f2fc, 
    result=0x969dd4c, numparams=0, param=0xbfffe2ec, objthis=0xa323ed8) at src/core/tjs2/tjsObject.cpp:1409
#52 0x0857925c in TJS::tTJSObjectProxy::FuncCall (this=0xbfffe554, flag=0, membername=0xb6d1f2c8 u"getNextTag", hint=0xb6d1f2fc, 
    result=0x969dd4c, numparams=0, param=0xbfffe2ec, objthis=0xa323ed8) at src/core/tjs2/tjsInterCodeExec.cpp:285
#53 0x0840e822 in TJS::tTJSVariantClosure::FuncCall (this=0xbfffe2e4, flag=0, membername=0xb6d1f2c8 u"getNextTag", hint=0xb6d1f2fc, 
    result=0x969dd4c, numparams=0, param=0xbfffe2ec, objthis=0xa323ed8) at ./src/core/tjs2/tjsVariant.h:230
#54 0x08574714 in TJS::tTJSInterCodeContext::CallFunctionDirect (this=0x9ce25c0, ra=0x969dd40, code=0x9ce9330, args=0xbfffe674, 
    numargs=1) at src/core/tjs2/tjsInterCodeExec.cpp:2883
#55 0x08570a0a in TJS::tTJSInterCodeContext::ExecuteCode (this=0x9ce25c0, ra_org=0x969dd40, startip=29, args=0xbfffe674, numargs=1, 
    result=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:1330
#56 0x08571272 in TJS::tTJSInterCodeContext::ExecuteCodeInTryBlock (this=0x9ce25c0, ra=0x969dd40, startip=29, args=0xbfffe674, 
    numargs=1, result=0x0, catchip=645, exobjreg=1) at src/core/tjs2/tjsInterCodeExec.cpp:1563
#57 0x08570d13 in TJS::tTJSInterCodeContext::ExecuteCode (this=0x9ce25c0, ra_org=0x969dd40, startip=0, args=0xbfffe674, numargs=1, 
    result=0x0) at src/core/tjs2/tjsInterCodeExec.cpp:1427
#58 0x0856ef5c in TJS::tTJSInterCodeContext::ExecuteAsFunction (this=0x9ce25c0, objthis=0xa323ed8, args=0xbfffe674, numargs=1, 
    result=0x0, start_ip=0) at src/core/tjs2/tjsInterCodeExec.cpp:884
#59 0x085770c4 in TJS::tTJSInterCodeContext::FuncCall (this=0x9ce25c0, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=1, 
    param=0xbfffe674, objthis=0xa323ed8) at src/core/tjs2/tjsInterCodeExec.cpp:3575
#60 0x0840e822 in TJS::tTJSVariantClosure::FuncCall (this=0xbfffe688, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=1, 
    param=0xbfffe674, objthis=0x0) at ./src/core/tjs2/tjsVariant.h:230
#61 0x0868bb68 in tTJSNC_AsyncTrigger::tTJSNC_AsyncTrigger()::NCM_onFire::Process(TJS::tTJSVariant*, int, TJS::tTJSVariant**, TJS::iTJSDispatch2*) () at src/core/base/EventIntf.cpp:1383
#62 0x0859d2f9 in TJS::tTJSNativeClassMethod::FuncCall (this=0x968ef60, flag=0, membername=0x0, hint=0x0, result=0x0, numparams=0, 
    param=0x984a290, objthis=0xa3250b0) at src/core/tjs2/tjsNative.cpp:101
---Type <return> to continue, or q <return> to quit---
#63 0x085a149a in TJS::TJSDefaultFuncCall (flag=0, targ=..., result=0x0, numparams=0, param=0x984a290, objthis=0xa3250b0)
    at src/core/tjs2/tjsObject.cpp:1352
#64 0x085a164c in TJS::tTJSCustomObject::FuncCall (this=0xa3250b0, flag=0, membername=0xa71a563c u"onFire", hint=0xa71a5670, 
    result=0x0, numparams=0, param=0x984a290, objthis=0xa3250b0) at src/core/tjs2/tjsObject.cpp:1409
#65 0x0868c669 in tTVPEvent::Deliver (this=0xa363ca0) at src/core/base/EventIntf.cpp:107
#66 0x08688405 in _TVPDeliverEventByPrio (prio=32) at src/core/base/EventIntf.cpp:509
#67 0x086884b2 in _TVPDeliverAllEvents2 () at src/core/base/EventIntf.cpp:532
#68 0x08688620 in _TVPDeliverAllEvents () at src/core/base/EventIntf.cpp:593
#69 0x0868868c in TVPDeliverAllEvents () at src/core/base/EventIntf.cpp:613
#70 0x085f9cd3 in tTVPSystemControl::DeliverEvents (this=0x96573c8) at src/core/environ/win32/SystemControl.cpp:120
#71 0x085f9d79 in tTVPSystemControl::SystemWatchTimerTimer (this=0x96573c8) at src/core/environ/win32/SystemControl.cpp:164
#72 0x0868010d in tTVPApplication::Run (this=0x9094050) at src/core/environ/Application.cpp:784
#73 0x08602e06 in TVPMainScene::update (this=0x9282ca8, delta=0) at src/core/environ/cocos2d/MainScene.cpp:1846
#74 0x086028a7 in TVPMainScene::doStartup (this=0x9282ca8, dt=0.0169680007, path="/home/wmt/LIKExLOVE2/data.xp3")
    at src/core/environ/cocos2d/MainScene.cpp:1811
#75 0x086182e4 in std::_Mem_fn_base<void (TVPMainScene::*)(float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >), true>::operator()<float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, void>(TVPMainScene*, float&&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const (this=0x968d788, 
    __object=0x9282ca8) at /usr/include/c++/5/functional:600
#76 0x08616e2e in std::_Bind<std::_Mem_fn<void (TVPMainScene::*)(float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)> (TVPMainScene*, std::_Placeholder<1>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)>::__call<void, float&&, 0u, 1u, 2u>(std::tuple<float&&>&&, std::_Index_tuple<0u, 1u, 2u>) (this=0x968d788, 
    __args=<unknown type in /home/wmt/kirikiroid2lite/project/kirikiroid2, CU 0xd0c9db, DIE 0xd6107e>)
    at /usr/include/c++/5/functional:1074
#77 0x0861531e in std::_Bind<std::_Mem_fn<void (TVPMainScene::*)(float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)> (TVPMainScene*, std::_Placeholder<1>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)>::operator()<float, void>(float&&) (this=0x968d788) at /usr/include/c++/5/functional:1133
#78 0x08612c76 in std::_Function_handler<void (float), std::_Bind<std::_Mem_fn<void (TVPMainScene::*)(float, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)> (TVPMainScene*, std::_Placeholder<1>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)> >::_M_invoke(std::_Any_data const&, float&&) (__functor=..., 
    __args#0=<unknown type in /home/wmt/kirikiroid2lite/project/kirikiroid2, CU 0xd0c9db, DIE 0xd5a612>)
    at /usr/include/c++/5/functional:1871
#79 0x0867713c in std::function<void (float)>::operator()(float) const (this=0x9799854, __args#0=0.0169680007)
    at /usr/include/c++/5/functional:2267
#80 0x087b6dd2 in cocos2d::TimerTargetCallback::trigger (this=0x9799820) at cocos2d-x-3.6/cocos/base/CCScheduler.cpp:201
#81 0x087b6b7c in cocos2d::Timer::update (this=0x9799820, dt=0.0169680007) at cocos2d-x-3.6/cocos/base/CCScheduler.cpp:132
#82 0x087bdcd9 in cocos2d::Scheduler::update (this=0x90982c8, dt=0.0169680007) at cocos2d-x-3.6/cocos/base/CCScheduler.cpp:895
#83 0x0879395a in cocos2d::Director::drawScene (this=0x90966b0) at cocos2d-x-3.6/cocos/base/CCDirector.cpp:270
#84 0x08797435 in cocos2d::DisplayLinkDirector::mainLoop (this=0x90966b0) at cocos2d-x-3.6/cocos/base/CCDirector.cpp:1380
#85 0x0876c169 in cocos2d::Application::run (this=0xbfffedec) at cocos2d-x-3.6/cocos/platform/linux/CCApplication-linux.cpp:86
#86 0x0805517d in main (argc=1, argv=0xbfffeec4) at ./project/proj.linux/main.cpp:27



