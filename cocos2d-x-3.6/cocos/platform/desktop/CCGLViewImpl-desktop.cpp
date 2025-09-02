/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCGLViewImpl-desktop.h"

#include <stdlib.h>
#if !defined(_MSC_VER)
#include <unistd.h>
#endif
#include <unordered_map>

#include "platform/CCApplication.h"
#include "base/CCDirector.h"
#include "base/CCTouch.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventMouse.h"
#include "base/CCIMEDispatcher.h"
#include "base/ccUtils.h"
#include "base/ccUTF8.h"

#define USE_SDL2 1
#if USE_SDL2
//SDL2, see SDL2/test/checkkeys.c and SDL2/test/loopwave.c
#include <SDL2/SDL.h>
SDL_Window *window;
int done;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    SDL_Quit();
    exit(rc);
}

static void
print_string(char **text, size_t *maxlen, const char *fmt, ...)
{
    int len;
    va_list ap;

    va_start(ap, fmt);
    len = SDL_vsnprintf(*text, *maxlen, fmt, ap);
    if (len > 0) {
        *text += len;
        if ( ((size_t) len) < *maxlen ) {
            *maxlen -= (size_t) len;
        } else {
            *maxlen = 0;
        }
    }
    va_end(ap);
}

static void
print_modifiers(char **text, size_t *maxlen)
{
    int mod;
    print_string(text, maxlen, " modifiers:");
    mod = SDL_GetModState();
    if (!mod) {
        print_string(text, maxlen, " (none)");
        return;
    }
    if (mod & KMOD_LSHIFT)
        print_string(text, maxlen, " LSHIFT");
    if (mod & KMOD_RSHIFT)
        print_string(text, maxlen, " RSHIFT");
    if (mod & KMOD_LCTRL)
        print_string(text, maxlen, " LCTRL");
    if (mod & KMOD_RCTRL)
        print_string(text, maxlen, " RCTRL");
    if (mod & KMOD_LALT)
        print_string(text, maxlen, " LALT");
    if (mod & KMOD_RALT)
        print_string(text, maxlen, " RALT");
    if (mod & KMOD_LGUI)
        print_string(text, maxlen, " LGUI");
    if (mod & KMOD_RGUI)
        print_string(text, maxlen, " RGUI");
    if (mod & KMOD_NUM)
        print_string(text, maxlen, " NUM");
    if (mod & KMOD_CAPS)
        print_string(text, maxlen, " CAPS");
    if (mod & KMOD_MODE)
        print_string(text, maxlen, " MODE");
}

static void
PrintModifierState()
{
    char message[512];
    char *spot;
    size_t left;

    spot = message;
    left = sizeof(message);

    print_modifiers(&spot, &left);
    SDL_Log("Initial state:%s\n", message);
}

static void
PrintKey(SDL_Keysym * sym, SDL_bool pressed, SDL_bool repeat)
{
    char message[512];
    char *spot;
    size_t left;

    spot = message;
    left = sizeof(message);

    /* Print the keycode, name and state */
    if (sym->sym) {
        print_string(&spot, &left,
                "Key %s:  scancode %d = %s, keycode 0x%08X = %s ",
                pressed ? "pressed " : "released",
                sym->scancode,
                SDL_GetScancodeName(sym->scancode),
                sym->sym, SDL_GetKeyName(sym->sym));
    } else {
        print_string(&spot, &left,
                "Unknown Key (scancode %d = %s) %s ",
                sym->scancode,
                SDL_GetScancodeName(sym->scancode),
                pressed ? "pressed " : "released");
    }
    print_modifiers(&spot, &left);
    if (repeat) {
        print_string(&spot, &left, " (repeat)");
    }
    SDL_Log("%s\n", message);
}

static void
PrintText(char *eventtype, char *text)
{
    char *spot, expanded[1024];

    expanded[0] = '\0';
    for ( spot = text; *spot; ++spot )
    {
        size_t length = SDL_strlen(expanded);
        SDL_snprintf(expanded + length, sizeof(expanded) - length, "\\x%.2x", (unsigned char)*spot);
    }
    SDL_Log("%s Text (%s): \"%s%s\"\n", eventtype, expanded, *text == '"' ? "\\" : "", text);
}

#endif

// <EGL/egl.h> exists since android 2.3
#include <EGL/egl.h>
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0;

//PFNGLMAPBUFFEROESPROC glMapBufferOES = 0;
//PFNGLUNMAPBUFFEROESPROC glUnmapBufferOES = 0;
//PFNGLGETBUFFERPOINTERVOESPROC glGetBufferPointervOES = 0;

void initExtensions() {
     glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
     glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
     glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");


 //   glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
 //   glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
 //   glGetBufferPointervOES = (PFNGLGETBUFFERPOINTERVOESPROC)eglGetProcAddress("glGetBufferPointervOES");
}


NS_CC_BEGIN

// GLFWEventHandler

#if USE_NO_GLFW

#else
class GLFWEventHandler
{
public:
    static void onGLFWError(int errorID, const char* errorDesc)
    {
        if (_view)
            _view->onGLFWError(errorID, errorDesc);
    }

    static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
    {
        if (_view)
            _view->onGLFWMouseCallBack(window, button, action, modify);
    }

    static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseMoveCallBack(window, x, y);
    }

    static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseScrollCallback(window, x, y);
    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (_view)
            _view->onGLFWKeyCallback(window, key, scancode, action, mods);
    }

    static void onGLFWCharCallback(GLFWwindow* window, unsigned int character)
    {
        if (_view)
            _view->onGLFWCharCallback(window, character);
    }

    static void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
    {
        if (_view)
            _view->onGLFWWindowPosCallback(windows, x, y);
    }

    static void onGLFWframebuffersize(GLFWwindow* window, int w, int h)
    {
        if (_view)
            _view->onGLFWframebuffersize(window, w, h);
    }

    static void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
    {
        if (_view)
            _view->onGLFWWindowSizeFunCallback(window, width, height);
    }

    static void setGLViewImpl(GLViewImpl* view)
    {
        _view = view;
    }

    static void onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified)
    {
        if (_view)
        {
            _view->onGLFWWindowIconifyCallback(window, iconified);
        }
    }

