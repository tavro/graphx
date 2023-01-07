#pragma region compiler_config
#define USE_EXPERIMENTAL_FS
#if defined(_WIN32)
	#if _MSC_VER >= 1920 && _MSVC_LANG >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif
#if defined(__linux__) || defined(__MINGW32__) || defined(__EMSCRIPTEN__) || defined(__FreeBSD__) || defined(__APPLE__)
	#if __cplusplus >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif

#if !defined(ENGINE_KEYBOARD_UK)
	#define ENGINE_KEYBOARD_UK
#endif

#if defined(USE_EXPERIMENTAL_FS) || defined(FORCE_EXPERIMENTAL_FS)
	// C++14
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#include <experimental/filesystem>
	namespace _gfs = std::experimental::filesystem::v1;
#else
	// C++17
	#include <filesystem>
	namespace _gfs = std::filesystem;
#endif

#if defined(UNICODE) || defined(_UNICODE)
	#define engineT(s) L##s
#else
	#define engineT(s) s
#endif

#define UNUSED(x) (void)(x)

#if defined(ENGINE_PGE_HEADLESS)
	#define ENGINE_PLATFORM_HEADLESS
	#define ENGINE_GFX_HEADLESS
	#if !defined(ENGINE_IMAGE_STB) && !defined(ENGINE_IMAGE_GDI) && !defined(ENGINE_IMAGE_LIBPNG)
		#define ENGINE_IMAGE_HEADLESS
	#endif
#endif

#if !defined(ENGINE_PLATFORM_WINAPI) && !defined(ENGINE_PLATFORM_X11) && !defined(ENGINE_PLATFORM_GLUT) && !defined(ENGINE_PLATFORM_EMSCRIPTEN) && !defined(ENGINE_PLATFORM_HEADLESS)
	#if !defined(ENGINE_PLATFORM_CUSTOM_EX)
		#if defined(_WIN32)
			#define ENGINE_PLATFORM_WINAPI
		#endif
		#if defined(__linux__) || defined(__FreeBSD__)
			#define ENGINE_PLATFORM_X11
		#endif
		#if defined(__APPLE__)
			#define GL_SILENCE_DEPRECATION
			#define ENGINE_PLATFORM_GLUT
		#endif
		#if defined(__EMSCRIPTEN__)
			#define ENGINE_PLATFORM_EMSCRIPTEN
		#endif
	#endif
#endif

#if defined(ENGINE_PLATFORM_GLUT) || defined(ENGINE_PLATFORM_EMSCRIPTEN)
	#define PGE_USE_CUSTOM_START
#endif

#if !defined(ENGINE_GFX_OPENGL10) && !defined(ENGINE_GFX_OPENGL33) && !defined(ENGINE_GFX_DIRECTX10) && !defined(ENGINE_GFX_HEADLESS)
	#if !defined(ENGINE_GFX_CUSTOM_EX)
		#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
			#define ENGINE_GFX_OPENGL33
		#else
			#define ENGINE_GFX_OPENGL10
		#endif
	#endif
#endif

#if !defined(ENGINE_IMAGE_STB) && !defined(ENGINE_IMAGE_GDI) && !defined(ENGINE_IMAGE_LIBPNG) && !defined(ENGINE_IMAGE_HEADLESS)
	#if !defined(ENGINE_IMAGE_CUSTOM_EX)
		#if defined(_WIN32)
			#define	ENGINE_IMAGE_GDI
		#endif
		#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
			#define	ENGINE_IMAGE_LIBPNG
		#endif
	#endif
#endif

#if !defined(ENGINE_PGE_HEADLESS)
#if defined(ENGINE_PLATFORM_WINAPI)	
	#define _WINSOCKAPI_
		#if !defined(VC_EXTRALEAN)
		#define VC_EXTRALEAN
	#endif
	#if !defined(NOMINMAX)
		#define NOMINMAX
	#endif

	#if !defined(_WIN32_WINNT)
		#ifdef HAVE_MSMF
			#define _WIN32_WINNT 0x0600
		#else
			#define _WIN32_WINNT 0x0500
		#endif
	#endif

	#include <windows.h>
	#undef _WINSOCKAPI_
#endif

#if defined(ENGINE_PLATFORM_X11)
	namespace X11
	{
		#include <X11/X.h>
		#include <X11/Xlib.h>
	}
#endif

#if defined(ENGINE_PLATFORM_GLUT)
	#if defined(__linux__)
		#include <GL/glut.h>
		#include <GL/freeglut_ext.h>
	#endif
	#if defined(__APPLE__)
		#include <GLUT/glut.h>
	#include <objc/message.h>
	#include <objc/NSObjCRuntime.h>
	#endif
#endif
#endif

#if defined(ENGINE_PGE_HEADLESS)
#if defined max
#undef max
#endif
#if defined min
#undef min
#endif
#endif
#pragma endregion