#pragma region opengl33_iface

#if defined(ENGINE_GFX_OPENGL33)

	#if defined(ENGINE_PLATFORM_WINAPI)
		#include <gl/GL.h>
		#define CALLSTYLE __stdcall
	#endif

	#if defined(__linux__) || defined(__FreeBSD__)
		#include <GL/gl.h>
	#endif

	#if defined(ENGINE_PLATFORM_X11)
		namespace X11
		{#include <GL/glx.h>}
		#define CALLSTYLE 
	#endif

	#if defined(__APPLE__)
		#define GL_SILENCE_DEPRECATION
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#endif

	#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#define GL_GLEXT_PROTOTYPES
		#include <GLES2/gl2ext.h>
		#include <emscripten/emscripten.h>
		#define CALLSTYLE
		#define GL_CLAMP GL_CLAMP_TO_EDGE
	#endif

namespace engine {
	typedef char GLchar;
	typedef ptrdiff_t GLsizeiptr;

	typedef GLuint CALLSTYLE locCreateShader_t(GLenum type);
	typedef GLuint CALLSTYLE locCreateProgram_t(void);
	typedef void CALLSTYLE locDeleteShader_t(GLuint shader);
	typedef void CALLSTYLE locCompileShader_t(GLuint shader);
	typedef void CALLSTYLE locLinkProgram_t(GLuint program);
	typedef void CALLSTYLE locDeleteProgram_t(GLuint program);
	typedef void CALLSTYLE locAttachShader_t(GLuint program, GLuint shader);
	typedef void CALLSTYLE locBindBuffer_t(GLenum target, GLuint buffer);
	typedef void CALLSTYLE locBufferData_t(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void CALLSTYLE locGenBuffers_t(GLsizei n, GLuint* buffers);
	typedef void CALLSTYLE locVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void CALLSTYLE locEnableVertexAttribArray_t(GLuint index);
	typedef void CALLSTYLE locUseProgram_t(GLuint program);
	typedef void CALLSTYLE locBindVertexArray_t(GLuint array);
	typedef void CALLSTYLE locGenVertexArrays_t(GLsizei n, GLuint* arrays);
	typedef void CALLSTYLE locGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
	typedef GLint CALLSTYLE locGetUniformLocation_t(GLuint program, const GLchar* name);
	typedef void CALLSTYLE locUniform1f_t(GLint location, GLfloat v0);
	typedef void CALLSTYLE locUniform1i_t(GLint location, GLint v0);
	typedef void CALLSTYLE locUniform2fv_t(GLint location, GLsizei count, const GLfloat* value);
	typedef void CALLSTYLE locActiveTexture_t(GLenum texture);
	typedef void CALLSTYLE locGenFrameBuffers_t(GLsizei n, GLuint* ids);
	typedef void CALLSTYLE locBindFrameBuffer_t(GLenum target, GLuint fb);
	typedef GLenum CALLSTYLE locCheckFrameBufferStatus_t(GLenum target);
	typedef void CALLSTYLE locDeleteFrameBuffers_t(GLsizei n, const GLuint* fbs);
	typedef void CALLSTYLE locFrameBufferTexture2D_t(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void CALLSTYLE locDrawBuffers_t(GLsizei n, const GLenum* bufs);
	typedef void CALLSTYLE locBlendFuncSeparate_t(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

#if defined(ENGINE_PLATFORM_WINAPI)
	typedef void __stdcall locSwapInterval_t(GLsizei n);
#endif

#if defined(ENGINE_PLATFORM_X11)
	typedef int(locSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
	typedef EGLBoolean(locSwapInterval_t)(EGLDisplay display, EGLint interval);
#else
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#endif

}
#endif
#pragma endregion