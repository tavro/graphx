#define APPLICATION_DEF
#include "engine.h"
#include "PerlinNoise.hpp"
#include "demo/state.h"
#include "demo/tile.h"
#include "demo/breakable.h"
#include "demo/level.h"
#include "demo/effect.h"
#include "demo/player.h"
#include "demo/recipe.h"
#include "demo/gui/ui_line.h"
#include "demo/menu/menu.h"

using namespace std;

class Engine2D : public engine::Engine {
public:
    Engine2D() {
        app_name = "demo";
    }

private:
	State state = State::MENU;
	Menu menu{{"start", "how to play", "about", "quit"}, engine::DARK_GREY, engine::WHITE};

	Level level;
	Player player;

	Effect* hit_effect = nullptr;

	vector<Item*> items;

	ItemStack* ovens = new ItemStack("oven");
	RecipeItem needed{"rock", 8};
	Recipe oven_recipe{ovens, needed};

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

	const int OFFSET = 2;

	float x_timer = 0.0f;
	float y_timer = 0.0f;

public:
	bool on_create() override {
		
		return true;
	}

	bool on_update(float elapsed_time) override {
		if(state == State::MENU) {
			fill_rect(0, 0, screen_width(), screen_height(), engine::BLACK);
			if(menu.state == MenuState::MAIN) {
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
							menu.state = MenuState::TUTOR;
							break;
						case 2:
							menu.state = MenuState::ABOUT;
							break;
						case 3:
							exit(0);
						default:
							break;
					}
				}
			}
			else if(menu.state == MenuState::TUTOR) {
				draw_gui_rect(0, 0, screen_width(), screen_height(), "How To Play");
				if(get_key(engine::Key::ENTER).pressed)
					menu.state = MenuState::MAIN;
			}
			else if(menu.state == MenuState::ABOUT) {
				draw_gui_rect(0, 0, screen_width(), screen_height(), "About");
				if(get_key(engine::Key::ENTER).pressed)
					menu.state = MenuState::MAIN;
			}
		}

		if(state == State::RUNNING) {
			if (get_key(engine::Key::A).held) {
				x_timer += elapsed_time;
				if(x_timer >= 0.01f) {
					x_timer = 0.0f;
					player.set_dir(-1, 0);

					if (!level.get_tile((player.pixel_pos.x-1)/8, ((player.pixel_pos.y+OFFSET)/8))->is_solid && 
						!level.get_tile((player.pixel_pos.x-1)/8, ((player.pixel_pos.y-OFFSET)/8)+2)->is_solid) {
						player.move();
					}
				}
			}
			else if (get_key(engine::Key::D).held) {
				x_timer += elapsed_time;
				if(x_timer >= 0.01f) {
					x_timer = 0.0f;
					player.set_dir(1, 0);

					if (!level.get_tile(((player.pixel_pos.x+1)/8)+2, ((player.pixel_pos.y-OFFSET)/8)+2)->is_solid && 
						!level.get_tile(((player.pixel_pos.x+1)/8)+2, ((player.pixel_pos.y+OFFSET)/8))->is_solid) {
						player.move();
					}
				}
			}
			
			if (get_key(engine::Key::W).held) {
				y_timer += elapsed_time;
				if(y_timer >= 0.01f) {
					y_timer = 0.0f;
					player.set_dir(0, -1);

					if (!level.get_tile(((player.pixel_pos.x+OFFSET)/8), (player.pixel_pos.y-1)/8)->is_solid && 
						!level.get_tile(((player.pixel_pos.x-OFFSET)/8)+2, (player.pixel_pos.y-1)/8)->is_solid) {
						player.move();
					}
				}
			}
			else if (get_key(engine::Key::S).held) {
				y_timer += elapsed_time;
				if(y_timer >= 0.01f) {
					y_timer = 0.0f;
					player.set_dir(0, 1);

					if (!level.get_tile(((player.pixel_pos.x-OFFSET)/8)+2, ((player.pixel_pos.y+1)/8)+2)->is_solid && 
						!level.get_tile(((player.pixel_pos.x+2)/8), ((player.pixel_pos.y+1)/8)+2)->is_solid) {
						player.move();
					}
				}
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

			for(int i = 0; i < items.size(); i++) {
				draw_sprite(items[i]->pos.x, items[i]->pos.y, items[i]->sprite, 1, 0);
				items[i]->tick();

				//TODO: use new playerpos
				if (((items[i]->pos.x >= player.pixel_pos.x && items[i]->pos.x <= player.pixel_pos.x+16) &&
					(items[i]->pos.y >= player.pixel_pos.y && items[i]->pos.y <= player.pixel_pos.y+16)) || 
					((items[i]->pos.x+8 >= player.pixel_pos.x && items[i]->pos.x+8 <= player.pixel_pos.x+16) &&
					(items[i]->pos.y+8 >= player.pixel_pos.y && items[i]->pos.y+8 <= player.pixel_pos.y+16))) {
						player.inventory.add_item(items[i]->name);
						items.erase(items.begin() + i);
					}
			}
			draw_sprite(player.pixel_pos.x, player.pixel_pos.y, player.get_sprite(), 1, 0);

				
			int drawx = 0; 
			int drawy = 0;

			if((player.tile_pos.x*8)%16 == 8)
				drawx = (player.tile_pos.x*8) + 8*player.dir.x;
			else
				drawx = (player.tile_pos.x*8);

			if((player.tile_pos.y*8)%16 == 8)
				drawy = (player.tile_pos.y*8) + 8*player.dir.y;
			else
				drawy = (player.tile_pos.y*8);

			int multiplier = 1;
			if(player.dir.y == -1)
				multiplier -1;

			if(drawx%16 == 8)
				drawx+=8 * multiplier;
			if(drawy%16 == 8)
				drawy+=8 * multiplier;


			//draw_rect(drawx, drawy, 16, 16, engine::RED);
			//draw_rect(drawx + player.dir.x*16, drawy + player.dir.y*16, 16, 16, engine::BLACK);

			fill_rect(screen_width()-96, screen_height()-16, 96, 16, engine::BLUE);
			draw_rect(screen_width()-96, screen_height()-16, 96-1, 16-1, engine::DARK_GREY);

			std::string item_amount = std::to_string(player.inventory.get_item_amount("rock"));
			draw_string(screen_width()-96+4, screen_height()-16+4, item_amount + " - rock", engine::WHITE);

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

			fill_rect(0, screen_height()-96, 96, 96, engine::BLUE);
			draw_rect(0, screen_height()-96, 96-1, 96-1, engine::DARK_GREY);
			draw_string(0+4, screen_height()-96+4, "OVEN", engine::WHITE);
			for(RecipeItem i : oven_recipe.needed_items) {
				std::string amnt = std::to_string(i.amount);
				draw_string(0+4, screen_height()-96+8+4, i.item_type + ":" + amnt, engine::GREY);
			}
			if(oven_recipe.can_craft(player.inventory)) {
				draw_string(0+4, screen_height()-96+16+4, "CRAFT - C", engine::WHITE);
				if(get_key(engine::Key::C).pressed) {
					player.inventory.add_item_stack(oven_recipe.craft(player.inventory));
				}
			}
			*/
			
			if (get_key(engine::Key::SPACE).pressed) {
				hit_effect = new Effect(drawx + player.dir.x*16, drawy + player.dir.y*16, new engine::Sprite("demo/resources/hit.png"), 16);

				for(int i = 0; i < 2; i++) {
					for(int j = 0; j < 2; j++) {
						int tilex = (drawx + player.dir.x*16)/8 + 1*i;
						int tiley = (drawy + player.dir.y*16)/8 + 1*j;

						//TODO: Check if tile is BreakableTile
						BreakableTile* breakable = (BreakableTile*)level.get_tile(tilex, tiley);
						if (breakable->hit()) {
							items.push_back(breakable->drop_item);
							Tile* tile = new Tile("demo/resources/", engine::int_vector_2d(tilex*8, tiley*8), false, "dirt");
							level.set_tile(tilex, tiley, tile);
						}
					}
				}
			}

			if(hit_effect) {
				hit_effect->tick();
				if(hit_effect->is_showing)
					draw_sprite(hit_effect->pos.x, hit_effect->pos.y, hit_effect->sprite, 1, 0);
				else
					hit_effect = nullptr;
			}
		}

		return true;
	}
};

int main() {
    Engine2D demo;

    if(demo.construct(256, 240, 4, 4))
        demo.start();

    return 0;
}