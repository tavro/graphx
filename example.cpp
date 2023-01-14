#define APPLICATION_DEF
#include "engine.h"
#include "PerlinNoise.hpp"
#include "demo/state.h"
#include "demo/tile.h"
#include "demo/breakable.h"
#include "demo/chunk.h"
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

	Chunk level;
	
	int chunk_x = 0;
	int chunk_y = 0;

	Chunk* chunks[10][10];
	Chunk* cave_chunks[10][10];

	Player player;

	Effect* hit_effect = nullptr;

	vector<Item*> items;

	ComponentUI tutorial{0, 0, screen_width(), screen_height(), "HOW TO PLAY", engine::DARK_GREY, engine::BLUE, engine::BLACK};
	ComponentUI about{0, 0, screen_width(), screen_height(), "ABOUT", engine::DARK_GREY, engine::BLUE, engine::BLACK};

	InventoryUI inv_ui{&player.inventory};
	CraftingUI craft_ui{&player.inventory};

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
			draw_sprite(inv_ui.component.pos.x + y_offset, inv_ui.component.pos.y + x_offset, new engine::Sprite("demo/resources/" + line->words[0].text + ".png"));
			y_offset += 8 + 4;
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
			draw_sprite(craft_ui.component.pos.x + y_offset, craft_ui.component.pos.y + x_offset, new engine::Sprite("demo/resources/" + line->words[0].text.substr(0, line->words[0].len-1) + ".png"));
			y_offset += 8 + 4;
			for(Word word : line->words) {
				draw_string(craft_ui.component.pos.x + y_offset, craft_ui.component.pos.y + x_offset, word.text, line->color);
				y_offset += word.len*8+8;
			}
		}
	}

