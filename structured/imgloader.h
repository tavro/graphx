class ImageLoader {
public:
	ImageLoader() = default;
	virtual ~ImageLoader() = default;

	virtual engine::Code load_img_resource(engine::Sprite* spr, const std::string& img_file, engine::ResourcePack* pack) = 0;
	virtual engine::Code save_img_resource(engine::Sprite* spr, const std::string& img_file) = 0;
};