private:
    static GLViewImpl* _view;
};

GLViewImpl* GLFWEventHandler::_view = nullptr;

////////////////////////////////////////////////////

struct keyCodeItem
{
    int glfwKeyCode;
    EventKeyboard::KeyCode keyCode;
};

static std::unordered_map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
    /* The unknown key */
    { GLFW_KEY_UNKNOWN         , EventKeyboard::KeyCode::KEY_NONE          },

    /* Printable keys */
    { GLFW_KEY_SPACE           , EventKeyboard::KeyCode::KEY_SPACE         },
    { GLFW_KEY_APOSTROPHE      , EventKeyboard::KeyCode::KEY_APOSTROPHE    },
    { GLFW_KEY_COMMA           , EventKeyboard::KeyCode::KEY_COMMA         },
    { GLFW_KEY_MINUS           , EventKeyboard::KeyCode::KEY_MINUS         },
    { GLFW_KEY_PERIOD          , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_SLASH           , EventKeyboard::KeyCode::KEY_SLASH         },
    { GLFW_KEY_0               , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_1               , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_2               , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_3               , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_4               , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_5               , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_6               , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_7               , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_8               , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_9               , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_SEMICOLON       , EventKeyboard::KeyCode::KEY_SEMICOLON     },
    { GLFW_KEY_EQUAL           , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_A               , EventKeyboard::KeyCode::KEY_A             },
    { GLFW_KEY_B               , EventKeyboard::KeyCode::KEY_B             },
    { GLFW_KEY_C               , EventKeyboard::KeyCode::KEY_C             },
    { GLFW_KEY_D               , EventKeyboard::KeyCode::KEY_D             },
    { GLFW_KEY_E               , EventKeyboard::KeyCode::KEY_E             },
    { GLFW_KEY_F               , EventKeyboard::KeyCode::KEY_F             },
    { GLFW_KEY_G               , EventKeyboard::KeyCode::KEY_G             },
    { GLFW_KEY_H               , EventKeyboard::KeyCode::KEY_H             },
    { GLFW_KEY_I               , EventKeyboard::KeyCode::KEY_I             },
    { GLFW_KEY_J               , EventKeyboard::KeyCode::KEY_J             },
    { GLFW_KEY_K               , EventKeyboard::KeyCode::KEY_K             },
    { GLFW_KEY_L               , EventKeyboard::KeyCode::KEY_L             },
    { GLFW_KEY_M               , EventKeyboard::KeyCode::KEY_M             },
    { GLFW_KEY_N               , EventKeyboard::KeyCode::KEY_N             },
    { GLFW_KEY_O               , EventKeyboard::KeyCode::KEY_O             },
    { GLFW_KEY_P               , EventKeyboard::KeyCode::KEY_P             },
    { GLFW_KEY_Q               , EventKeyboard::KeyCode::KEY_Q             },
    { GLFW_KEY_R               , EventKeyboard::KeyCode::KEY_R             },
    { GLFW_KEY_S               , EventKeyboard::KeyCode::KEY_S             },
    { GLFW_KEY_T               , EventKeyboard::KeyCode::KEY_T             },
    { GLFW_KEY_U               , EventKeyboard::KeyCode::KEY_U             },
    { GLFW_KEY_V               , EventKeyboard::KeyCode::KEY_V             },
    { GLFW_KEY_W               , EventKeyboard::KeyCode::KEY_W             },
    { GLFW_KEY_X               , EventKeyboard::KeyCode::KEY_X             },
    { GLFW_KEY_Y               , EventKeyboard::KeyCode::KEY_Y             },
    { GLFW_KEY_Z               , EventKeyboard::KeyCode::KEY_Z             },
    { GLFW_KEY_LEFT_BRACKET    , EventKeyboard::KeyCode::KEY_LEFT_BRACKET  },
    { GLFW_KEY_BACKSLASH       , EventKeyboard::KeyCode::KEY_BACK_SLASH    },
    { GLFW_KEY_RIGHT_BRACKET   , EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
    { GLFW_KEY_GRAVE_ACCENT    , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_1         , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_2         , EventKeyboard::KeyCode::KEY_NONE          },

    /* Function keys */
    { GLFW_KEY_ESCAPE          , EventKeyboard::KeyCode::KEY_ESCAPE        },
    { GLFW_KEY_ENTER           , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_TAB             , EventKeyboard::KeyCode::KEY_TAB           },
    { GLFW_KEY_BACKSPACE       , EventKeyboard::KeyCode::KEY_BACKSPACE     },
    { GLFW_KEY_INSERT          , EventKeyboard::KeyCode::KEY_INSERT        },
    { GLFW_KEY_DELETE          , EventKeyboard::KeyCode::KEY_DELETE        },
    { GLFW_KEY_RIGHT           , EventKeyboard::KeyCode::KEY_RIGHT_ARROW   },
    { GLFW_KEY_LEFT            , EventKeyboard::KeyCode::KEY_LEFT_ARROW    },
    { GLFW_KEY_DOWN            , EventKeyboard::KeyCode::KEY_DOWN_ARROW    },
    { GLFW_KEY_UP              , EventKeyboard::KeyCode::KEY_UP_ARROW      },
    { GLFW_KEY_PAGE_UP         , EventKeyboard::KeyCode::KEY_KP_PG_UP      },
    { GLFW_KEY_PAGE_DOWN       , EventKeyboard::KeyCode::KEY_KP_PG_DOWN    },
    { GLFW_KEY_HOME            , EventKeyboard::KeyCode::KEY_KP_HOME       },
    { GLFW_KEY_END             , EventKeyboard::KeyCode::KEY_END           },
    { GLFW_KEY_CAPS_LOCK       , EventKeyboard::KeyCode::KEY_CAPS_LOCK     },
    { GLFW_KEY_SCROLL_LOCK     , EventKeyboard::KeyCode::KEY_SCROLL_LOCK   },
    { GLFW_KEY_NUM_LOCK        , EventKeyboard::KeyCode::KEY_NUM_LOCK      },
    { GLFW_KEY_PRINT_SCREEN    , EventKeyboard::KeyCode::KEY_PRINT         },
    { GLFW_KEY_PAUSE           , EventKeyboard::KeyCode::KEY_PAUSE         },
    { GLFW_KEY_F1              , EventKeyboard::KeyCode::KEY_F1            },
    { GLFW_KEY_F2              , EventKeyboard::KeyCode::KEY_F2            },
    { GLFW_KEY_F3              , EventKeyboard::KeyCode::KEY_F3            },
    { GLFW_KEY_F4              , EventKeyboard::KeyCode::KEY_F4            },
    { GLFW_KEY_F5              , EventKeyboard::KeyCode::KEY_F5            },
    { GLFW_KEY_F6              , EventKeyboard::KeyCode::KEY_F6            },
    { GLFW_KEY_F7              , EventKeyboard::KeyCode::KEY_F7            },
    { GLFW_KEY_F8              , EventKeyboard::KeyCode::KEY_F8            },
    { GLFW_KEY_F9              , EventKeyboard::KeyCode::KEY_F9            },
    { GLFW_KEY_F10             , EventKeyboard::KeyCode::KEY_F10           },
    { GLFW_KEY_F11             , EventKeyboard::KeyCode::KEY_F11           },
    { GLFW_KEY_F12             , EventKeyboard::KeyCode::KEY_F12           },
    { GLFW_KEY_F13             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F14             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F15             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F16             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F17             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F18             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F19             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F20             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F21             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F22             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F23             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F24             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F25             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_KP_0            , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_KP_1            , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_KP_2            , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_KP_3            , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_KP_4            , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_KP_5            , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_KP_6            , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_KP_7            , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_KP_8            , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_KP_9            , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_KP_DECIMAL      , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_KP_DIVIDE       , EventKeyboard::KeyCode::KEY_KP_DIVIDE     },
    { GLFW_KEY_KP_MULTIPLY     , EventKeyboard::KeyCode::KEY_KP_MULTIPLY   },
    { GLFW_KEY_KP_SUBTRACT     , EventKeyboard::KeyCode::KEY_KP_MINUS      },
    { GLFW_KEY_KP_ADD          , EventKeyboard::KeyCode::KEY_KP_PLUS       },
    { GLFW_KEY_KP_ENTER        , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_KP_EQUAL        , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_LEFT_SHIFT      , EventKeyboard::KeyCode::KEY_LEFT_SHIFT         },
    { GLFW_KEY_LEFT_CONTROL    , EventKeyboard::KeyCode::KEY_LEFT_CTRL          },
    { GLFW_KEY_LEFT_ALT        , EventKeyboard::KeyCode::KEY_LEFT_ALT           },
    { GLFW_KEY_LEFT_SUPER      , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_RIGHT_SHIFT     , EventKeyboard::KeyCode::KEY_RIGHT_SHIFT         },
    { GLFW_KEY_RIGHT_CONTROL   , EventKeyboard::KeyCode::KEY_RIGHT_CTRL          },
    { GLFW_KEY_RIGHT_ALT       , EventKeyboard::KeyCode::KEY_RIGHT_ALT           },
    { GLFW_KEY_RIGHT_SUPER     , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_MENU            , EventKeyboard::KeyCode::KEY_MENU          },
    { GLFW_KEY_LAST            , EventKeyboard::KeyCode::KEY_NONE          }
};
#endif

