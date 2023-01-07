#ifndef DECAL_DEF
#define DECAL_DEF

class Decal {
public:
	Decal(engine::Sprite* spr, bool filter = false, bool clamp = true);
	Decal(const uint32_t existing_texture_resource, engine::Sprite* spr);
	virtual ~Decal();

	void update();
	void update_sprite();

public:
	int32_t id = -1;
	engine::Sprite* sprite = nullptr;
	engine::float_vector_2d UV_scale = { 1.0f, 1.0f };
};

#endif