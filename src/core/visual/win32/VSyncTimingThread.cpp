
#include "tjsCommHead.h"

#include <mmsystem.h>

#include "VSyncTimingThread.h"
#include "WindowImpl.h"
#include "EventIntf.h"
#include "UserEvent.h"
#include "DebugIntf.h"
#include "MsgIntf.h"

//---------------------------------------------------------------------------
tTVPVSyncTimingThread::tTVPVSyncTimingThread(tTJSNI_Window* owner)
	 : tTVPThread(true), EventQueue(this,&tTVPVSyncTimingThread::Proc), OwnerWindow(owner)
{
	SleepTime = 1;
	LastVBlankTick = 0;
	VSyncInterval = 16; // ��60FPS
	Enabled = false;
	EventQueue.Allocate();
	MeasureVSyncInterval();
	Resume();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tTVPVSyncTimingThread::~tTVPVSyncTimingThread()
{
	Terminate();
	Resume();
	Event.Set();
	WaitFor();
	EventQueue.Deallocate();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPVSyncTimingThread::Execute()
{
	while(!GetTerminated())
	{
		// SleepTime �� LastVBlankTick �𓾂�
		DWORD sleep_time, last_vblank_tick;
		{	// thread-protected
			tTJSCriticalSectionHolder holder(CS);
			sleep_time = SleepTime;
			last_vblank_tick = LastVBlankTick;
		}

		// SleepTime ������
		// LastVBlankTick ����N�Z���ASleepTime ������
		DWORD sleep_start_tick = timeGetTime();

		DWORD sleep_time_adj = sleep_start_tick - last_vblank_tick;

		if(sleep_time_adj < sleep_time)
		{
			::Sleep(sleep_time - sleep_time_adj);
		}
		else
		{
			// ���ʁA���C���X���b�h���� Event.Set() �����Ȃ�΁A
			// �^�C���X���C�X(������10ms) ���I��鍠��
			// �����ɗ��Ă���͂��ł���B
			// sleep_time �͒ʏ� 10ms ��蒷���̂ŁA
			// �����ɗ�����Ăُ͈̂�B
			// ��قǃV�X�e�����d������ԂɂȂ��Ă�ƍl������B
			// �����ŗ��đ����� �C�x���g���|�X�g����킯�ɂ͂����Ȃ��̂�
			// �K���Ȏ���(�{���ɓK��) ����B
			::Sleep(5);
		}

		// �C�x���g���|�X�g����
		NativeEvent ev(TVP_EV_VSYNC_TIMING_THREAD);
		ev.LParam = (LPARAM)sleep_start_tick;
		EventQueue.PostEvent(ev);

		Event.WaitFor(0x7fffffff); // vsync �܂ő҂�
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPVSyncTimingThread::Proc( NativeEvent& ev )
{
	if(ev.Message != TVP_EV_VSYNC_TIMING_THREAD) {
		EventQueue.HandlerDefault(ev);
		return;
	}
	if( OwnerWindow == NULL ) return;

	// tTVPVSyncTimingThread ���瓊����ꂽ���b�Z�[�W

	tjs_int in_vblank = 0;
	tjs_int delayed = 0;
	bool supportvwait = OwnerWindow->WaitForVBlank( &in_vblank, &delayed );
	if( supportvwait == false )
	{	// VBlank�҂��̓T�|�[�g����Ă��Ȃ��̂ŁA�C�ɂ������̂܂ܐi�s(�҂����Ԃ͂������������C�ɂ��Ȃ����Ƃɂ���)
		in_vblank = 0;
		delayed = 0;
	}

	// �^�C�}�̎��Ԍ��_��ݒ肷��
	if(!delayed)
	{
		tTJSCriticalSectionHolder holder(CS);
		LastVBlankTick = timeGetTime(); // ���ꂪ���ɖ��鎞�Ԃ̋N�Z�_�ɂȂ�
	}
	else
	{
		tTJSCriticalSectionHolder holder(CS);
		LastVBlankTick += VSyncInterval; // ���ꂪ���ɖ��鎞�Ԃ̋N�Z�_�ɂȂ�(����������)
		if((long) (timeGetTime() - (LastVBlankTick + SleepTime)) <= 0)
		{
			// ��������A���ɋN���悤�Ƃ��鎞�Ԃ����łɉߋ��Ȃ̂Ŗ���܂���
			LastVBlankTick = timeGetTime(); // �����I�ɍ��̎����ɂ��܂�
		}
	}

	// ��ʂ̍X�V���s�� (DrawDevice��Show���\�b�h���Ă�)
	OwnerWindow->DeliverDrawDeviceShow();

	// ���� vsync �҂����s�����O�A���ł� vblank �ɓ����Ă����ꍇ�́A
	// �҂��Ԃ����������ƌ������Ƃł���
	if(in_vblank)
	{
		// ���̏ꍇ�� SleepTime �����炷
		tTJSCriticalSectionHolder holder(CS);
		if(SleepTime > 8) SleepTime --;
	}
	else
	{
		// vblank �Ŗ��������ꍇ�͓�̏ꍇ���l������
		// 1. vblank �O������
		// 2. vblank �ゾ����
		// �ǂ������͕�����Ȃ���
		// SleepTime �𑝂₷�B���������ꂪ VSyncInterval �𒴂���͂��͂Ȃ��B
		tTJSCriticalSectionHolder holder(CS);
		SleepTime ++;
		if(SleepTime > VSyncInterval) SleepTime = VSyncInterval;
	}

	// �^�C�}���N������
	Event.Set();

	// ContinuousHandler ���Ă�
	// ����͏\���Ȏ��Ԃ��Ƃ��悤�Avsync �҂��̒���ɌĂ΂��
	TVPProcessContinuousHandlerEventFlag = true; // set flag to invoke continuous handler on next idle
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tTVPVSyncTimingThread::MeasureVSyncInterval()
{
	TVPEnsureDirect3DObject();

	DWORD vsync_interval = 10000;
	DWORD vsync_rate = 0;

	HDC dc = ::GetDC(0);
	vsync_rate = ::GetDeviceCaps(dc, VREFRESH);
	::ReleaseDC(0, dc);

	if(vsync_rate != 0)
		vsync_interval = 1000 / vsync_rate;
	else
		vsync_interval = 0;

	TVPAddLog( TVPFormatMessage(TVPRoughVsyncIntervalReadFromApi,ttstr((int)vsync_interval)) );

	// vsync �����͓K�؂��ۂ��H
	if(vsync_interval < 6 || vsync_interval > 66)
	{
		TVPAddLog( (const tjs_char*)TVPRoughVsyncIntervalStillSeemsWrong );
		vsync_interval = 16;
	}

	VSyncInterval = vsync_interval;
}
//---------------------------------------------------------------------------
