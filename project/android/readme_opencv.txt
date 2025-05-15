

https://stackoverflow.com/questions/50517883/fatal-error-ext-atomicity-h-file-not-found-using-opencv-2-4-11

Fatal error: 'ext/atomicity.h' file not found using OpenCV 2.4.11

src\core\external\opencv-2.4.13\modules\core\include\opencv2\core\operations.hpp

    #if !(defined WIN32 || defined _WIN32) && (defined __i486__ || defined __i586__ || \
        defined __i686__ || defined __MMX__ || defined __SSE__  || defined __ppc__) || \
        (defined __GNUC__ && defined _STLPORT_MAJOR) || \
        defined __EMSCRIPTEN__ || defined(ANDROID)

      #define CV_XADD __sync_fetch_and_add
    #else
      #include <ext/atomicity.h>
      #define CV_XADD __gnu_cxx::__exchange_and_add
    #endif
	