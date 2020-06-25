
#ifndef __VSYNC_TIMING_THREAD_H__
#define __VSYNC_TIMING_THREAD_H__

#include "ThreadIntf.h"
#include "NativeEventQueue.h"

//---------------------------------------------------------------------------
// VSync�p�̃^�C�~���O�𔭐������邽�߂̃X���b�h
//---------------------------------------------------------------------------
class tTVPVSyncTimingThread : public tTVPThread
{
	tjs_uint32 SleepTime;
	tTVPThreadEvent Event;
	tTJSCriticalSection CS;
	tjs_uint32 VSyncInterval; //!< VSync �̊Ԋu(�Q�l�l)
	tjs_uint32 LastVBlankTick; //!< �Ō�� vblank �̎���

	bool Enabled;

	NativeEventQueue<tTVPVSyncTimingThread> EventQueue;

	class tTJSNI_Window* OwnerWindow;
public:
	tTVPVSyncTimingThread(class tTJSNI_Window* owner);
	~tTVPVSyncTimingThread();

protected:
	void Execute();
	void Proc( NativeEvent& ev );

public:
	void MeasureVSyncInterval(); // VSyncInterval ���v������
};
//---------------------------------------------------------------------------

#endif // __VSYNC_TIMING_THREAD_H__
