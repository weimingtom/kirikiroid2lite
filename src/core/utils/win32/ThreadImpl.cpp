//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Thread base class
//---------------------------------------------------------------------------
#define NOMINMAX
#include "tjsCommHead.h"

//#include <process.h>
#include <algorithm>

#include "ThreadIntf.h"
#include "ThreadImpl.h"
#include "MsgIntf.h"
#include "DebugIntf.h"

#if defined(CC_TARGET_OS_IPHONE) || defined(__aarch64__)
#else
//#define USING_THREADPOOL11
#endif

#ifdef USING_THREADPOOL11
#include "threadpool11/pool.hpp"
#endif
#include <pthread.h>

#if defined(ANDROID)
#include <android/log.h>
#endif

#ifdef _MSC_VER
//https://github.com/LarryIII/Larry_Vcpkg/blob/fa94febc7cc9c68f2743ba87acfbea2e86785d69/ports/gettimeofday/gettimeofday.c

#include <winsock2.h>
#include <time.h>

/* FILETIME of Jan 1 1970 00:00:00. */
static const unsigned __int64 epoch = 116444736000000000Ui64;

/*
 * timezone information is stored outside the kernel so tzp isn't used anymore.
 *
 * Note: this function is not for Win32 high precision timing purpose. See
 * elapsed_time().
 */
static int
gettimeofday__(struct timeval * tp, struct timezone * tzp)
{
	FILETIME	file_time;
	SYSTEMTIME	system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

	return 0;
}
#else
#include <sys/time.h>
#endif

//---------------------------------------------------------------------------
// tTVPThread : a wrapper class for thread
//---------------------------------------------------------------------------
tTVPThread::tTVPThread(bool suspended)
{
#if defined(ANDROID)
	__android_log_print(ANDROID_LOG_ERROR, "ThreadImpl.cpp", "%s", "*** *** tTVPThread");
#endif
	pthread_mutex_init(&_mutex, NULL);
	pthread_cond_init(&_cond, NULL);

	Terminated = false;
	Suspended = suspended;

	pthread_attr_t attr;
	if (pthread_attr_init(&attr) != 0) {
		TVPThrowInternalError;
	}
	if (pthread_create(&Handle, &attr, StartProc, this) != 0) {
		pthread_attr_destroy(&attr);
		TVPThrowInternalError;
	}
	pthread_attr_destroy(&attr);
}
//---------------------------------------------------------------------------
tTVPThread::~tTVPThread()
{
	//CloseHandle(Handle);
	pthread_cond_destroy(&_cond);
	pthread_mutex_destroy(&_mutex);
}
//---------------------------------------------------------------------------
void * tTVPThread::StartProc(void * arg)
{
	tTVPThread* _this = ((tTVPThread*)arg);
	if (_this->Suspended) {
		pthread_mutex_lock(&_this->_mutex);
		pthread_cond_wait(&_this->_cond, &_this->_mutex);
		pthread_mutex_unlock(&_this->_mutex);
	}
	_this->Execute();
	TVPOnThreadExited();
	return nullptr;
}
//---------------------------------------------------------------------------
void tTVPThread::WaitFor()
{
	void *result;
	pthread_join(Handle, &result);
}
//---------------------------------------------------------------------------
tTVPThreadPriority tTVPThread::GetPriority()
{
	sched_param npri = { 0 };
	int policy = 0;
	pthread_getschedparam(Handle, &policy, &npri);

	switch (policy)
	{
	case 5/*SCHED_IDLE*/: return ttpIdle;
	case 3/*SCHED_BATCH*/: return (npri.sched_priority == 0) ? ttpLowest : ttpLower;
	case 0/*SCHED_NORMAL*/:
		switch (npri.sched_priority) {
		case 0: return ttpNormal;
		case 1: return ttpHigher;
		case 2: return ttpHighest;
		}
		break;
	case 2/*SCHED_RR*/: return ttpTimeCritical;
	}

	return ttpNormal;
}
//---------------------------------------------------------------------------
void tTVPThread::SetPriority(tTVPThreadPriority pri)
{
	sched_param npri = { 0 }; //SCHED_NORMAL
	int policy = 0;
	switch (pri)
	{
	case ttpIdle:			policy = 5/*SCHED_IDLE*/;			break;
	case ttpLowest:			policy = 3/*SCHED_BATCH*/;	npri.sched_priority = 0;		break;
	case ttpLower:			policy = 3/*SCHED_BATCH*/;	npri.sched_priority = 1;        break;
	case ttpNormal:			policy = 0/*SCHED_NORMAL*/;		break;
	case ttpHigher:			policy = 0/*SCHED_NORMAL*/;	npri.sched_priority = 1;    break;
	case ttpHighest:		policy = 0/*SCHED_NORMAL*/;	npri.sched_priority = 2;	break;
	case ttpTimeCritical:	policy = 2/*SCHED_RR*/;	    break;
	}
	pthread_setschedparam(Handle, policy, &npri);
}
//---------------------------------------------------------------------------
// void tTVPThread::Suspend()
// {
// 	SuspendThread(Handle);
// }
//---------------------------------------------------------------------------
void tTVPThread::Resume()
{
	Suspended = false;
	pthread_cond_signal(&_cond);
	//while((tjs_int32)ResumeThread(Handle) > 1) ;
}
//---------------------------------------------------------------------------







