#include "inventory.h"

class Player {
public:
    Inventory inventory;

    engine::int_vector_2d pos;

    engine::int_vector_2d pixel_pos{0,0};
    engine::int_vector_2d tile_pos{0,0};

    engine::int_vector_2d dir;

    Player() {
        pos.x = 16;
        pos.y = 16;

        dir.x = 1;
        dir.y = 0;
    }

    void set_dir(int x, int y) {
        dir.x = x;
        dir.y = y;
    }

    void move() {
        pos.x += dir.x*16;
        pos.y += dir.y*16;

        pixel_pos.x+=dir.x;
        pixel_pos.y+=dir.y;
        tile_pos.x = pixel_pos.x/8;
        tile_pos.y = pixel_pos.y/8;
    }

    engine::Sprite* get_sprite() {
        if(dir.x == 1)
			return new engine::Sprite("demo/resources/player-right.png");
		else if(dir.x == -1)
			return new engine::Sprite("demo/resources/player-left.png");
		else if(dir.y == 1)
			return new engine::Sprite("demo/resources/player-down.png");
		return new engine::Sprite("demo/resources/player-up.png");
    }

};