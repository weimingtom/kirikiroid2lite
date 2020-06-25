#if 0
#include "main.h"
#include "../Classes/AppDelegate.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// create the application instance
	AppDelegate app;
	return Application::getInstance()->run();
}

#else
#include "main.h"
#include "../../src/core/environ/cocos2d/AppDelegate.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// create the application instance
	TVPAppDelegate app;
	return Application::getInstance()->run();
}

#endif