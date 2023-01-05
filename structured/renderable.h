class Renderable {
public:
	Renderable() = default;		
	Renderable(Renderable&& r) : sprite(std::move(r.sprite)), decal(std::move(r.decal)) {}		
	Renderable(const Renderable&) = delete;

	engine::Code load(const std::string& file, ResourcePack* pack = nullptr, bool filter = false, bool clamp = true);

	void create(uint32_t width, uint32_t height, bool filter = false, bool clamp = true);

	engine::Decal* Decal() const;
	engine::Sprite* Sprite() const;

private:
	std::unique_ptr<engine::Sprite> sprite = nullptr;
	std::unique_ptr<engine::Decal> decal = nullptr;
};