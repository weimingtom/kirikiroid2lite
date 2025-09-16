<<<<<<<<<<<snprintf 2, 59
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
<<<<<<<<<<<snprintf 2, 27
<<<<<<<2 w == 2
<<<<<<<2.2 w == 2
<<<<<<<3 w == 2
<<<<<<<pl == 0, p == 2, sum == 2
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [30] to [41].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [41] to [56].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [56] to [76].
cocos2d: SpriteBatchNode: resizing TextureAtlas capacity from [76] to [102].
^C
Thread 1 "kirikiroid2" received signal SIGINT, Interrupt.
0x0000003ff1259e3e in ?? () from /usr/lib/riscv64-linux-gnu/dri/pvr_dri.so
(gdb) bt
#0  0x0000003ff1259e3e in  () at /usr/lib/riscv64-linux-gnu/dri/pvr_dri.so
(gdb) threads
Undefined command: "threads".  Try "help".
(gdb) thread
[Current thread is 1 (Thread 0x3ff6e18a20 (LWP 21335))]
(gdb) info thread
  Id   Target Id                                      Frame 
* 1    Thread 0x3ff6e18a20 (LWP 21335) "kirikiroid2"  0x0000003ff1259e3e in ??
    () from /usr/lib/riscv64-linux-gnu/dri/pvr_dri.so
  2    Thread 0x3ff25e70c0 (LWP 21337) "PulseHotplug" 0x0000003ff79f330e in __ppoll (fds=0x2aabf08970, nfds=2, timeout=<optimized out>, sigmask=0x0)
    at ../sysdeps/unix/sysv/linux/ppoll.c:42
  8    Thread 0x3ff091b0c0 (LWP 21345) "kirikiroid2"  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x3ff091a7a8, 
    op=<optimized out>, expected=<optimized out>, futex_word=0x2aadd35668)
    at ./nptl/futex-internal.c:57
  9    Thread 0x3fe31d00c0 (LWP 21346) "kirikiroid2"  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x0, op=<optimized out>, 
    expected=<optimized out>, futex_word=0x2aadcadd58)
--Type <RET> for more, q to quit, c to continue without paging--
    at ./nptl/futex-internal.c:57
(gdb) bt
#0  0x0000003ff1259e3e in  () at /usr/lib/riscv64-linux-gnu/dri/pvr_dri.so
(gdb) thread 2
[Switching to thread 2 (Thread 0x3ff25e70c0 (LWP 21337))]
#0  0x0000003ff79f330e in __ppoll (fds=0x2aabf08970, nfds=2, timeout=<optimized out>, sigmask=0x0) at ../sysdeps/unix/sysv/linux/ppoll.c:42
42	../sysdeps/unix/sysv/linux/ppoll.c: No such file or directory.
(gdb) thread 2
[Switching to thread 2 (Thread 0x3ff25e70c0 (LWP 21337))]
#0  0x0000003ff79f330e in __ppoll (fds=0x2aabf08970, nfds=2, timeout=<optimized out>, sigmask=0x0) at ../sysdeps/unix/sysv/linux/ppoll.c:42
42	in ../sysdeps/unix/sysv/linux/ppoll.c
(gdb) bt
#0  0x0000003ff79f330e in __ppoll (fds=0x2aabf08970, nfds=2, timeout=<optimized out>, sigmask=0x0) at ../sysdeps/unix/sysv/linux/ppoll.c:42
#1  0x0000003ff77b022e in pa_mainloop_poll () at /lib/riscv64-linux-gnu/libpulse.so.0
#2  0x0000003ff77b06e2 in pa_mainloop_iterate () at /lib/riscv64-linux-gnu/libpulse.so.0
#3  0x0000003ff77b0776 in pa_mainloop_run () at /lib/riscv64-linux-gnu/libpulse.so.0
#4  0x0000003ff7df95de in  () at /lib/riscv64-linux-gnu/libSDL2-2.0.so.0
(gdb) thread 3
Unknown thread 3.
(gdb) th 4
Ambiguous command "th 4": thbreak, thread.
(gdb) thread 4
Unknown thread 4.
(gdb) thread 8
[Switching to thread 8 (Thread 0x3ff091b0c0 (LWP 21345))]
#0  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x3ff091a7a8, op=<optimized out>, expected=<optimized out>, futex_word=0x2aadd35668) at ./nptl/futex-internal.c:57
57	./nptl/futex-internal.c: No such file or directory.
(gdb) bt
#0  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x3ff091a7a8, op=<optimized out>, expected=<optimized out>, futex_word=0x2aadd35668) at ./nptl/futex-internal.c:57
#1  __futex_abstimed_wait_common
    (futex_word=futex_word@entry=0x2aadd35668, expected=<optimized out>, clockid=<optimized out>, abstime=abstime@entry=0x3ff091a7a8, private=<optimized out>, cancel=cancel@entry=true)
    at ./nptl/futex-internal.c:87
