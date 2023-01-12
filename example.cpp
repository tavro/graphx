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
#include "demo/gui/ui_inventory.h"
#include "demo/gui/ui_crafting.h"
#include "demo/menu/menu.h"
#include "demo/gui/ui_component.h"
#include <typeinfo>

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

	ComponentUI tutorial{0, 0, screen_width(), screen_height(), "HOW TO PLAY", engine::DARK_GREY, engine::BLUE, engine::BLACK};
	ComponentUI about{0, 0, screen_width(), screen_height(), "ABOUT", engine::DARK_GREY, engine::BLUE, engine::BLACK};

	InventoryUI inv_ui{&player.inventory};
	CraftingUI craft_ui{&player.inventory, &oven_recipe};

	const int OFFSET = 2;

	float x_timer = 0.0f;
	float y_timer = 0.0f;

    void draw(ComponentUI component) {
        fill_rect(component.pos.x, component.pos.y, component.size.x, component.size.y, component.inside_color);
		draw_rect(component.pos.x, component.pos.y, component.size.x-1, component.size.y-1, component.border_color);

		int title_len = component.title.length()*8;
		draw_string(component.pos.x+(component.size.x/2)-title_len/2, component.pos.y+2, component.title, component.text_color);

		std::string exit_str = "ENTER TO CLOSE";
		int exit_str_len = exit_str.length()*8;
		draw_string(component.pos.x+(component.size.x/2)-exit_str_len/2, component.pos.y+component.size.y-8-1, exit_str, component.text_color);
    }

	void draw_inventory() {
		draw(inv_ui.component);
		int x_offset = 4;
		for(Line* line : inv_ui.get_lines()) {
			x_offset += 8;
			int y_offset = 4;
			for(Word word : line->words) {
				draw_string(inv_ui.component.pos.x + y_offset, inv_ui.component.pos.y + x_offset, word.text, line->color);
				y_offset += word.len*8+8;
			}
		}
	}

	void draw_crafting() {
		draw(craft_ui.component);
		int x_offset = 4;
		for(Line* line : craft_ui.get_lines()) {
			x_offset += 8;
			int y_offset = 4;
			for(Word word : line->words) {
				draw_string(craft_ui.component.pos.x + y_offset, craft_ui.component.pos.y + x_offset, word.text, line->color);
				y_offset += word.len*8+8;
			}
		}
	}

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
				draw(tutorial);
				if(get_key(engine::Key::ENTER).pressed)
					menu.state = MenuState::MAIN;
			}
			else if(menu.state == MenuState::ABOUT) {
				draw(about);
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

						BreakableTile *b=dynamic_cast<BreakableTile*>(level.get_tile(tilex, tiley));
						if(b) {
							if (b->hit()) {
								items.push_back(b->drop_item);
								Tile* tile = new Tile("demo/resources/", engine::int_vector_2d(tilex*8, tiley*8), false, "dirt");
								level.set_tile(tilex, tiley, tile);
							}
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

			if(get_key(engine::Key::I).pressed)
				inv_ui.is_open = !inv_ui.is_open;

			if(inv_ui.is_open) {
				draw_inventory();
				if(get_key(engine::Key::DOWN).pressed)
					player.inventory.next_item();
				else if(get_key(engine::Key::UP).pressed)
					player.inventory.previous_item();
			}

			if(get_key(engine::Key::C).pressed)
				craft_ui.is_open = !craft_ui.is_open;
			
			if(craft_ui.is_open) {
				draw_crafting();
				if(get_key(engine::Key::DOWN).pressed)
					craft_ui.next_recipe();
				else if(get_key(engine::Key::UP).pressed)
					craft_ui.previous_recipe();
				if(get_key(engine::Key::ENTER).pressed)
					craft_ui.craft_active();
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