//////////////////////////////////////////////////////////////////////////
// implement GLViewImpl
//////////////////////////////////////////////////////////////////////////


GLViewImpl::GLViewImpl()
: _captured(false)
, _supportTouch(false)
, _isInRetinaMonitor(false)
, _isRetinaEnabled(false)
, _retinaFactor(1)
, _frameZoomFactor(1.0f)
#if USE_NO_GLFW

#else
, _mainWindow(nullptr)
, _monitor(nullptr)
#endif
, _mouseX(0.0f)
, _mouseY(0.0f)
{
initExtensions();
    _viewName = "cocos2dx";
#if USE_NO_GLFW

#else	
    g_keyCodeMap.clear();
    //for (auto& item : g_keyCodeStructArray)
    for (int i = 0; i < sizeof(g_keyCodeStructArray) / sizeof(g_keyCodeStructArray[0]); ++i)
	{
		const auto& item = g_keyCodeStructArray[i];
        g_keyCodeMap[item.glfwKeyCode] = item.keyCode;
    }

    GLFWEventHandler::setGLViewImpl(this);

    glfwSetErrorCallback(GLFWEventHandler::onGLFWError);
    glfwInit();
#endif

#if USE_SDL2
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Load the SDL library */
    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER ) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
        quit(1);
    }

//------testgamecontroller.c
    int i;
    int nController = 0;
    int retcode = 0;
    char guid[64];
    SDL_GameController *gamecontroller;
    /* Print information about the controller */
    for (i = 0; i < SDL_NumJoysticks(); ++i) {
        const char *name;
        const char *description;

        SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i),
                                  guid, sizeof (guid));

        if ( SDL_IsGameController(i) )
        {
            nController++;
            name = SDL_GameControllerNameForIndex(i);
            description = "Controller";
        } else {
            name = SDL_JoystickNameForIndex(i);
            description = "Joystick";
        }
        SDL_Log("%s %d: %s (guid %s, VID 0x%.4x, PID 0x%.4x)\n",
            description, i, name ? name : "Unknown", guid,
            SDL_JoystickGetDeviceVendor(i), SDL_JoystickGetDeviceProduct(i));
    }
    SDL_Log("There are %d game controller(s) attached (%d joystick(s))\n", nController, SDL_NumJoysticks());


char *argv1 = "0";
 if (argv1) {
        SDL_bool reportederror = SDL_FALSE;
        SDL_bool keepGoing = SDL_TRUE;
        SDL_Event event;
        int device = atoi(argv1);
        if (device >= SDL_NumJoysticks()) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%i is an invalid joystick index.\n", device);
            retcode = 1;
        } else {
            SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(device),
                                      guid, sizeof (guid));
            SDL_Log("Attempting to open device %i, guid %s\n", device, guid);
            gamecontroller = SDL_GameControllerOpen(device);

            if (gamecontroller != NULL) {
                SDL_assert(SDL_GameControllerFromInstanceID(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gamecontroller))) == gamecontroller);
            }

        }
    }

