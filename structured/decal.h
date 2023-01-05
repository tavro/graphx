class Decal {
public:
	Decal(engine::Sprite* spr, bool filter = false, bool clamp = true);
	Decal(const uint32_t existing_texture_resource, engine::Sprite* spr);
	virtual ~Decal();

	void update();
	void update_sprite();

	int32_t id = -1;
	engine::Sprite* sprite = nullptr;
	engine::float_vector_2d UV_scale = { 1.0f, 1.0f };
};

enum class DecalMode {
	NORMAL,
	ADDITIVE,
	MULTIPLICATIVE,
	STENCIL,
	ILLUMINATE,
	WIREFRAME,
	MODEL3D,
};

enum class DecalStructure {
	LINE,
	FAN,
	STRIP,
	LIST
};

struct DecalInstance {
	engine::Decal* decal = nullptr;

	std::vector<engine::float_vector_2d> pos;
	std::vector<engine::float_vector_2d> uv;
	std::vector<float> w;
	std::vector<engine::Pixel> tint;

	engine::DecalMode mode = engine::DecalMode::NORMAL;
	engine::DecalStructure structure = engine::DecalStructure::FAN;

	uint32_t points = 0;
};