#define APPLICATION_DEF
#include "engine.h"
#include "PerlinNoise.hpp"
#include "demo/state.h"
#include "demo/tile.h"
#include "demo/level.h"
#include "demo/player.h"
#include "demo/gui/ui_line.h"
#include "demo/menu/menu.h"

using namespace std;

class Engine3D : public engine::Engine {
public:
    Engine3D() {
        app_name = "demo";
    }

private:
	Menu menu{{"start", "how to play", "about", "quit"}, engine::DARK_GREY, engine::WHITE};

	State state = State::MENU;
	MenuState menu_state = MenuState::MAIN;

	Level level;

	Player player;

	engine::int_vector_2d dir;

	int frames_to_appear = 16;

	void draw_gui_rect(int x, int y, int width, int height, string title) {
		fill_rect(x, y, width, height, engine::BLUE);
		draw_rect(x, y, width-1, height-1, engine::DARK_GREY);

		int title_len = title.length()*8;
		draw_string(x+(width/2)-title_len/2, y+2, title, engine::GREY);

		int offset = y+2+16;

		vector<Line> text_objs;
		Line line1{"defeat the evil boss", engine::WHITE};
		text_objs.push_back(line1);
		Line line2{"head down before going up", engine::WHITE};
		text_objs.push_back(line2);

		for(auto text_obj : text_objs) {
			int x_off = x+2;
			for(auto word : text_obj.words) {
				if(x_off + word.len*8 > x+width) {
					x_off = x+2;
					offset += 8;
				}
				draw_string(x_off, offset, word.text, text_obj.color);
				x_off += (word.len*8+8);
			}
			offset+=16;
		}

		string close = "PRESS ANY KEY";
		int close_len = close.length()*8;
		draw_string(x+(width/2)-close_len/2, y+height-8-1, close, engine::BLACK);
	}

public:
	bool on_create() override {
		
		return true;
	}

