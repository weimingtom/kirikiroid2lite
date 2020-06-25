#include "Thread.h"
#include <boost/thread/thread.hpp>
#include <stdexcept>
#include "MsgIntf.h"
#include "ThreadImpl.h"

NS_KRMOVIE_BEGIN

CThread::CThread()
	: m_bStop(false), m_bRunning(false), m_ThreadId(nullptr)
{

}

CThread::~CThread()
{
	if (m_bRunning) {
		StopThread();
	}
	if (m_ThreadId) {
		m_ThreadId->join(); delete m_ThreadId;
	}
}

void CThread::Create()
{
	if (m_bRunning.exchange(true)) {
		TVPThrowExceptionMessage(TJS_W("thread already in running"));
	}
	m_bStop = false;
	if (m_ThreadId) {
		m_ThreadId->join(); delete m_ThreadId;
	}
	m_ThreadId = new boost::thread(&CThread::entry, this);
}

void CThread::StopThread(bool bWait /*= true*/)
{
	m_bStop = true;
	m_StopEvent.notify_all();
	if (m_ThreadId && bWait) {
		m_ThreadId->join();
		delete m_ThreadId;
		m_ThreadId = nullptr;
	}
}

void CThread::Sleep(unsigned int milliseconds)
{
	if (IsCurrentThread()) {
		boost::unique_lock<boost::mutex> lock(m_mtxStopEvent);
		m_StopEvent.wait_for(lock, boost::chrono::milliseconds(milliseconds));
	} else {
		boost::this_thread::sleep_for(boost::chrono::milliseconds(milliseconds));
	}
}

bool CThread::IsCurrentThread()
{
	if (!m_ThreadId) return false;
	return m_ThreadId->get_id() == boost::this_thread::get_id();
}

int CThread::entry()
{
	OnStartup();
	Process();
	OnExit();
	m_bRunning = false;
	TVPOnThreadExited();
	return 0;
}

NS_KRMOVIE_END
