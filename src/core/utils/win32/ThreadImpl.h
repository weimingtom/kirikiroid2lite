//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Thread base class
//---------------------------------------------------------------------------
#ifndef ThreadImplH
#define ThreadImplH
#include "tjsNative.h"
#include "ThreadIntf.h"
#include <pthread.h>
#if defined(ANDROID)
#include <android/log.h>
#endif

//---------------------------------------------------------------------------
// tTVPThread
//---------------------------------------------------------------------------
class tTVPThread
{
	bool Terminated;
	pthread_t Handle;
	//DWORD ThreadId;
	pthread_mutex_t _mutex; // for suspend
	pthread_cond_t _cond;
	bool Suspended;

	static void* /*__stdcall*/ StartProc(void * arg);

public:
	tTVPThread(bool suspended);
	virtual ~tTVPThread();

	bool GetTerminated() const { return Terminated; }
	void SetTerminated(bool s) { Terminated = s; }
	void Terminate() { Terminated = true; }

protected:
	virtual void Execute() = 0;

public:
	void WaitFor();

	tTVPThreadPriority GetPriority();
	void SetPriority(tTVPThreadPriority pri);

	//void Suspend();
	void Resume();

// 	HANDLE GetHandle() const { return Handle; } 	/* win32 specific */
// 	DWORD GetThreadId() const { return ThreadId; }  /* win32 specific */

};
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// tTVPThreadEvent
//---------------------------------------------------------------------------
class tTVPThreadEvent
{
	pthread_cond_t Handle;
	pthread_mutex_t Mutex;

public:
	void Set();
	void WaitFor(tjs_uint timeout);
	tTVPThreadEvent() {
#if defined(ANDROID)
		__android_log_print(ANDROID_LOG_ERROR, "ThreadImpl.h", "%s", "*** *** tTVPThreadEvent");
#endif
		pthread_cond_init(&Handle, NULL);
		pthread_mutex_init(&Mutex, NULL);
	}

	virtual ~tTVPThreadEvent() {
		pthread_cond_destroy(&Handle);
		pthread_mutex_destroy(&Mutex);
	}
};
//---------------------------------------------------------------------------

void TVPAddOnThreadExitEvent(const std::function<void()> &ev);
void TVPOnThreadExited();
//---------------------------------------------------------------------------
#endif
