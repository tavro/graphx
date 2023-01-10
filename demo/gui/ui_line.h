#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "../../engine.h"
#include "ui_word.h"

class Line {
public:
    std::vector<Word> words;
    engine::Pixel color;

    Line(std::string str, engine::Pixel col) {
        std::stringstream ss(str);

        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;

        std::vector<std::string> vstrings(begin, end);

        for(std::string str : vstrings) {
            Word word{str};
            words.push_back(word);
        }

        color = col;
    }
};