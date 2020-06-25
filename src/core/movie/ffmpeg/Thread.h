#pragma once
#include "KRMovieDef.h"
#if 0
#include <boost/atomic.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#endif

NS_KRMOVIE_BEGIN
typedef boost::recursive_mutex CCriticalSection;
typedef boost::unique_lock<boost::recursive_mutex> CSingleLock;
class CThread {
public:
	CThread();
	virtual ~CThread();
	void Create();
	bool IsRunning() const { return m_bRunning; }
	void StopThread(bool bWait = true);
	void Sleep(unsigned int milliseconds);
	bool IsCurrentThread();

protected:
	int entry();
	virtual void OnStartup() {}
	virtual void Process() = 0;
	virtual void OnExit() {}

	boost::thread *m_ThreadId/* = nullptr*/;
	boost::atomic<bool> m_bStop, m_bRunning;
	boost::mutex m_mtxStopEvent;
	boost::condition_variable m_StopEvent;
	CCriticalSection m_CriticalSection;
};

class CEvent {
public:
	void Set() { m_cond.notify_all(); }
	void Reset() {}
	bool WaitMSec(unsigned int milliSeconds) {
		CSingleLock lock(mutex);
		return m_cond.wait_for(lock, boost::chrono::milliseconds(milliSeconds)) != boost::cv_status::timeout;
	}
	void Wait() {
		CSingleLock lock(mutex);
		m_cond.wait(lock);
	}

protected:
	CCriticalSection mutex;
	boost::condition_variable_any m_cond;
};
NS_KRMOVIE_END