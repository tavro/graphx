#ifndef PLATFORM_DEF
#define PLATFORM_DEF

class Platform {
public:
	virtual ~Platform() = default;

	virtual engine::Code application_startup() = 0;
	virtual engine::Code application_cleanup() = 0;

	virtual engine::Code thread_startup() = 0;
	virtual engine::Code thread_cleanup() = 0;

	virtual engine::Code create_graphics(bool fullscreen, bool enable_VSYNC, const engine::int_vector_2d& view_pos, const engine::int_vector_2d& view_size) = 0;
	virtual engine::Code create_window_pane(const engine::int_vector_2d& window_pos, engine::int_vector_2d& window_size, bool fullscreen) = 0;
	virtual engine::Code set_window_title(const std::string& s) = 0;

	virtual engine::Code start_system_event_loop() = 0;
	virtual engine::Code handle_system_event    () = 0;

	static engine::Engine* ptr_engine;
};

#endif