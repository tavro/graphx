class Engine {
public:
	Engine();
	virtual ~Engine();
    
	engine::Code construct (int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool fullscreen = false, bool vsync = false, bool cohesion = false);
	engine::Code start();

	virtual bool on_create();
	virtual bool on_update(float elapsed_time);
	virtual bool on_destroy();

	virtual void on_text_entry_complete(const std::string& stext);
	virtual bool on_console_cmd(const std::string& sCommand);

	bool is_focused() const;

	ButtonState get_key(Key k) const;
	ButtonState get_mouse(uint32_t b) const;

	int32_t get_mouse_x() const;
	int32_t get_mouse_y() const;
	int32_t get_mouse_wheel_delta() const;
		
	const engine::int_vector_2d& get_window_mouse() const;
	const engine::int_vector_2d& get_mouse_pos() const;

	static const std::map<size_t, uint8_t>& get_keys() { return keys; }
	
    int32_t screen_width() const;
	int32_t screen_height() const;
		
    int32_t get_draw_target_width() const;
	int32_t get_draw_target_height() const;

	engine::Sprite* get_draw_target() const;

	void set_screen_size(int w, int h);
        
	void set_draw_target(Sprite* target);
		
    uint32_t get_FPS() const;
		
    float get_elapsed_time() const;
		
    const engine::int_vector_2d& get_window_size() const;
    const engine::int_vector_2d& get_pixel_size() const;
	const engine::int_vector_2d& get_screen_pixel_size() const;
	const engine::int_vector_2d& get_screen_size() const;
		
    const std::vector<std::string>& get_dropped_files() const;
	const engine::int_vector_2d& get_dropped_files_point() const;

	void set_draw_target (uint8_t layer, bool bDirty = true);
	void enable_layer    (uint8_t layer, bool b);
	void set_layer_offset(uint8_t layer, const engine::float_vector_2d& offset);
	void set_layer_offset(uint8_t layer, float x, float y);
	void set_layer_scale (uint8_t layer, const engine::float_vector_2d& scale);
	void set_layer_scale (uint8_t layer, float x, float y);
	void set_layer_tint  (uint8_t layer, const engine::Pixel& tint);
	void set_layer_custom_render_func(uint8_t layer, std::function<void()> f);

	std::vector<LayerDesc>& get_layers();
	uint32_t create_layer();

	// NORMAL = No transparency
	// MASK   = Transparent if alpha is < 255
	// ALPHA  = Full transparency
	void set_pixel_mode(Pixel::Mode m);
	Pixel::Mode get_pixel_mode();

	void set_pixel_mode(std::function<engine::Pixel(const int x, const int y, const engine::Pixel& src, const engine::Pixel& dest)> pixel_mode);
	void set_pixel_blend(float blend);

	virtual bool draw(int32_t x, int32_t y, Pixel p = engine::WHITE);
	bool draw(const engine::int_vector_2d& pos, Pixel p = engine::WHITE);

	void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = engine::WHITE, uint32_t pattern = 0xFFFFFFFF);
	void draw_line(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, Pixel p = engine::WHITE, uint32_t pattern = 0xFFFFFFFF);

	void draw_circle(int32_t x, int32_t y, int32_t radius, Pixel p = engine::WHITE, uint8_t mask = 0xFF);
	void draw_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p = engine::WHITE, uint8_t mask = 0xFF);
	void fill_circle(int32_t x, int32_t y, int32_t radius, Pixel p = engine::WHITE);
	void fill_circle(const engine::int_vector_2d& pos, int32_t radius, Pixel p = engine::WHITE);
		
	void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = engine::WHITE);
	void draw_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p = engine::WHITE);
	void fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = engine::WHITE);
	void fill_rect(const engine::int_vector_2d& pos, const engine::int_vector_2d& size, Pixel p = engine::WHITE);
		
	void draw_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = engine::WHITE);
	void draw_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p = engine::WHITE);
	void fill_triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = engine::WHITE);
	void fill_triangle(const engine::int_vector_2d& pos1, const engine::int_vector_2d& pos2, const engine::int_vector_2d& pos3, Pixel p = engine::WHITE);
		
	void fill_textured_triangle(const std::vector<engine::float_vector_2d>& points, std::vector<engine::float_vector_2d> tex, std::vector<engine::Pixel> color, engine::Sprite* spr_tex);
	void fill_textured_polygon(const std::vector<engine::float_vector_2d>& points, const std::vector<engine::float_vector_2d>& tex, const std::vector<engine::Pixel>& color, engine::Sprite* spr_tex, engine::DecalStructure structure = engine::DecalStructure::LIST);
		
	void draw_sprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
	void draw_sprite(const engine::int_vector_2d& pos, Sprite* sprite, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
	void draw_partial_sprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
	void draw_partial_sprite(const engine::int_vector_2d& pos, Sprite* sprite, const engine::int_vector_2d& source_pos, const engine::int_vector_2d& size, uint32_t scale = 1, uint8_t flip = engine::Sprite::NONE);
		
	void draw_string(int32_t x, int32_t y, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
	void draw_string(const engine::int_vector_2d& pos, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		
	engine::int_vector_2d get_text_size(const std::string& s);
		
	void draw_string_prop(int32_t x, int32_t y, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
	void draw_string_prop(const engine::int_vector_2d& pos, const std::string& text, Pixel col = engine::WHITE, uint32_t scale = 1);
		
	engine::int_vector_2d get_text_size_prop(const std::string& s);

	void set_decal_mode(const engine::DecalMode& mode);
	void set_decal_structure(const engine::DecalStructure& structure);
		
	void draw_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
		
	void draw_partial_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
	void draw_partial_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, engine::Decal* decal, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		
	void draw_explicit_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d* uv, const engine::Pixel* col, uint32_t elements = 4);
		
	void draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::Pixel& tint = engine::WHITE);
	void draw_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::Pixel& tint = engine::WHITE);
	void draw_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::Pixel& tint = engine::WHITE);
		
	void draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d(&pos)[4], const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
	void draw_partial_warped_decal(engine::Decal* decal, const engine::float_vector_2d* pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
	void draw_partial_warped_decal(engine::Decal* decal, const std::array<engine::float_vector_2d, 4>& pos, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::Pixel& tint = engine::WHITE);
		
	void draw_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::float_vector_2d& scale = { 1.0f,1.0f }, const engine::Pixel& tint = engine::WHITE);
	void draw_partial_rotated_decal(const engine::float_vector_2d& pos, engine::Decal* decal, const float angle, const engine::float_vector_2d& center, const engine::float_vector_2d& source_pos, const engine::float_vector_2d& source_size, const engine::float_vector_2d& scale = { 1.0f, 1.0f }, const engine::Pixel& tint = engine::WHITE);
		
	void draw_string_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
	void draw_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
	void draw_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col = engine::WHITE);
	void fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col = engine::WHITE);
		
	void gradient_fill_rect_decal(const engine::float_vector_2d& pos, const engine::float_vector_2d& size, const engine::Pixel col_TL, const engine::Pixel col_BL, const engine::Pixel col_BR, const engine::Pixel col_TR);
		
	void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint = engine::WHITE);
	void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<float>& depth, const std::vector<engine::float_vector_2d>& uv, const engine::Pixel tint = engine::WHITE);
	void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel>& tint);
	void draw_polygon_decal(engine::Decal* decal, const std::vector<engine::float_vector_2d>& pos, const std::vector<engine::float_vector_2d>& uv, const std::vector<engine::Pixel>& colors, const engine::Pixel tint);

	void draw_line_decal(const engine::float_vector_2d& pos1, const engine::float_vector_2d& pos2, Pixel p = engine::WHITE);
	void draw_rotated_string_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
	void draw_rotated_string_prop_decal(const engine::float_vector_2d& pos, const std::string& text, const float angle, const engine::float_vector_2d& center = { 0.0f, 0.0f }, const engine::Pixel col = engine::WHITE, const engine::float_vector_2d& scale = { 1.0f, 1.0f });
		
	void clear(Pixel p);
	void clear_buffer(Pixel p, bool depth = true);
		
	engine::Sprite* get_font_sprite();

	bool clip_line_to_screen(engine::int_vector_2d& pos1, engine::int_vector_2d& pos2);

	void enable_pixel_transfer(const bool enable = true);

	void console_show(const engine::Key &key_exit, bool suspend_time = true);
	bool is_console_showing() const;
	void console_clear();
	std::stringstream& console_out();
	void console_capture_std_out(const bool capture);

	void text_entry_enable(const bool enable, const std::string& text = "");
	std::string text_entry_get_string() const;
	int32_t text_entry_get_cursor() const;
	bool is_text_entry_enabled() const;

