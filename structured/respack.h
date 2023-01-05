class ResourcePack : public std::streambuf {
public:
	ResourcePack();
	~ResourcePack();

	bool add(const std::string& file);
	bool load(const std::string& file, const std::string& key);
	bool save(const std::string& file, const std::string& key);

	ResourceBuffer get_file_buffer(const std::string& file);
	bool loaded();
private:
	struct ResourceFile { 
        uint32_t size; 
        uint32_t offset; 
    };
	std::map<std::string, ResourceFile> files;
	std::ifstream base_file;
	std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
	std::string makeposix(const std::string& path);
};