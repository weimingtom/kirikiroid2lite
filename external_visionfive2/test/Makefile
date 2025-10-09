#	gcc triangle_x11.c -lGLESv2 -lEGL -lglfw

#GL_VERSION  : OpenGL ES 3.2 build 1.19@6345021
#GL_RENDERER : PowerVR B-Series BXE-4-32

all:
	gcc -I../include test.c -lGLESv2_PVR_MESA ../lib/libglfw3.a -lm
	
#gcc triangle.c -lGLESv2 -lEGL

#gcc triangle_x11.c -lglfw -lGLESv2_PVR_MESA /usr/lib/riscv64-linux-gnu/libEGL_mesa.so.0

clean:
	rm -rf a.out