	bool on_update(float elapsed_time) override {
		if(state == State::MENU) {
			fill_rect(0, 0, screen_width(), screen_height(), engine::BLACK);
			if(menu_state == MenuState::MAIN) {
				for(int i = 0; i < menu.MAX_INDEX; i++) {
					draw_string((screen_width()/2)-(menu.get_option(i).length()*8)/2, screen_height()/2+i*16, menu.get_option(i), menu.get_option_col(i));
				}

				if(get_key(engine::Key::S).pressed) {
					menu.increase_index();
				}
				else if(get_key(engine::Key::W).pressed) {
					menu.decrease_index();
				}

				if(get_key(engine::Key::ENTER).pressed) {
					switch(menu.option_index) {
						case 0:
							state = State::RUNNING;
							break;
						case 1:
							menu_state = MenuState::TUTOR;
							break;
						case 2:
							menu_state = MenuState::ABOUT;
							break;
						case 3:
							exit(0);
						default:
							break;
					}
				}
			}
			else if(menu_state == MenuState::TUTOR) {
				draw_gui_rect(0, 0, screen_width(), screen_height(), "How To Play");
				if(get_key(engine::Key::ENTER).pressed)
					menu_state = MenuState::MAIN;
			}
			else if(menu_state == MenuState::ABOUT) {
				draw_gui_rect(0, 0, screen_width(), screen_height(), "About");
				if(get_key(engine::Key::ENTER).pressed)
					menu_state = MenuState::MAIN;
			}
		}

		if(state == State::RUNNING) {
			if (get_key(engine::Key::D).pressed) {
				player.set_dir(1, 0);
				if(!level.get_tile((player.pos.x + 16) / 8, player.pos.y / 8)->is_solid)
					player.move();
			}
			
			if (get_key(engine::Key::S).pressed) {
				player.set_dir(0, 1);
				if(!level.get_tile(player.pos.x / 8, (player.pos.y + 16) / 8)->is_solid)
					player.move();
			}

			if (get_key(engine::Key::A).pressed) {
				player.set_dir(-1, 0);
				if(!level.get_tile((player.pos.x - 16) / 8, player.pos.y / 8)->is_solid)
					player.move();
			}
			
			if (get_key(engine::Key::W).pressed) {
				player.set_dir(0, -1);
				if(!level.get_tile(player.pos.x / 8, (player.pos.y - 16) / 8)->is_solid)
					player.move();
			}

			for (int y = 0; y < screen_height() / 8; ++y) {
				for (int x = 0; x < screen_width() / 8; ++x) {
					if((x != 0 && x != (screen_width() / 8)-1) && (y != 0 && y != (screen_width() / 8)-1))
						draw_sprite(level.get_tile(x,y)->pos.x, level.get_tile(x,y)->pos.y, level.get_tile(x,y)->get_sprite(level.get_tile(x,y-1), 
																															level.get_tile(x,y+1), 
																															level.get_tile(x-1,y), 
																															level.get_tile(x+1,y)), 1, 0);
				}
			}


			draw_sprite(player.pos.x, player.pos.y, player.get_sprite(), 1, 0);

			/*
			std::string mouse_x = std::to_string(get_mouse_x());
			std::string mouse_y = std::to_string(get_mouse_y());
			draw_string(0, 0, "mouse x:" + mouse_x, engine::WHITE);
			draw_string(0, 8, "mouse y:" + mouse_y, engine::WHITE);

			std::string dir_x = std::to_string(dir.x);
			std::string dir_y = std::to_string(dir.y);
			draw_string(0, 16, "dir: (" + dir_x + ", " + dir_y + ")", engine::WHITE);

			draw_string(0, 24, "block standing:" + level.get_tile(player_x/8, player_y/8)->block_type, engine::WHITE);
			draw_string(0, 32, "block infront:" + level.get_tile(player_x/8 + dir.x*2, player_y/8 + dir.y*2)->block_type, engine::WHITE);

			int x_pos = get_mouse_x()/8;
			int y_pos = get_mouse_y()/8;
			draw_rect(x_pos*8, y_pos*8, 7, 7, engine::BLACK);
			*/

			if(get_key(engine::Key::SPACE).pressed) {
				frames_to_appear = 16;
				level.get_tile((player.pos.x/8) + player.dir.x*2, (player.pos.y/8) + player.dir.y*2)->hit(1);
				level.get_tile((player.pos.x/8) + player.dir.x*2+1, (player.pos.y/8) + player.dir.y*2)->hit(1);
				level.get_tile((player.pos.x/8) + player.dir.x*2, (player.pos.y/8) + player.dir.y*2+1)->hit(1);
				level.get_tile((player.pos.x/8) + player.dir.x*2+1, (player.pos.y/8) + player.dir.y*2+1)->hit(1);
			}

			/*
			if(frames_to_appear > 0) {
				frames_to_appear--;
				if(level.get_tile(player_x/8 + dir.x*2, player_y/8 + dir.y*2)->is_solid)
					draw_sprite(player_x + dir.x*2*8, player_y + dir.y*2*8, new engine::Sprite("demo/resources/hit.png"), 1, 0);
				else {
					if(dir.x == 1)
						draw_sprite(player_x + dir.x*2*8, player_y + dir.y*2*8, new engine::Sprite("demo/resources/empty-hit-flipped.png"), 1, 0);
					else if(dir.x == -1)
						draw_sprite(player_x + dir.x*2*8, player_y + dir.y*2*8, new engine::Sprite("demo/resources/empty-hit-flipped.png"), 1, 1);
					else if(dir.y == 1)
						draw_sprite(player_x + dir.x*2*8, player_y + dir.y*2*8, new engine::Sprite("demo/resources/empty-hit.png"), 1, -1);
					else 
						draw_sprite(player_x + dir.x*2*8, player_y + dir.y*2*8, new engine::Sprite("demo/resources/empty-hit.png"), 1, 0);
				}
			}
			*/

			//draw_sprite(0, screen_height()-8, new engine::Sprite("demo/resources/heart.png"), 1, 0);
			//draw_sprite(8, screen_height()-8, new engine::Sprite("demo/resources/heart.png"), 1, 0);
			//draw_sprite(16, screen_height()-8, new engine::Sprite("demo/resources/heart.png"), 1, 0);
		}

		return true;
	}
};

int main() {
    Engine3D demo;

    if(demo.construct(256, 240, 4, 4))
        demo.start();

    return 0;
}