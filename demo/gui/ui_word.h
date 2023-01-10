#include <string>

class Word {
public:
    std::string text;
    int len;

    Word(std::string str) {
        text = str;
        len = text.length();
    }
};