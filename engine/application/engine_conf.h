#pragma region engine_config
namespace engine {
	void Engine::engine_configure_system()
	{
		engine::Sprite::loader = nullptr;

#if defined(ENGINE_IMAGE_GDI)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_GDIPlus>();
#endif

#if defined(ENGINE_IMAGE_LIBPNG)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_LibPNG>();
#endif

#if defined(ENGINE_IMAGE_STB)
		engine::Sprite::loader = std::make_unique<engine::ImageLoader_STB>();
#endif

#if defined(ENGINE_IMAGE_CUSTOM_EX)
		engine::Sprite::loader = std::make_unique<ENGINE_IMAGE_CUSTOM_EX>();
#endif

#if defined(ENGINE_PLATFORM_HEADLESS)
		platform = std::make_unique<engine::Platform_Headless>();
#endif

#if defined(ENGINE_PLATFORM_WINAPI)
		platform = std::make_unique<engine::Platform_Windows>();
#endif

#if defined(ENGINE_PLATFORM_X11)
		platform = std::make_unique<engine::Platform_Linux>();
#endif

#if defined(ENGINE_PLATFORM_GLUT)
		platform = std::make_unique<engine::Platform_GLUT>();
#endif

#if defined(ENGINE_PLATFORM_EMSCRIPTEN)
		platform = std::make_unique<engine::Platform_Emscripten>();
#endif

#if defined(ENGINE_PLATFORM_CUSTOM_EX)
		platform = std::make_unique<ENGINE_PLATFORM_CUSTOM_EX>();
#endif

#if defined(ENGINE_GFX_HEADLESS)
		renderer = std::make_unique<engine::Renderer_Headless>();
#endif

#if defined(ENGINE_GFX_OPENGL10)
		renderer = std::make_unique<engine::Renderer_OGL10>();
#endif

#if defined(ENGINE_GFX_OPENGL33)
		renderer = std::make_unique<engine::Renderer_OGL33>();
#endif

#if defined(ENGINE_GFX_OPENGLES2)
		renderer = std::make_unique<engine::Renderer_OGLES2>();
#endif

#if defined(ENGINE_GFX_DIRECTX10)
		renderer = std::make_unique<engine::Renderer_DX10>();
#endif

#if defined(ENGINE_GFX_DIRECTX11)
		renderer = std::make_unique<engine::Renderer_DX11>();
#endif

#if defined(ENGINE_GFX_CUSTOM_EX)
		renderer = std::make_unique<ENGINE_RENDERER_CUSTOM_EX>();
#endif
		platform->ptr_engine = this;
		renderer->ptr_engine = this;
	}
}
#pragma endregion