//getchar();


//------testgamecontroller.c

#if !USE_NO_GLFW
    window = SDL_CreateWindow("SDL2 Joystick capture",//"CheckKeys Test",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              0, 0/*640, 480*/, SDL_WINDOW_INPUT_FOCUS/*SDL_WINDOW_INPUT_GRABBED*/ /*SDL_WINDOW_HIDDEN*/);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create 640x480 window: %s\n",
                SDL_GetError());
        quit(2);
    }
#else
//FIXME:on linux handheld, SDL window by SDL_CreateWindow() will be 
//empty and transparent, and no video output, 
//so I must skip this here. 
//But on X11, SDL_CreateWindow is OK.  
#endif

#endif
}

GLViewImpl::~GLViewImpl()
{
    CCLOGINFO("deallocing GLViewImpl: %p", this);
#if USE_NO_GLFW

#else	
    GLFWEventHandler::setGLViewImpl(nullptr);
    glfwTerminate();
#endif	
}

GLViewImpl* GLViewImpl::create(const std::string& viewName)
{
    auto ret = new (std::nothrow) GLViewImpl;
#if 0 //defined(USE_APP_WIDTH) && defined(USE_APP_HEIGHT)
    if(ret && ret->initWithRect(viewName, Rect(0, 0, USE_APP_WIDTH, USE_APP_HEIGHT), 1)) {
#else      
    if(ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1)) {
    //if(ret && ret->initWithRect(viewName, Rect(0, 0, 640, 480), 1)) {
#endif    
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

GLViewImpl* GLViewImpl::createWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{
    auto ret = new (std::nothrow) GLViewImpl;
    if(ret && ret->initWithRect(viewName, rect, frameZoomFactor)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

#if USE_NO_GLFW
#else
GLViewImpl* GLViewImpl::createWithFullScreen(const std::string& viewName)
{
    auto ret = new (std::nothrow) GLViewImpl();
    if(ret && ret->initWithFullScreen(viewName)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

GLViewImpl* GLViewImpl::createWithFullScreen(const std::string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    auto ret = new (std::nothrow) GLViewImpl();
    if(ret && ret->initWithFullscreen(viewName, videoMode, monitor)) {
        ret->autorelease();
        return ret;
    }
    
    return nullptr;
}
#endif

#if USE_NO_GLFW
EGLDisplay display = EGL_NO_DISPLAY;
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;


GLfloat vVertices[] = {
 0.0f,  0.5f, 0.0f,
-0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f
};
#endif

bool GLViewImpl::initWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{
    setViewName(viewName);

    _frameZoomFactor = frameZoomFactor;

#if USE_NO_GLFW
	EGLint egl_major = 0;
    EGLint egl_minor = 0;
    EGLint num_configs = 0;
    EGLConfig configs = {0};
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,   8,//5,  
        EGL_GREEN_SIZE, 8,//6,
        EGL_BLUE_SIZE,  8,//5,  
        EGL_ALPHA_SIZE, 8,//0,
        EGL_NONE
    };
/*
    glfwWindowHint(GLFW_RED_BITS,_glContextAttrs.redBits);
    glfwWindowHint(GLFW_GREEN_BITS,_glContextAttrs.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS,_glContextAttrs.blueBits);
    glfwWindowHint(GLFW_ALPHA_BITS,_glContextAttrs.alphaBits);
    glfwWindowHint(GLFW_DEPTH_BITS,_glContextAttrs.depthBits);
    glfwWindowHint(GLFW_STENCIL_BITS,_glContextAttrs.stencilBits);
*/	
    EGLint window_attributes[] = { 
        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        EGL_NONE
    };
    EGLint const context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
    };
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, &egl_major, &egl_minor);
    eglChooseConfig(display, config_attribs, &configs, 1, &num_configs);
    surface = eglCreateWindowSurface(display, configs, 0, window_attributes);
    context = eglCreateContext(display, configs, EGL_NO_CONTEXT, context_attributes);
    eglMakeCurrent(display, surface, surface, context);
	
const char *vShaderSrc =
    "attribute vec4 vPosition;  \n"
    "void main()                \n"
    "{                          \n"
    "  gl_Position = vPosition; \n"
    "}                          \n";
 
const char *fShaderSrc =
    "precision mediump float;                       \n"
    "void main()                                    \n"
    "{                                              \n"
    "   gl_FragColor = vec4(0.7, 0.85, 0.97, 1.0);    \n"
    "}                                              \n"; 

    GLuint vShader = 0;
    GLuint fShader = 0;
    GLuint pObject = 0;
    GLint compiled = 0;
    
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSrc, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
 
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSrc, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
  
    pObject = glCreateProgram();
    glAttachShader(pObject, vShader);
    glAttachShader(pObject, fShader);
    glLinkProgram(pObject);
    glUseProgram(pObject);

#if 0
#define LCD_W   480
#define LCD_H   640
    glViewport(0, 0, LCD_W, LCD_H);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(display, surface);
CCLOGINFO("setFrameSize %f, %f", width, height);

//while(1) {
    glViewport(0, 0, LCD_W, LCD_H);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    eglSwapBuffers(display, surface);
    sleep(3);
//}
#endif























#else
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_RED_BITS,_glContextAttrs.redBits);
    glfwWindowHint(GLFW_GREEN_BITS,_glContextAttrs.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS,_glContextAttrs.blueBits);
    glfwWindowHint(GLFW_ALPHA_BITS,_glContextAttrs.alphaBits);
    glfwWindowHint(GLFW_DEPTH_BITS,_glContextAttrs.depthBits);
    glfwWindowHint(GLFW_STENCIL_BITS,_glContextAttrs.stencilBits);

    _mainWindow = glfwCreateWindow(rect.size.width * _frameZoomFactor,
                                   rect.size.height * _frameZoomFactor,
                                   _viewName.c_str(),
                                   _monitor,
                                   nullptr);
glfwSetWindowPos(_mainWindow, 0, 0); //FIXME:added, show on left top of the screen
    glfwMakeContextCurrent(_mainWindow);

    glfwSetMouseButtonCallback(_mainWindow, GLFWEventHandler::onGLFWMouseCallBack);
    glfwSetCursorPosCallback(_mainWindow, GLFWEventHandler::onGLFWMouseMoveCallBack);
    glfwSetScrollCallback(_mainWindow, GLFWEventHandler::onGLFWMouseScrollCallback);
    glfwSetCharCallback(_mainWindow, GLFWEventHandler::onGLFWCharCallback);
    glfwSetKeyCallback(_mainWindow, GLFWEventHandler::onGLFWKeyCallback);
    glfwSetWindowPosCallback(_mainWindow, GLFWEventHandler::onGLFWWindowPosCallback);
    glfwSetFramebufferSizeCallback(_mainWindow, GLFWEventHandler::onGLFWframebuffersize);
    glfwSetWindowSizeCallback(_mainWindow, GLFWEventHandler::onGLFWWindowSizeFunCallback);
    glfwSetWindowIconifyCallback(_mainWindow, GLFWEventHandler::onGLFWWindowIconifyCallback);
#endif
	
    setFrameSize(rect.size.width, rect.size.height);

#if USE_NO_GLFW
//OpenGL version too old: OpenGL 1.5 or higher is required (your version is OpenGL ES 3.0 Mesa 18.0.5). Please upgrade the driver of your video card.
#else
    // check OpenGL version at first
    const GLubyte* glVersion = glGetString(GL_VERSION);

    if ( utils::atof((const char*)glVersion) < 1.5 )
    {
        char strComplain[256] = {0};
        sprintf(strComplain,
                "OpenGL 1.5 or higher is required (your version is %s). Please upgrade the driver of your video card.",
                glVersion);
        MessageBox(strComplain, "OpenGL version too old");
        return false;
    }
#endif

    initGlew();

#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642 
#endif

    // Enable point size by default.
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    return true;
}

bool GLViewImpl::initWithFullScreen(const std::string& viewName)
{
    //Create fullscreen window on primary monitor at its current video mode.
#if USE_NO_GLFW
#if defined(USE_APP_WIDTH) && defined(USE_APP_HEIGHT)
        return initWithRect(viewName, Rect(0, 0, USE_APP_WIDTH, USE_APP_HEIGHT), 1.0f);
#else 
	return initWithRect(viewName, Rect(0, 0, 640, 480), 1.0f);
#endif
#else	
    _monitor = glfwGetPrimaryMonitor();
    if (nullptr == _monitor)
        return false;

    const GLFWvidmode* videoMode = glfwGetVideoMode(_monitor);
    return initWithRect(viewName, Rect(0, 0, videoMode->width, videoMode->height), 1.0f);
#endif
}

#if USE_NO_GLFW

#else
bool GLViewImpl::initWithFullscreen(const std::string &viewname, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    //Create fullscreen on specified monitor at the specified video mode.
    _monitor = monitor;
    if (nullptr == _monitor)
        return false;
    
    //These are soft contraints. If the video mode is retrieved at runtime, the resulting window and context should match these exactly. If invalid attribs are passed (eg. from an outdated cache), window creation will NOT fail but the actual window/context may differ.
    glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);
    glfwWindowHint(GLFW_RED_BITS, videoMode.redBits);
    glfwWindowHint(GLFW_BLUE_BITS, videoMode.blueBits);
    glfwWindowHint(GLFW_GREEN_BITS, videoMode.greenBits);
    
    return initWithRect(viewname, Rect(0, 0, videoMode.width, videoMode.height), 1.0f);
}
#endif

bool _ready = true;

bool GLViewImpl::isOpenGLReady()
{
#if USE_NO_GLFW
    return true;
#else
    return nullptr != _mainWindow;
#endif
}

void GLViewImpl::end()
{
#if USE_NO_GLFW
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
#else
    if(_mainWindow)
    {
        glfwSetWindowShouldClose(_mainWindow,1);
        _mainWindow = nullptr;
    }
#endif	
    // Release self. Otherwise, GLViewImpl could not be freed.
    release();
}

void GLViewImpl::swapBuffers()
{
CCLOGINFO("swapBuffers");
#if USE_NO_GLFW	
//    glViewport(0, 0, LCD_W, LCD_H);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
//    glEnableVertexAttribArray(0);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    if (display && surface) {
	eglSwapBuffers(display, surface);
    }
#else		
    if(_mainWindow) {
        glfwSwapBuffers(_mainWindow);
    }
#endif	
}

