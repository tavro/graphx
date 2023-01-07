#ifndef LAYER_DESC_DEF
#define LAYER_DESC_DEF

struct LayerDesc {
	engine::float_vector_2d offset = { 0, 0 };
	engine::float_vector_2d scale  = { 1, 1 };

	bool show   = false;
	bool update = false;

	engine::Renderable draw_target;
		
    uint32_t res_ID = 0;
	std::vector<DecalInstance> decal_instances;
	engine::Pixel tint = engine::WHITE;
	std::function<void()> func_hook = nullptr;
};

#endif