#ifndef RES_BUFF_DEF
#define RES_BUFF_DEF

struct ResourceBuffer : public std::streambuf {
	ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size);
	std::vector<char> memory;
};

#endif