bool GLViewImpl::windowShouldClose()
{
#if USE_NO_GLFW	
	return false;
#else	
    if(_mainWindow)
        return glfwWindowShouldClose(_mainWindow) ? true : false;
    else
        return true;
#endif	
}

static void convertTo(cocos2d::EventKeyboard::KeyCode key, bool press) {
        EventKeyboard event(key, press);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}

void GLViewImpl::pollEvents()
{
#if USE_NO_GLFW	
//skip???
#else	
    glfwPollEvents();
#endif

#if USE_SDL2
    SDL_Event event;
    /* Check for events */
    /*SDL_WaitEvent(&event); emscripten does not like waiting*/

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP: 
	{
            PrintKey(&event.key.keysym, (event.key.state == SDL_PRESSED) ? SDL_TRUE : SDL_FALSE, (event.key.repeat) ? SDL_TRUE : SDL_FALSE);

#if USE_NO_GLFW	
#else
//FIXME: added, to escape for PC
if (event.key.keysym.sym == SDLK_ESCAPE)
{
	exit(0);
}
#endif

//miyoo a30 key translate:
//A=44=SPACE
//B=224/0x400000E0=LCTRL
//Y=226/0x400000E2=LALT
//X=225/0x400000E1=LSHIFT
//Left/0x40000050=80
//Right/0x4000004F=79
//Up/0x40000052=82
//Down/0x40000051=81
//L1=43=TAB
//L2=8=E
//R1=42=BACKSPACE
//R2=23=T
//Vol-=129/0x40000081=VOLUMEDOWN
//Vol+=128/0x40000080=VOLUMEUP
//Menu=27/(41?)/0x0000001B=ESCAPE
//Select=228/0x400000E4=RCTRL
//Start=40=RETURN
//Power=102/0x40000066=POWER

//SDL_Keysym:
//SDL_ScanCode scancode = event.key.keysym.scancode;
SDL_Keycode key = event.key.keysym.sym;

/*UP*/if (key == 82 || key == 0x40000052) convertTo(EventKeyboard::KeyCode::KEY_TAB/*KEY_UP_ARROW*/, event.key.state == SDL_PRESSED);
/*DOWN*/if (key == 81 || key == 0x40000051) convertTo(EventKeyboard::KeyCode::KEY_TAB/*KEY_DOWN_ARROW*/, event.key.state == SDL_PRESSED);
/*LEFT*/if (key == 80 || key == 0x40000050) convertTo(EventKeyboard::KeyCode::KEY_LEFT_ARROW, event.key.state == SDL_PRESSED);
/*RIGHT*/if (key == 79 || key == 0x4000004F) convertTo(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, event.key.state == SDL_PRESSED);

/*A*/if (key == 44) return convertTo(EventKeyboard::KeyCode::KEY_RETURN, event.key.state == SDL_PRESSED);
/*B*/if (key == 224 || key == 0x400000E0) convertTo(EventKeyboard::KeyCode::KEY_SPACE, event.key.state == SDL_PRESSED);
/*X*/if (key == 225 || key == 0x400000E1) convertTo(EventKeyboard::KeyCode::KEY_R, event.key.state == SDL_PRESSED); //history log
/*Y*/if (key == 226 || key == 0x400000E2) convertTo(EventKeyboard::KeyCode::KEY_ESCAPE, event.key.state == SDL_PRESSED); //game menu //or 

/*L1*/if (key == 43) convertTo(EventKeyboard::KeyCode::KEY_F, event.key.state == SDL_PRESSED); //auto fast forward
/*R1*/if (key == 42) convertTo(EventKeyboard::KeyCode::KEY_CTRL, event.key.state == SDL_PRESSED); //manual fast forward, not good
/*Start*/if (key == 40) convertTo(EventKeyboard::KeyCode::KEY_A, event.key.state == SDL_PRESSED); //auto not good
// /*Select*/if (key == 228 || key == 0x400000E4) return ?;
/*Menu*/if (key == 27 || key == 41 || key == 0x0000001B) exit(0);
	}
        break;
        
	case SDL_TEXTEDITING:
            PrintText("EDIT", event.text.text);
            break;
        case SDL_TEXTINPUT:
            PrintText("INPUT", event.text.text);
            break;
        case SDL_MOUSEBUTTONDOWN:
            /* Left button quits the app, other buttons toggles text input */
            if (event.button.button == SDL_BUTTON_LEFT) {
                done = 1;
            } else {
                if (SDL_IsTextInputActive()) {
                    SDL_Log("Stopping text input\n");
                    SDL_StopTextInput();
                } else {
                    SDL_Log("Starting text input\n");
                    SDL_StartTextInput();
                }
            }
            break;
        case SDL_QUIT:
            done = 1;
            break;



	case SDL_CONTROLLERBUTTONDOWN:
	case SDL_CONTROLLERBUTTONUP: {
#if 1//defined(USE_DEBUG_INPUT)
SDL_Log("%s, %s, %d\n", 
(event.type == SDL_CONTROLLERBUTTONDOWN) ? 
"SDL_CONTROLLERBUTTONDOWN" : 
"SDL_CONTROLLERBUTTONUP", 
(event.cbutton.state == SDL_PRESSED) ? 
"SDL_PRESSED" : 
"SDL_RELEASED", 
event.cbutton.button);
#endif				
		switch (event.cbutton.state) {
			case SDL_PRESSED:
#if 1						
				//TVPPostInputEvent(new tTVPOnKeyDownInputEvent(TJSNativeInstance, sdl_gamecontrollerbutton_to_vk_key(event.cbutton.button, &isQuit, s, 0, TJSNativeInstance), s));
#endif

//trimui smart pro:
//UP:11
//DOWN:12
//LEFT:13
//RIGHT:14
//A:1(joy 1, only up)
//B:0(joy 0)
//X:3(joy 3)
//Y:2(joy 2)
//L1:9(Joy 4)
//L2:10(Joy 5)
//Start:6(Joy 7)
//Select:4(Joy 6)
//Menu:5(Joy 8)
//joy: joyaxismotion
//key: Power(102, VolumeUp(128), VolumeDown(129)
//===
//plan:
//A=return=kag3.Return
//B=space=kag3.Space
//X=fast=kag3.F(#'F')
//Y=menu=kag3.ESC=message history
//
//Select=????
//Start=auto=kag3.A
//Menu=exit=???kage3.exit
//
//L1=left?=?kag3.B=back?
//R1=right?
//
//?=kage3.S=save?
//?=kage3.L=load?

if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_UP_ARROW, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_DOWN_ARROW, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_LEFT_ARROW, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}

if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_RETURN, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_SPACE, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_R, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_ESCAPE, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_A, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_TAB, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 9)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_F, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 10)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_CTRL, true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 5)
{
exit(0);//Menu
}

				break;
				
			case SDL_RELEASED:
