class Sprite {
public:
	Sprite();
	Sprite(const std::string& img_file, engine::ResourcePack* pack = nullptr);
	Sprite(int32_t w, int32_t h);
	Sprite(const engine::Sprite&) = delete;
	~Sprite();

	engine::Code load_from_file(const std::string& img_file, engine::ResourcePack* pack = nullptr);

	int32_t width = 0;
	int32_t height = 0;

	enum Mode { 
        NORMAL, 
        PERIODIC, 
        CLAMP 
    };

	enum Flip { 
        NONE = 0, 
        HORIZ = 1, 
        VERT = 2 
    };

	void set_sample_mode(engine::Sprite::Mode mode = engine::Sprite::Mode::NORMAL);

	Pixel get_pixel(int32_t x, int32_t y) const;
	Pixel get_pixel(const engine::int_vector_2d& a) const;

	bool  set_pixel(int32_t x, int32_t y, Pixel p);
	bool  set_pixel(const engine::int_vector_2d& a, Pixel p);

	Pixel sample(float x, float y) const;
	Pixel sample(const engine::float_vector_2d& uv) const;

	Pixel sample_BL(float u, float v) const;
	Pixel sample_BL(const engine::float_vector_2d& uv) const;

	Pixel* get_data();

	engine::Sprite* duplicate();
	engine::Sprite* duplicate(const engine::int_vector_2d& pos, const engine::int_vector_2d& size);

	engine::int_vector_2d size() const;
	std::vector<engine::Pixel> col_data;
	Mode sample_mode = Mode::NORMAL;

	static std::unique_ptr<engine::ImageLoader> loader;
};