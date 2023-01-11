#include <string>
#include <vector>
#include "menu_state.h"

class Menu {
public:
    MenuState state;

    const int MIN_INDEX = 0;
    int MAX_INDEX;

    int option_index;

    std::vector<std::string> options;

    engine::Pixel text_color_selected;
    engine::Pixel text_color;

    Menu(std::vector<std::string> opt, engine::Pixel col, engine::Pixel col_sel) {
        option_index = 0;
        options = opt;
        MAX_INDEX = opt.size();
        text_color = col;
        text_color_selected = col_sel;
    }

    void increase_index() {
        option_index++;
        if(option_index > MAX_INDEX-1)
            option_index = MIN_INDEX;
    }

    void decrease_index() {
        option_index--;
        if(option_index < MIN_INDEX)
            option_index = MAX_INDEX;
    }

    std::string get_option(int index) {
        if(index == option_index)
            return "> " + options[index] + " <";
        return options[index];
    }

    engine::Pixel get_option_col(int index) {
        if(index == option_index)
            return text_color_selected;
        return text_color;
    }
};