#if 1
				if (!SDL_IsTextInputActive())
				{
					//TVPPostInputEvent(new tTVPOnKeyPressInputEvent(TJSNativeInstance, sdl_gamecontrollerbutton_to_vk_key(event.cbutton.button, &isQuit, s, 0, TJSNativeInstance)));
				}						
				//TVPPostInputEvent(new tTVPOnKeyUpInputEvent(TJSNativeInstance, sdl_gamecontrollerbutton_to_vk_key(event.cbutton.button, &isQuit, s, 1, TJSNativeInstance), s));
#else
//sdl_gamecontrollerbutton_to_vk_key(event.cbutton.button, &isQuit, s, 1, TJSNativeInstance);
#endif								
				//if (isQuit) {
					//TVPPostInputEvent(new tTVPOnCloseInputEvent(TJSNativeInstance));
				//}




if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_UP_ARROW, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_DOWN_ARROW, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_LEFT_ARROW, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_RIGHT_ARROW, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}

if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_RETURN, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_SPACE, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_R, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_ESCAPE, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_A, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_TAB, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 9)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_F, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 10)
{
        EventKeyboard event(EventKeyboard::KeyCode::KEY_CTRL, false);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
}
if (event.cbutton.button == 5)
{
exit(0);//Menu
}

				break;
		}
	}
	break;
	
	case SDL_JOYBUTTONDOWN:
		//SDL_Log("%s, %d\n", "SDL_JOYBUTTONDOWN", event.jbutton.button);
		break;
 
	case SDL_JOYBUTTONUP:
		//SDL_Log("%s, %d\n", "SDL_JOYBUTTONUP", event.jbutton.button);
		break;

	case SDL_JOYAXISMOTION:
		//SDL_Log("%s, %d\n", "SDL_JOYAXISMOTION");
		break;

        default:
            break;
        }
    }
