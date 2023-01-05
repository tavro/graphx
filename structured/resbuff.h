struct ResourceBuffer : public std::streambuf {
	ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size);
	std::vector<char> memory;
};