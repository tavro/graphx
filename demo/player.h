#include "inventory.h"

class Player {
public:
    Inventory inventory;

    engine::int_vector_2d pixel_pos{0,0};
    engine::int_vector_2d tile_pos{0,0};

    engine::int_vector_2d dir;

    float time_elapsed = 0.0f;
    int animation_frame = 1;

    Player() {
        dir.x = 1;
        dir.y = 0;
    }

    void set_dir(int x, int y) {
        dir.x = x;
        dir.y = y;
    }

    void set_position(int x, int y) {
        pixel_pos.x = x;
        pixel_pos.y = y;
        tile_pos.x = pixel_pos.x/8;
        tile_pos.y = pixel_pos.y/8;
    }

    void move() {
        time_elapsed += 0.01f;
        if(time_elapsed >= 0.15f) {
            time_elapsed = 0.0f;
            if(++animation_frame > 2)
                animation_frame = 1;
        }
                
        pixel_pos.x+=dir.x;
        pixel_pos.y+=dir.y;
        tile_pos.x = pixel_pos.x/8;
        tile_pos.y = pixel_pos.y/8;
    }


    engine::Sprite* get_sprite() {
        std::string frame = std::to_string(animation_frame);
        if(dir.x == 1)
			return new engine::Sprite("demo/resources/player-right-" + frame + ".png");
		else if(dir.x == -1)
			return new engine::Sprite("demo/resources/player-left-" + frame + ".png");
		else if(dir.y == 1)
			return new engine::Sprite("demo/resources/player-down-" + frame + ".png");
		return new engine::Sprite("demo/resources/player-up-" + frame + ".png");
    }

};