#endif
}

void GLViewImpl::enableRetina(bool enabled)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    _isRetinaEnabled = enabled;
    if (_isRetinaEnabled)
    {
        _retinaFactor = 1;
    }
    else
    {
        _retinaFactor = 2;
    }
    updateFrameSize();
#endif
}


void GLViewImpl::setIMEKeyboardState(bool /*bOpen*/)
{

}

void GLViewImpl::setCursorVisible( bool isVisible )
{
#if USE_NO_GLFW

#else		
    if( _mainWindow == NULL )
        return;
    
    if( isVisible )
        glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif	
}

void GLViewImpl::setFrameZoomFactor(float zoomFactor)
{
    CCASSERT(zoomFactor > 0.0f, "zoomFactor must be larger than 0");

    if (fabs(_frameZoomFactor - zoomFactor) < FLT_EPSILON)
    {
        return;
    }

    _frameZoomFactor = zoomFactor;
    updateFrameSize();
}

float GLViewImpl::getFrameZoomFactor() const
{
    return _frameZoomFactor;
}

void GLViewImpl::updateFrameSize()
{
#if USE_NO_GLFW	
//skip???
#else	
    if (_screenSize.width > 0 && _screenSize.height > 0)
    {
        int w = 0, h = 0;
        glfwGetWindowSize(_mainWindow, &w, &h);

        int frameBufferW = 0, frameBufferH = 0;
        glfwGetFramebufferSize(_mainWindow, &frameBufferW, &frameBufferH);

        if (frameBufferW == 2 * w && frameBufferH == 2 * h)
        {
            if (_isRetinaEnabled)
            {
                _retinaFactor = 1;
            }
            else
            {
                _retinaFactor = 2;
            }
            glfwSetWindowSize(_mainWindow, _screenSize.width/2 * _retinaFactor * _frameZoomFactor, _screenSize.height/2 * _retinaFactor * _frameZoomFactor);

            _isInRetinaMonitor = true;
        }
        else
        {
            if (_isInRetinaMonitor)
            {
                _retinaFactor = 1;
            }
            glfwSetWindowSize(_mainWindow, _screenSize.width * _retinaFactor * _frameZoomFactor, _screenSize.height *_retinaFactor * _frameZoomFactor);

            _isInRetinaMonitor = false;
        }
    }
#endif	
}

void GLViewImpl::setFrameSize(float width, float height)
{
    GLView::setFrameSize(width, height);
    updateFrameSize();
}

void GLViewImpl::setViewPortInPoints(float x , float y , float w , float h)
{
#if !USE_ROTATE90
    glViewport((GLint)(x * _scaleX * _retinaFactor * _frameZoomFactor + _viewPortRect.origin.x * _retinaFactor * _frameZoomFactor),
               (GLint)(y * _scaleY * _retinaFactor  * _frameZoomFactor + _viewPortRect.origin.y * _retinaFactor * _frameZoomFactor),
               (GLsizei)(w * _scaleX * _retinaFactor * _frameZoomFactor),
               (GLsizei)(h * _scaleY * _retinaFactor * _frameZoomFactor));
#else
    glViewport(
    	(GLint)(y * _scaleY * _retinaFactor * _frameZoomFactor + _viewPortRect.origin.y * _retinaFactor * _frameZoomFactor),
    	(GLint)(x * _scaleX * _retinaFactor * _frameZoomFactor + _viewPortRect.origin.x * _retinaFactor * _frameZoomFactor),
               (GLsizei)(h * _scaleY * _retinaFactor * _frameZoomFactor),
               (GLsizei)(w * _scaleX * _retinaFactor * _frameZoomFactor)
              );
#endif
}

void GLViewImpl::setScissorInPoints(float x , float y , float w , float h)
{
#if !USE_ROTATE90
    glScissor((GLint)(x * _scaleX * _retinaFactor * _frameZoomFactor + _viewPortRect.origin.x * _retinaFactor * _frameZoomFactor),
               (GLint)(y * _scaleY * _retinaFactor  * _frameZoomFactor + _viewPortRect.origin.y * _retinaFactor * _frameZoomFactor),
               (GLsizei)(w * _scaleX * _retinaFactor * _frameZoomFactor),
               (GLsizei)(h * _scaleY * _retinaFactor * _frameZoomFactor));
#else
    glScissor(
    	(GLint)(y * _scaleY * _retinaFactor  * _frameZoomFactor + _viewPortRect.origin.y * _retinaFactor * _frameZoomFactor),
        (GLint)(x * _scaleX * _retinaFactor * _frameZoomFactor + _viewPortRect.origin.x * _retinaFactor * _frameZoomFactor),
        (GLsizei)(h * _scaleY * _retinaFactor * _frameZoomFactor),
        (GLsizei)(w * _scaleX * _retinaFactor * _frameZoomFactor)
       );
#endif
}

#if USE_NO_GLFW

#else	
void GLViewImpl::onGLFWError(int errorID, const char* errorDesc)
{
    CCLOGERROR("GLFWError #%d Happen, %s\n", errorID, errorDesc);
}

