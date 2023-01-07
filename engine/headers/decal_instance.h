#ifndef DECAL_INS_DEF
#define DECAL_INS_DEF

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

#endif