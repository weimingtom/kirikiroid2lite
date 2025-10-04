#pragma once
#ifdef WIN32 
#if defined(_M_X64)
#define GLEW_STATIC
#endif
#include "GL/glew.h"
#else
#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#endif
#ifdef __APPLE__
#if 1 //defined(__MACOS__)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif
#endif

bool TVPCheckGLExtension(const std::string &extname);