void GLViewImpl::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
    if(GLFW_MOUSE_BUTTON_LEFT == button)
    {
        if(GLFW_PRESS == action)
        {
            _captured = true;
            if (this->getViewPortRect().equals(Rect::ZERO) || this->getViewPortRect().containsPoint(Vec2(_mouseX,_mouseY)))
            {
                intptr_t id = 0;
                this->handleTouchesBegin(1, &id, &_mouseX, &_mouseY);
            }
        }
        else if(GLFW_RELEASE == action)
        {
            if (_captured)
            {
                _captured = false;
                intptr_t id = 0;
                this->handleTouchesEnd(1, &id, &_mouseX, &_mouseY);
            }
        }
    }
    
    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    if(GLFW_PRESS == action)
    {
        EventMouse event(EventMouse::MouseEventType::MOUSE_DOWN);
        event.setCursorPosition(cursorX, cursorY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    else if(GLFW_RELEASE == action)
    {
        EventMouse event(EventMouse::MouseEventType::MOUSE_UP);
        event.setCursorPosition(cursorX, cursorY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void GLViewImpl::onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
{
    _mouseX = (float)x;
    _mouseY = (float)y;

    _mouseX /= this->getFrameZoomFactor();
    _mouseY /= this->getFrameZoomFactor();

    if (_isInRetinaMonitor)
    {
        if (_retinaFactor == 1)
        {
            _mouseX *= 2;
            _mouseY *= 2;
        }
    }

    if (_captured)
    {
        intptr_t id = 0;
        this->handleTouchesMove(1, &id, &_mouseX, &_mouseY);
    }
    
    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
    // Set current button
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        event.setMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        event.setMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        event.setMouseButton(GLFW_MOUSE_BUTTON_MIDDLE);
    }
    event.setCursorPosition(cursorX, cursorY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GLViewImpl::onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{
    EventMouse event(EventMouse::MouseEventType::MOUSE_SCROLL);
    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;
    event.setScrollData((float)x, -(float)y);
    event.setCursorPosition(cursorX, cursorY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GLViewImpl::onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (GLFW_REPEAT != action)
    {
        EventKeyboard event(g_keyCodeMap[key], GLFW_PRESS == action);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->dispatchEvent(&event);
    }
    
    if (GLFW_RELEASE != action && g_keyCodeMap[key] == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        IMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
    }
}

void GLViewImpl::onGLFWCharCallback(GLFWwindow *window, unsigned int character)
{
    char16_t wcharString[2] = { (char16_t) character, 0 };
    std::string utf8String;

    StringUtils::UTF16ToUTF8( wcharString, utf8String );
    IMEDispatcher::sharedDispatcher()->dispatchInsertText( utf8String.c_str(), utf8String.size() );
}

void GLViewImpl::onGLFWWindowPosCallback(GLFWwindow *windows, int x, int y)
{
    Director::getInstance()->setViewport();
}

void GLViewImpl::onGLFWframebuffersize(GLFWwindow* window, int w, int h)
{
    float frameSizeW = _screenSize.width;
    float frameSizeH = _screenSize.height;
    float factorX = frameSizeW / w * _retinaFactor * _frameZoomFactor;
    float factorY = frameSizeH / h * _retinaFactor * _frameZoomFactor;

    if (fabs(factorX - 0.5f) < FLT_EPSILON && fabs(factorY - 0.5f) < FLT_EPSILON )
    {
        _isInRetinaMonitor = true;
        if (_isRetinaEnabled)
        {
            _retinaFactor = 1;
        }
        else
        {
            _retinaFactor = 2;
        }

        glfwSetWindowSize(window, static_cast<int>(frameSizeW * 0.5f * _retinaFactor * _frameZoomFactor) , static_cast<int>(frameSizeH * 0.5f * _retinaFactor * _frameZoomFactor));
    }
    else if(fabs(factorX - 2.0f) < FLT_EPSILON && fabs(factorY - 2.0f) < FLT_EPSILON)
    {
        _isInRetinaMonitor = false;
        _retinaFactor = 1;
        glfwSetWindowSize(window, static_cast<int>(frameSizeW * _retinaFactor * _frameZoomFactor), static_cast<int>(frameSizeH * _retinaFactor * _frameZoomFactor));
    }
}

void GLViewImpl::onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
{
    if (_resolutionPolicy != ResolutionPolicy::UNKNOWN)
    {
        updateDesignResolutionSize();
        Director::getInstance()->setViewport();
    }
}

void GLViewImpl::onGLFWWindowIconifyCallback(GLFWwindow* window, int iconified)
{
    if (iconified == GL_TRUE)
    {
        Application::getInstance()->applicationDidEnterBackground();
    }
    else
    {
        Application::getInstance()->applicationWillEnterForeground();
    }
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static bool glew_dynamic_binding()
{
    const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

    // If the current opengl driver doesn't have framebuffers methods, check if an extension exists
    if (glGenFramebuffers == nullptr)
    {
        log("OpenGL: glGenFramebuffers is nullptr, try to detect an extension");
        if (strstr(gl_extensions, "ARB_framebuffer_object"))
        {
            log("OpenGL: ARB_framebuffer_object is supported");

            glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
            glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
            glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
            glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
            glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
            glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
            glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
            glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
            glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
            glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
            glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
            glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
            glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
        }
        else
        if (strstr(gl_extensions, "EXT_framebuffer_object"))
        {
            log("OpenGL: EXT_framebuffer_object is supported");
            glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbufferEXT");
            glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
            glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
            glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
            glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
            glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
            glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebufferEXT");
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
            glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
            glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
            glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
            glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
            glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
            glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmapEXT");
        }
        else
        {
            log("OpenGL: No framebuffers extension is supported");
            log("OpenGL: Any call to Fbo will crash!");
            return false;
        }
    }
    return true;
}
#endif

// helper
bool GLViewImpl::initGlew()
{
#if 0
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        MessageBox((char *)glewGetErrorString(GlewInitResult), "OpenGL error");
        return false;
    }

    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
    {
        log("Ready for GLSL");
    }
    else
    {
        log("Not totally ready :(");
    }

    if (glewIsSupported("GL_VERSION_2_0"))
    {
        log("Ready for OpenGL 2.0");
    }
    else
    {
        log("OpenGL 2.0 not supported");
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if(glew_dynamic_binding() == false)
    {
        MessageBox("No OpenGL framebuffer support. Please upgrade the driver of your video card.", "OpenGL error");
        return false;
    }
#endif

#endif // (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#endif
    return true;
}

NS_CC_END // end of namespace cocos2d;