//---------------------------------------------------------------------------
// tTVPThreadEvent
//---------------------------------------------------------------------------
void tTVPThreadEvent::Set()
{
	pthread_mutex_lock(&Mutex);
	pthread_cond_signal(&Handle);
	pthread_mutex_unlock(&Mutex);
}
//---------------------------------------------------------------------------
void tTVPThreadEvent::WaitFor(tjs_uint timeout)
{
	// wait for event;
	// returns true if the event is set, otherwise (when timed out) returns false.

	pthread_mutex_lock(&Mutex);
	if (timeout != 0) {
		//https://github.com/JIANGJZ/DCR/blob/f55f8835c97e11d855d4e6453b1da73fcb80216e/src/util/condition_variable.cpp
		struct timeval tv;
#ifdef _MSC_VER
		gettimeofday__(&tv, NULL);
#else
		gettimeofday(&tv, NULL);
#endif

		tv.tv_sec += timeout / 1000;
		tv.tv_usec += (timeout % 1000) * 1000;
    
		int32_t million = 1000000;
		if (tv.tv_usec >= million) {
			tv.tv_sec += tv.tv_usec / million;
			tv.tv_usec %= million;
		}
    
		struct timespec ts;
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000;

		pthread_cond_timedwait(&Handle, &Mutex, &ts);
	} else {
		pthread_cond_wait(&Handle, &Mutex);
	}
	pthread_mutex_unlock(&Mutex);
	
#if 0
	DWORD state = WaitForSingleObject(Handle, timeout == 0 ? INFINITE : timeout);

	if(state == WAIT_OBJECT_0) return true;
	return false;
#endif
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tjs_int TVPDrawThreadNum = 1;

static std::vector<tjs_int> TVPProcesserIdList;
static tjs_int TVPThreadTaskNum, TVPThreadTaskCount;

//---------------------------------------------------------------------------
//https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
#ifdef _WIN32
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

static int getNumCores() {
#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

static tjs_int GetProcesserNum(void)
{
  static tjs_int processor_num = 0;
  if (! processor_num) {
#if defined(_MSC_VER)
	  processor_num = pthread_num_processors_np();
#elif defined(ANDROID)
	  processor_num = 1;
#else
	  processor_num = 1;
#endif
	tjs_char tmp[34];
	TVPAddLog(ttstr(TJS_W("Detected CPU core(s): ")) + TJS_tTVInt_to_str(processor_num, tmp));
  }
  return processor_num;
}

tjs_int TVPGetProcessorNum(void)
{
  return GetProcesserNum();
}

//---------------------------------------------------------------------------
tjs_int TVPGetThreadNum(void)
{
  tjs_int threadNum = TVPDrawThreadNum ? TVPDrawThreadNum : GetProcesserNum();
  threadNum = std::min(threadNum, TVPMaxThreadNum);
  return threadNum;
}

//---------------------------------------------------------------------------
void TVPExecThreadTask(int numThreads, TVP_THREAD_TASK_FUNC func)
{
  if (numThreads == 1) {
    func(0);
    return;
  }
#if !defined(USING_THREADPOOL11)
#pragma omp parallel for schedule(static)
  for (int i = 0; i < numThreads; ++i)
	  func(i);
#else
  static threadpool11::Pool pool;
  std::vector<boost::future<void>> futures;
  for (int i = 0; i < numThreads; ++i) {
	  futures.push_back(pool.postWork<void>(std::bind(func, i)));
  }
  for (auto& it : futures)
	  it.get();
#endif
#if 0
  ThreadInfo *threadInfo;
  threadInfo = TVPThreadList[TVPThreadTaskCount++];
  threadInfo->lpStartAddress = func;
  threadInfo->lpParameter = param;
  InterlockedIncrement(&TVPRunningThreadCount);
  while (ResumeThread(threadInfo->thread) == 0)
    Sleep(0);
#endif
}
//---------------------------------------------------------------------------

std::vector<std::function<void()>> _OnThreadExitedEvents;

void TVPOnThreadExited() {
	//for (const auto &ev : _OnThreadExitedEvents) {
	for (auto p_ev = _OnThreadExitedEvents.begin(); p_ev != _OnThreadExitedEvents.end(); ++p_ev)
	{
		const auto& ev = *p_ev;
		ev();
	}
}

void TVPAddOnThreadExitEvent(const std::function<void()> &ev)
{
	_OnThreadExitedEvents.push_back(ev);
}
