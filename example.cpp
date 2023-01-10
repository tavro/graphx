#define APPLICATION_DEF
#include "engine.h"
#include "PerlinNoise.hpp"
#include "demo/state.h"
#include "demo/tile.h"
#include "demo/level.h"

using namespace std;

class Engine3D : public engine::Engine {
public:
    Engine3D() {
        app_name = "demo";
    }

private:
	State state = State::RUNNING;

	Level level;

	int player_x = (screen_width() / 8) / 2;
	int player_y = (screen_height() / 8) / 2 + 1;

	engine::int_vector_2d dir;

	int frames_to_appear = 32;

	void draw_gui_rect(int x, int y, int width, int height, string title) {
		fill_rect(x, y, width, height, engine::BLUE);
		draw_rect(x, y, width-1, height-1, engine::DARK_GREY);
		draw_string(x+1, y+2, title, engine::WHITE);
		draw_string(x+2, y+height-8-1, "PRESS ANY KEY", engine::BLACK);
	}

public:
	bool on_create() override {
		
		return true;
	}

	bool on_update(float elapsed_time) override {
		if (get_key(engine::Key::D).pressed) {
			player_x += 8;
			dir.x = 1;
			dir.y = 0;
		}
		
		if (get_key(engine::Key::S).pressed) {
			player_y += 8;
			dir.x = 0;
			dir.y = 1;
		}

		if (get_key(engine::Key::A).pressed) {
			player_x -= 8;
			dir.x = -1;
			dir.y = 0;
		}
		
		if (get_key(engine::Key::W).pressed) {
			player_y -= 8;
			dir.x = 0;
			dir.y = -1;
		}

		for (int y = 0; y < screen_height() / 8; ++y) {
			for (int x = 0; x < screen_width() / 8; ++x) {
				draw_sprite(level.get_tile(x,y)->pos.x, level.get_tile(x,y)->pos.y, level.get_tile(x,y)->sprite, 1, 0);
			}
		}

		fill_rect(0, 0, 96, 24, engine::BLACK);
		fill_rect(player_x, player_y, 8, 8, engine::WHITE);

		std::string mouse_x = std::to_string(get_mouse_x());
		std::string mouse_y = std::to_string(get_mouse_y());
		draw_string(0, 0, "mouse x:" + mouse_x, engine::WHITE);
		draw_string(0, 8, "mouse y:" + mouse_y, engine::WHITE);

		std::string dir_x = std::to_string(dir.x);
		std::string dir_y = std::to_string(dir.y);
		draw_string(0, 16, "dir: (" + dir_x + ", " + dir_y + ")", engine::WHITE);

		int x_pos = get_mouse_x()/8;
		int y_pos = get_mouse_y()/8;
		draw_rect(x_pos*8, y_pos*8, 7, 7, engine::BLACK);

		if(get_key(engine::Key::SPACE).pressed) {
			frames_to_appear = 32;
		}

		if(frames_to_appear > 0) {
			frames_to_appear--;
			fill_rect(player_x + dir.x*8, player_y + dir.y*8, 8, 8, engine::RED);
		}

		//draw_gui_rect(64, 64, 128, 128, "TITLE");

		return true;
	}
};

int main() {
    Engine3D demo;

    if(demo.construct(256, 240, 4, 4))
        demo.start();

    return 0;
}