#2  0x0000003ff79a9f72 in __GI___futex_abstimed_wait_cancelable64
    (futex_word=futex_word@entry=0x2aadd35668, expected=expected@entry=0, clockid=clockid@entry=0, abstime=abstime@entry=0x3ff091a7a8, private=private@entry=0) at ./nptl/futex-internal.c:139
#3  0x0000003ff79abf1e in __pthread_cond_wait_common (abstime=0x3ff091a7a8, clockid=0, mutex=0x2aadd35670, cond=0x2aadd35640) at ./nptl/pthread_cond_wait.c:503
#4  ___pthread_cond_timedwait64 (cond=0x2aadd35640, mutex=0x2aadd35670, abstime=0x3ff091a7a8) at ./nptl/pthread_cond_wait.c:643
#5  0x0000002aab0a6ecc in tTVPThreadEvent::WaitFor(unsigned int) (this=0x2aadd35638, timeout=268435456) at src/core/utils/win32/ThreadImpl.cpp:220
#6  0x0000002aab0ca542 in tTVPWatchThread::Execute() (this=0x2aadd355c0) at src/core/utils/TickCount.cpp:88
#7  0x0000002aab0a6bb6 in tTVPThread::StartProc(void*) (arg=0x2aadd355c0) at src/core/utils/win32/ThreadImpl.cpp:112
#8  0x0000003ff79ac51c in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#9  0x0000003ff79f9dfa in __thread_start () at ../sysdeps/unix/sysv/linux/riscv/clone.S:85
(gdb) thread 9
[Switching to thread 9 (Thread 0x3fe31d00c0 (LWP 21346))]
#0  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x0, op=<optimized out>, expected=<optimized out>, futex_word=0x2aadcadd58) at ./nptl/futex-internal.c:57
57	in ./nptl/futex-internal.c
(gdb) bt
#0  __futex_abstimed_wait_common64 (private=<optimized out>, cancel=true, abstime=0x0, op=<optimized out>, expected=<optimized out>, futex_word=0x2aadcadd58) at ./nptl/futex-internal.c:57
#1  __futex_abstimed_wait_common (futex_word=futex_word@entry=0x2aadcadd58, expected=<optimized out>, clockid=<optimized out>, abstime=abstime@entry=0x0, private=<optimized out>, cancel=cancel@entry=true)
    at ./nptl/futex-internal.c:87
#2  0x0000003ff79a9f72 in __GI___futex_abstimed_wait_cancelable64
    (futex_word=futex_word@entry=0x2aadcadd58, expected=expected@entry=0, clockid=clockid@entry=0, abstime=abstime@entry=0x0, private=private@entry=0) at ./nptl/futex-internal.c:139
#3  0x0000003ff79abce4 in __pthread_cond_wait_common (abstime=0x0, clockid=0, mutex=0x2aadcadd60, cond=0x2aadcadd30) at ./nptl/pthread_cond_wait.c:503
#4  ___pthread_cond_wait (cond=0x2aadcadd30, mutex=0x2aadcadd60) at ./nptl/pthread_cond_wait.c:618
#5  0x0000002aab0a6eea in tTVPThreadEvent::WaitFor(unsigned int) (this=0x2aadcadd28, timeout=0) at src/core/utils/win32/ThreadImpl.cpp:222
#6  0x0000002aaafb41c2 in tTVPAsyncImageLoader::LoadingThread() (this=0x2aadcadc80) at src/core/visual/GraphicsLoadThread.cpp:247
#7  0x0000002aaafb3788 in tTVPAsyncImageLoader::Execute() (this=0x2aadcadc80) at src/core/visual/GraphicsLoadThread.cpp:112
#8  0x0000002aab0a6bb6 in tTVPThread::StartProc(void*) (arg=0x2aadcadc80) at src/core/utils/win32/ThreadImpl.cpp:112
#9  0x0000003ff79ac51c in start_thread (arg=<optimized out>) at ./nptl/pthread_create.c:442
#10 0x0000003ff79f9dfa in __thread_start () at ../sysdeps/unix/sysv/linux/riscv/clone.S:85
(gdb) 

