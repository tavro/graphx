#ifndef ITEM_DEF
#define ITEM_DEF

#include <string>

class Item {
public:
    engine::int_vector_2d pos;
    engine::Sprite* sprite;
    std::string name;

    engine::int_vector_2d rand_dir;
    int vel=4;

    Item(int x, int y, engine::Sprite* spr, std::string n) {
        pos.x = x;
        pos.y = y;
        sprite = spr;
        name = n;

        rand_dir.x = rand() % 3 + (-1);
        rand_dir.y = rand() % 3 + (-1);
    }

    void tick() {
        if(vel > 0) {
            pos.x += vel*rand_dir.x;
            pos.y += vel*rand_dir.y;
            vel--;
        }
    }
};

#endif