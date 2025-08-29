#define USE_TVP_APP 1

#if USE_TVP_APP
#include "../src/core/environ/cocos2d/AppDelegate.h"
#else
#include "../Classes/AppDelegate.h"
#endif

#include "cocos2d.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
#if USE_TVP_APP
	TVPAppDelegate app;
#else
    AppDelegate app;
#endif

    return Application::getInstance()->run();
}
