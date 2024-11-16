//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//!@file 
//---------------------------------------------------------------------------
#ifndef DRAWDEVICE_H
#define DRAWDEVICE_H

#include "LayerIntf.h"
#include "LayerManager.h"
#include "ComplexRect.h"

class iTVPWindow;
class tTJSNI_BaseLayer;

/*[*/
//---------------------------------------------------------------------------
//! @brief		
//---------------------------------------------------------------------------
class iTVPDrawDevice
{
public:
	virtual void TJS_INTF_METHOD Destruct() = 0;

	virtual void TJS_INTF_METHOD SetWindowInterface(iTVPWindow * window) = 0;

	virtual void TJS_INTF_METHOD AddLayerManager(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD RemoveLayerManager(iTVPLayerManager * manager) = 0;

//	virtual void TJS_INTF_METHOD SetTargetWindow(int wnd, bool is_main) = 0;

	virtual void TJS_INTF_METHOD SetDestRectangle(const tTVPRect & rect) = 0;

	virtual void TJS_INTF_METHOD SetWindowSize(tjs_int w, tjs_int h) = 0;
	virtual void TJS_INTF_METHOD SetClipRectangle(const tTVPRect & rect) = 0;

	virtual void TJS_INTF_METHOD GetSrcSize(tjs_int &w, tjs_int &h) = 0;

	virtual void TJS_INTF_METHOD NotifyLayerResize(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD NotifyLayerImageChange(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD OnClick(tjs_int x, tjs_int y) = 0;

	virtual void TJS_INTF_METHOD OnDoubleClick(tjs_int x, tjs_int y) = 0;

	virtual void TJS_INTF_METHOD OnMouseDown(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags) = 0;

	virtual void TJS_INTF_METHOD OnMouseUp(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags) = 0;

	virtual void TJS_INTF_METHOD OnMouseMove(tjs_int x, tjs_int y, tjs_uint32 flags) = 0;

	virtual void TJS_INTF_METHOD OnReleaseCapture() = 0;

	virtual void TJS_INTF_METHOD OnMouseOutOfWindow() = 0;

	virtual void TJS_INTF_METHOD OnKeyDown(tjs_uint key, tjs_uint32 shift) = 0;

	virtual void TJS_INTF_METHOD OnKeyUp(tjs_uint key, tjs_uint32 shift) = 0;

	virtual void TJS_INTF_METHOD OnKeyPress(tjs_char key) = 0;

	virtual void TJS_INTF_METHOD OnMouseWheel(tjs_uint32 shift, tjs_int delta, tjs_int x, tjs_int y) = 0;

	virtual void TJS_INTF_METHOD OnTouchDown( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id ) = 0;

	virtual void TJS_INTF_METHOD OnTouchUp( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id ) = 0;

	virtual void TJS_INTF_METHOD OnTouchMove( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id ) = 0;

	virtual void TJS_INTF_METHOD OnTouchScaling( tjs_real startdist, tjs_real curdist, tjs_real cx, tjs_real cy, tjs_int flag ) = 0;

	virtual void TJS_INTF_METHOD OnTouchRotate( tjs_real startangle, tjs_real curangle, tjs_real dist, tjs_real cx, tjs_real cy, tjs_int flag ) = 0;

	virtual void TJS_INTF_METHOD OnMultiTouch() = 0;

	virtual void TJS_INTF_METHOD OnDisplayRotate( tjs_int orientation, tjs_int rotate, tjs_int bpp, tjs_int width, tjs_int height ) = 0;

	virtual void TJS_INTF_METHOD RecheckInputState() = 0;

	virtual void TJS_INTF_METHOD SetDefaultMouseCursor(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD SetMouseCursor(iTVPLayerManager * manager, tjs_int cursor) = 0;

	virtual void TJS_INTF_METHOD GetCursorPos(iTVPLayerManager * manager, tjs_int &x, tjs_int &y) = 0;

	virtual void TJS_INTF_METHOD SetCursorPos(iTVPLayerManager * manager, tjs_int x, tjs_int y) = 0;

	virtual void TJS_INTF_METHOD WindowReleaseCapture(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD SetHintText(iTVPLayerManager * manager, iTJSDispatch2* sender, const ttstr & text) = 0;

	virtual void TJS_INTF_METHOD SetAttentionPoint(iTVPLayerManager * manager, tTJSNI_BaseLayer *layer,
							tjs_int l, tjs_int t) = 0;

	virtual void TJS_INTF_METHOD DisableAttentionPoint(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD SetImeMode(iTVPLayerManager * manager, tTVPImeMode mode) = 0;

	virtual void TJS_INTF_METHOD ResetImeMode(iTVPLayerManager * manager) = 0;

	virtual tTJSNI_BaseLayer * TJS_INTF_METHOD GetPrimaryLayer() = 0;

	virtual tTJSNI_BaseLayer * TJS_INTF_METHOD GetFocusedLayer() = 0;

	virtual void TJS_INTF_METHOD SetFocusedLayer(tTJSNI_BaseLayer * layer) = 0;


	virtual void TJS_INTF_METHOD RequestInvalidation(const tTVPRect & rect) = 0;

	virtual void TJS_INTF_METHOD Update() = 0;

	virtual void TJS_INTF_METHOD Show() = 0;

	virtual void TJS_INTF_METHOD StartBitmapCompletion(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD NotifyBitmapCompleted(iTVPLayerManager * manager,
		tjs_int x, tjs_int y, tTVPBaseTexture * bmp,
		const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity) = 0;

	virtual void TJS_INTF_METHOD EndBitmapCompletion(iTVPLayerManager * manager) = 0;

	virtual void TJS_INTF_METHOD DumpLayerStructure() = 0;

	virtual void TJS_INTF_METHOD SetShowUpdateRect(bool b) = 0;
    virtual void Clear() {}

	virtual bool TJS_INTF_METHOD SwitchToFullScreen( int window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color, bool changeresolution ) = 0;
	
	virtual void TJS_INTF_METHOD RevertFromFullScreen(int window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color) = 0;

	virtual bool TJS_INTF_METHOD WaitForVBlank( tjs_int* in_vblank, tjs_int* delayed ) = 0;
};
//---------------------------------------------------------------------------
/*]*/

//---------------------------------------------------------------------------
//! @brief
//---------------------------------------------------------------------------
class tTVPDrawDevice : public iTVPDrawDevice
{
protected:
	iTVPWindow * Window;
	size_t PrimaryLayerManagerIndex; 
	std::vector<iTVPLayerManager *> Managers; 
	tTVPRect DestRect; 
    tjs_int SrcWidth, SrcHeight;
    tjs_int WinWidth, WinHeight;
	tjs_int LockedWidth/* = 0*/, LockedHeight/* = 0*/;
	tTVPRect ClipRect; 

protected:
	tTVPDrawDevice(); 
protected:
	virtual ~tTVPDrawDevice(); 

public:
	iTVPLayerManager * GetLayerManagerAt(size_t index)
	{
		if(Managers.size() <= index) return NULL;
		return Managers[index];
	}

	bool TransformToPrimaryLayerManager(tjs_int &x, tjs_int &y);
	bool TransformToPrimaryLayerManager(tjs_real &x, tjs_real &y);

	bool TransformFromPrimaryLayerManager(tjs_int &x, tjs_int &y);

	virtual void TJS_INTF_METHOD Destruct();

	virtual void TJS_INTF_METHOD SetWindowInterface(iTVPWindow * window);

	virtual void TJS_INTF_METHOD AddLayerManager(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD RemoveLayerManager(iTVPLayerManager * manager);

	virtual void TJS_INTF_METHOD SetDestRectangle(const tTVPRect & rect);
    virtual void TJS_INTF_METHOD SetWindowSize(tjs_int w, tjs_int h);
	virtual void TJS_INTF_METHOD SetClipRectangle(const tTVPRect & rect);
	virtual void TJS_INTF_METHOD GetSrcSize(tjs_int &w, tjs_int &h);
	virtual void TJS_INTF_METHOD NotifyLayerResize(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD NotifyLayerImageChange(iTVPLayerManager * manager);

	virtual void TJS_INTF_METHOD OnClick(tjs_int x, tjs_int y);
	virtual void TJS_INTF_METHOD OnDoubleClick(tjs_int x, tjs_int y);
	virtual void TJS_INTF_METHOD OnMouseDown(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnMouseUp(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnMouseMove(tjs_int x, tjs_int y, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnReleaseCapture();
	virtual void TJS_INTF_METHOD OnMouseOutOfWindow();
	virtual void TJS_INTF_METHOD OnKeyDown(tjs_uint key, tjs_uint32 shift);
	virtual void TJS_INTF_METHOD OnKeyUp(tjs_uint key, tjs_uint32 shift);
	virtual void TJS_INTF_METHOD OnKeyPress(tjs_char key);
	virtual void TJS_INTF_METHOD OnMouseWheel(tjs_uint32 shift, tjs_int delta, tjs_int x, tjs_int y);
	virtual void TJS_INTF_METHOD OnTouchDown( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id );
	virtual void TJS_INTF_METHOD OnTouchUp( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id );
	virtual void TJS_INTF_METHOD OnTouchMove( tjs_real x, tjs_real y, tjs_real cx, tjs_real cy, tjs_uint32 id );
	virtual void TJS_INTF_METHOD OnTouchScaling( tjs_real startdist, tjs_real curdist, tjs_real cx, tjs_real cy, tjs_int flag );
	virtual void TJS_INTF_METHOD OnTouchRotate( tjs_real startangle, tjs_real curangle, tjs_real dist, tjs_real cx, tjs_real cy, tjs_int flag );
	virtual void TJS_INTF_METHOD OnMultiTouch();
	virtual void TJS_INTF_METHOD OnDisplayRotate( tjs_int orientation, tjs_int rotate, tjs_int bpp, tjs_int width, tjs_int height );
	virtual void TJS_INTF_METHOD RecheckInputState();

	virtual void TJS_INTF_METHOD SetDefaultMouseCursor(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD SetMouseCursor(iTVPLayerManager * manager, tjs_int cursor);
	virtual void TJS_INTF_METHOD GetCursorPos(iTVPLayerManager * manager, tjs_int &x, tjs_int &y);
	virtual void TJS_INTF_METHOD SetCursorPos(iTVPLayerManager * manager, tjs_int x, tjs_int y);
	virtual void TJS_INTF_METHOD SetHintText(iTVPLayerManager * manager, iTJSDispatch2* sender, const ttstr & text);
	virtual void TJS_INTF_METHOD WindowReleaseCapture(iTVPLayerManager * manager);

	virtual void TJS_INTF_METHOD SetAttentionPoint(iTVPLayerManager * manager, tTJSNI_BaseLayer *layer,
							tjs_int l, tjs_int t);
	virtual void TJS_INTF_METHOD DisableAttentionPoint(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD SetImeMode(iTVPLayerManager * manager, tTVPImeMode mode);
	virtual void TJS_INTF_METHOD ResetImeMode(iTVPLayerManager * manager);

	virtual tTJSNI_BaseLayer * TJS_INTF_METHOD GetPrimaryLayer();
	virtual tTJSNI_BaseLayer * TJS_INTF_METHOD GetFocusedLayer();
	virtual void TJS_INTF_METHOD SetFocusedLayer(tTJSNI_BaseLayer * layer);

	virtual void TJS_INTF_METHOD RequestInvalidation(const tTVPRect & rect);
	virtual void TJS_INTF_METHOD Update();
	virtual void TJS_INTF_METHOD Show() = 0;
	virtual bool TJS_INTF_METHOD WaitForVBlank( tjs_int* in_vblank, tjs_int* delayed );

	virtual void TJS_INTF_METHOD DumpLayerStructure();
	virtual void TJS_INTF_METHOD SetShowUpdateRect(bool b);

	void SetLockedSize(tjs_int w, tjs_int h);
	virtual bool TJS_INTF_METHOD SwitchToFullScreen(int window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color, bool changeresolution);
	virtual void TJS_INTF_METHOD RevertFromFullScreen(int window, tjs_uint w, tjs_uint h, tjs_uint bpp, tjs_uint color);

};
//---------------------------------------------------------------------------
#endif
