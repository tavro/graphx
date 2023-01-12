#ifndef BREAKABLE_DEF
#define BREAKABLE_DEF

#include "tile.h"
#include "item.h"

class BreakableTile : public Tile {
public:
    int hp = 3;
    Item* drop_item = nullptr;

    /*
     * Returns true if tile is broken
     */
    bool hit() {
        return (--hp <= 0);
    }

    BreakableTile(std::string path, engine::int_vector_2d p, bool solid, std::string type, Item* item) : Tile(path, p, solid, type) {
        drop_item = item;
    }
};

#endif