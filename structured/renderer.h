class Renderer {
public:
	virtual ~Renderer() = default;

	virtual void prepare_device() = 0;

	virtual engine::Code create_device(std::vector<void*> params, bool fullscreen, bool VSYNC) = 0;
	virtual engine::Code destroy_device() = 0;

	virtual void       display_frame  () = 0;
	virtual void       prepare_drawing() = 0;
	virtual void	   set_decal_mode (const engine::DecalMode& mode) = 0;
	virtual void       draw_layer_quad(const engine::float_vector_2d& offset, const engine::float_vector_2d& scale, const engine::Pixel tint) = 0;
	virtual void       draw_decal     (const engine::DecalInstance& decal) = 0;
	virtual uint32_t   create_texture (const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) = 0;
	virtual void       update_texture (uint32_t id, engine::Sprite* spr) = 0;
	virtual void       read_texture   (uint32_t id, engine::Sprite* spr) = 0;
	virtual uint32_t   delete_texture (const uint32_t id) = 0;
	virtual void       apply_texture  (uint32_t id) = 0;
	virtual void       update_viewport(const engine::int_vector_2d& pos, const engine::int_vector_2d& size) = 0;
	virtual void       clear_buffer   (engine::Pixel p, bool depth) = 0;

	static engine::Engine* engine_ptr;
};