private:
	void update_text_entry();
	void update_console();

public:
	std::string app_name;

private:
	engine::Sprite*         draw_target = nullptr;
	Pixel::Mode	            pixel_mode = Pixel::NORMAL;
	float		            blend_factor = 1.0f;
	engine::int_vector_2d	screen_size = { 256, 240 };
	engine::float_vector_2d	inv_screen_size = { 1.0f / 256.0f, 1.0f / 240.0f };
	engine::int_vector_2d	pixel_size = { 4, 4 };
	engine::int_vector_2d   screen_pixel_size = { 4, 4 };
	engine::int_vector_2d	mouse_pos = { 0, 0 };
	int32_t		            mouse_wheel_delta = 0;
	engine::int_vector_2d	mouse_pos_cache = { 0, 0 };
	engine::int_vector_2d   mouse_window_pos = { 0, 0 };
	int32_t		            mouse_wheel_delta_cache = 0;
	engine::int_vector_2d	window_size = { 0, 0 };
	engine::int_vector_2d	view_pos = { 0, 0 };
	engine::int_vector_2d	view_size = { 0,0 };
	bool		            fullscreen = false;
	engine::float_vector_2d	pixel = { 1.0f, 1.0f };
	bool		            has_input_focus = false;
	bool		            has_mouse_focus = false;
	bool		            enable_VSYNC = false;
	float		            frame_timer = 1.0f;
	float		            last_elapsed = 0.0f;
	int			            frame_count = 0;		
	bool                    suspend_texture_transfer = false;
	Renderable              font_renderable;
	std::vector<LayerDesc>  layers;
	uint8_t		            target_layer = 0;
	uint32_t	            last_FPS = 0;
	bool                    pixel_cohesion = false;
	DecalMode               decal_mode = DecalMode::NORMAL;
	DecalStructure          decal_structure = DecalStructure::FAN;

	std::function<engine::Pixel(const int x, const int y, const engine::Pixel&, const engine::Pixel&)> func_pixel_mode;
	std::chrono::time_point<std::chrono::system_clock> time_point1, time_point2;
	std::vector<engine::int_vector_2d> font_spacing;

	std::vector<std::string> dropped_files;
	std::vector<std::string> dropped_files_cache;

	engine::int_vector_2d dropped_files_point;
	engine::int_vector_2d dropped_files_point_cache;

	bool console_show = false;
	bool console_suspend_time = false;
		
    engine::Key key_console_exit = engine::Key::F1;
		
    std::stringstream console_output;
	std::streambuf* buf_old_cout = nullptr;
		
    engine::int_vector_2d console_size;
	engine::int_vector_2d console_cursor = { 0,0 };
	engine::float_vector_2d console_character_scale = { 1.0f, 2.0f };

	std::vector<std::string> console_lines;
	std::list<std::string> cmd_history;
	std::list<std::string>::iterator cmd_history_it;

	bool text_entry_enable = false;
	std::string text_entry_string = "";
	int32_t text_entry_cursor = 0;
	std::vector<std::tuple<engine::Key, std::string, std::string>> keyboard;

	bool		key_new_state[256] = { 0 };
	bool		key_old_state[256] = { 0 };
	ButtonState	keyboard_state[256] = { 0 };

	bool		mouse_new_state[mouse_buttons] = { 0 };
	bool		mouse_old_state[mouse_buttons] = { 0 };
	ButtonState	mouse_state[mouse_buttons] = { 0 };

	void		EngineThread();

	static std::atomic<bool> atom_active;

public:
	void engine_update_mouse(int32_t x, int32_t y);
	void engine_update_mouse_wheel(int32_t delta);
	void engine_update_window_size(int32_t x, int32_t y);
	void engine_update_viewport();
	void engine_construct_fontsheet();
	void engine_core_update();
	void engine_prepare_engine();
	void engine_update_mouse_state(int32_t button, bool state);
	void engine_update_key_state(int32_t key, bool state);
	void engine_update_mouse_focus(bool state);
	void engine_update_key_focus(bool state);
	void engine_terminate();
	void engine_drop_files(int32_t x, int32_t y, const std::vector<std::string>& files);
	void engine_reanimate();
	bool engine_is_running();

	// chooses which components to compile
	virtual void engine_configure_system();

	friend class EngineX;
	void enginex_register(engine::EngineX* enginex);

private:
	std::vector<engine::EngineX*> extensions;
};

class EngineX {
	friend class engine::Engine;
public:
	EngineX(bool hook = false);

protected:
	virtual void on_before_create();
	virtual void on_after_create ();
	virtual bool on_before_update(float &elapsed_time);
	virtual void on_after_update (float elapsed_time);

	static Engine* engine_ptr;
};