public:
	bool on_create() override {
		chunks[chunk_x][chunk_y] = new Chunk();
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

					if(player.pixel_pos.x == 0 && chunk_x > 0) {
						chunk_x--;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(screen_width()-16, player.pixel_pos.y);
					}
					else if(player.pixel_pos.x == 0 && chunk_x == 0) { 
						chunk_x=9;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(screen_width()-16, player.pixel_pos.y);
					}
					
					if (!chunks[chunk_x][chunk_y]->get_tile((player.pixel_pos.x-1)/8, ((player.pixel_pos.y+OFFSET)/8))->is_solid && 
						!chunks[chunk_x][chunk_y]->get_tile((player.pixel_pos.x-1)/8, ((player.pixel_pos.y-OFFSET)/8)+2)->is_solid) {
						player.move();
					}
					else if(!chunks[chunk_x][chunk_y]->get_tile((player.pixel_pos.x-1)/8, ((player.pixel_pos.y+OFFSET)/8))->is_enterable) {
						//CHANGE TO CAVE
					}
				}
			}
			else if (get_key(engine::Key::D).held) {
				x_timer += elapsed_time;
				if(x_timer >= 0.01f) {
					x_timer = 0.0f;
					player.set_dir(1, 0);

					if(player.pixel_pos.x == screen_width()-17 && chunk_x < 9) {
						chunk_x++;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(0, player.pixel_pos.y);
					}
					else if(player.pixel_pos.x == screen_width()-17 && chunk_x == 9) {
						chunk_x=0;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(0, player.pixel_pos.y);
					}

					if (!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x+1)/8)+2, ((player.pixel_pos.y-OFFSET)/8)+2)->is_solid && 
						!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x+1)/8)+2, ((player.pixel_pos.y+OFFSET)/8))->is_solid) {
						player.move();
					}
				}
			}
			
			if (get_key(engine::Key::W).held) {
				y_timer += elapsed_time;
				if(y_timer >= 0.01f) {
					y_timer = 0.0f;
					player.set_dir(0, -1);

					if(player.pixel_pos.y == 0 && chunk_y > 0) {
						chunk_y--;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(player.pixel_pos.x, screen_height()-16);
					}
					else if(player.pixel_pos.y == 0 && chunk_y == 0) {
						chunk_y=9;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(player.pixel_pos.x, screen_height()-16);
					}

					if (!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x+OFFSET)/8), (player.pixel_pos.y-1)/8)->is_solid && 
						!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x-OFFSET)/8)+2, (player.pixel_pos.y-1)/8)->is_solid) {
						player.move();
					}
				}
			}
			else if (get_key(engine::Key::S).held) {
				y_timer += elapsed_time;
				if(y_timer >= 0.01f) {
					y_timer = 0.0f;
					player.set_dir(0, 1);

					if(player.pixel_pos.y == screen_height()-16 && chunk_y < 9) {
						chunk_y++;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(player.pixel_pos.x, 8);
					}
					else if(player.pixel_pos.y == screen_height()-16 && chunk_y == 9) {
						chunk_y=0;
						if(!chunks[chunk_x][chunk_y])
							chunks[chunk_x][chunk_y] = new Chunk();
						player.set_position(player.pixel_pos.x, 8);
					}

					if (!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x-OFFSET)/8)+2, ((player.pixel_pos.y+1)/8)+2)->is_solid && 
						!chunks[chunk_x][chunk_y]->get_tile(((player.pixel_pos.x+2)/8), ((player.pixel_pos.y+1)/8)+2)->is_solid) {
						player.move();
					}
				}
			}

			for (int y = 0; y < screen_height() / 8; ++y) {
				for (int x = 0; x < screen_width() / 8; ++x) {
					if((x != 0 && x != (screen_width() / 8)-1) && (y != 0 && y != (screen_width() / 8)-1))
						draw_sprite(chunks[chunk_x][chunk_y]->get_tile(x,y)->pos.x, chunks[chunk_x][chunk_y]->get_tile(x,y)->pos.y, chunks[chunk_x][chunk_y]->get_tile(x,y)->get_sprite(chunks[chunk_x][chunk_y]->get_tile(x,y-1), 
																															chunks[chunk_x][chunk_y]->get_tile(x,y+1), 
																															chunks[chunk_x][chunk_y]->get_tile(x-1,y), 
																															chunks[chunk_x][chunk_y]->get_tile(x+1,y)), 1, 0);
				}
			}

			for(int i = 0; i < items.size(); i++) {
				draw_sprite(items[i]->pos.x, items[i]->pos.y, items[i]->sprite, 1, 0);
				items[i]->tick();

				if (((items[i]->pos.x >= player.pixel_pos.x && items[i]->pos.x <= player.pixel_pos.x+16) &&
					(items[i]->pos.y >= player.pixel_pos.y && items[i]->pos.y <= player.pixel_pos.y+16)) || 
					((items[i]->pos.x+8 >= player.pixel_pos.x && items[i]->pos.x+8 <= player.pixel_pos.x+16) &&
					(items[i]->pos.y+8 >= player.pixel_pos.y && items[i]->pos.y+8 <= player.pixel_pos.y+16))) {
						player.inventory.add_item(items[i]->name);
						items.erase(items.begin() + i);
					}
			}
			draw_sprite(player.pixel_pos.x, player.pixel_pos.y, player.get_sprite(), 1, 0);
			if(player.inventory.get_active_item() == "crafting-table" || player.inventory.get_active_item() == "oven" || player.inventory.get_active_item() == "chest") {
				draw_sprite(player.pixel_pos.x, player.pixel_pos.y - 16, new engine::Sprite("demo/resources/" + player.inventory.get_active_item() + "-tile.png"), 1, 0);
			}
				
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
			
			if (get_key(engine::Key::SPACE).pressed) {
				if(player.inventory.get_active_item() == "crafting-table" || player.inventory.get_active_item() == "oven" || player.inventory.get_active_item() == "chest") {
					if(player.inventory.get_item_amount(player.inventory.get_active_item()) > 0) {
						for(int i = 0; i < 2; i++) {
							for(int j = 0; j < 2; j++) {
								int tilex = (drawx + player.dir.x*16)/8 + 1*i;
								int tiley = (drawy + player.dir.y*16)/8 + 1*j;
								Tile* tile = new Tile("demo/resources/", engine::int_vector_2d(tilex*8, tiley*8), true, player.inventory.get_active_item());
								chunks[chunk_x][chunk_y]->set_tile(tilex, tiley, tile);
							}
						}
						player.inventory.remove_item(player.inventory.get_active_item(), 1);
					}
				}
				else {
					hit_effect = new Effect(drawx + player.dir.x*16, drawy + player.dir.y*16, new engine::Sprite("demo/resources/hit.png"), 16);

					for(int i = 0; i < 2; i++) {
						for(int j = 0; j < 2; j++) {
							int tilex = (drawx + player.dir.x*16)/8 + 1*i;
							int tiley = (drawy + player.dir.y*16)/8 + 1*j;

							BreakableTile *b=dynamic_cast<BreakableTile*>(chunks[chunk_x][chunk_y]->get_tile(tilex, tiley));
							if(b) {
								if (b->hit()) {
									items.push_back(b->drop_item);
									Tile* tile = new Tile("demo/resources/", engine::int_vector_2d(tilex*8, tiley*8), false, "dirt");
									chunks[chunk_x][chunk_y]->set_tile(tilex, tiley, tile);
								}
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

			if(player.inventory.get_active_item() != "nothing" && !inv_ui.is_open) {
				fill_rect(screen_width()-128, screen_height()-16, 128, 16, engine::BLUE);
				draw_rect(screen_width()-128, screen_height()-16, 128-1, 16-1, engine::DARK_GREY);
				draw_string(screen_width()-128+12, screen_height()-16+4, player.inventory.get_active_item(), engine::BLACK);
				draw_sprite(screen_width()-128+4, screen_height()-16+4, new engine::Sprite("demo/resources/" + player.inventory.get_active_item() + ".png"));
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