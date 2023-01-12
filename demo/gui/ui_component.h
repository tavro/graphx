#ifndef COMP_DEF
#define COMP_DEF

#include <string>

class ComponentUI {
public:
    engine::int_vector_2d pos;
    engine::int_vector_2d size;

    std::string title;

    engine::Pixel border_color;
    engine::Pixel inside_color;
    engine::Pixel text_color;

    ComponentUI(int x, int y, int w, int h, std::string str, engine::Pixel border, engine::Pixel inside, engine::Pixel text) {
        pos.x = x;
        pos.y = y;
        size.x = w;
        size.y = h;
        title = str;
        border_color = border;
        inside_color = inside;
        text_color = text;
    }
};

#endif