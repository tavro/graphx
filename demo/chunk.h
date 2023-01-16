#include "../engine.h"
#include "item.h"
#include "breakable.h"
#include "../PerlinNoise.hpp"
#include "chunk_type.h"

class Chunk {
public:
    const siv::PerlinNoise perlin{ std::random_device{} };

    ChunkType type;
    
    int width = 32;
    int height = 30;

    Tile* tiles[32][30];

    std::string base_type = "dirt"; //"cobble";
    std::string layer_type = "grass"; //"hardened-stone";

    std::string stair_type = "stair"; //"stair-up";

    std::string mineral_type = "tree"; //"ore";
    std::string mineral_droppable = "wood";

    void place_staircase() {
        int x = rand() % 31;
        int y = rand() % 29;

        if(x%2 != 0)
            x++;

        if(y%2 != 0)
            y++;

        tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, stair_type);
        tiles[x][y]->is_enterable = true;

        tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, stair_type);
        tiles[x+1][y]->is_enterable = true;

        tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, stair_type);
        tiles[x][y+1]->is_enterable = true;

        tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, stair_type);
        tiles[x+1][y+1]->is_enterable = true;
    }

    void generate() {
        for (int y = 0; y < height; y+=2) {
			for (int x = 0; x < width; x+=2) {
				const double noise = perlin.octave2D_01(x * 0.1f, y * 0.1f, 10);
				
                if(noise < 0.25f) {
                    tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, "water");
                    tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, "water");
                    tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, "water");
                    tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, "water");
                }
				else if(noise < 0.4f) {
                    tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, base_type);
                    tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, base_type);
                    tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, base_type);
                    tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, base_type);
                }
				else if(noise < 0.7f) {
                    tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, layer_type);
                    tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, layer_type);
                    tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, layer_type);
                    tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, layer_type);
                }
                else if(noise < 0.8f) {
                    tiles[x][y]     = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, y*8), true, mineral_type, new Item(x*8, y*8, new engine::Sprite("demo/resources/"+mineral_droppable+".png"), mineral_droppable));
                    tiles[x+1][y]   = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), true, mineral_type, new Item((x+1)*8, y*8, new engine::Sprite("demo/resources/"+mineral_droppable+".png"), mineral_droppable));
                    tiles[x][y+1]   = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), true, mineral_type, new Item(x*8, (y+1)*8, new engine::Sprite("demo/resources/"+mineral_droppable+".png"), mineral_droppable));
                    tiles[x+1][y+1] = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), true, mineral_type, new Item((x+1)*8, (y+1)*8, new engine::Sprite("demo/resources/"+mineral_droppable+".png"), mineral_droppable));
                }
                else {
                    tiles[x][y]     = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, y*8), true, "stone", new Item(x*8, y*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x+1][y]   = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), true, "stone", new Item((x+1)*8, y*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x][y+1]   = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), true, "stone", new Item(x*8, (y+1)*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x+1][y+1] = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), true, "stone", new Item((x+1)*8, (y+1)*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                }
			}
		}
        place_staircase();
    }

    Chunk(ChunkType chunk_type) {
        type = chunk_type;

        if(type == ChunkType::OVERWORLD) {
            base_type = "dirt"; //"cobble";
            layer_type = "grass"; //"hardened-stone";

            stair_type = "stair"; //"stair-up";

            mineral_type = "tree"; //"ore";
            mineral_droppable = "wood";
        }
        else {
            base_type = "cobble";
            layer_type = "hardened-stone";

            stair_type = "stair-up";

            mineral_type = "ore";
            mineral_droppable = "wood"; //TODO
        }

        generate();
    }

    Chunk() {
        generate();
    }

    Tile* get_tile(int x, int y) {
        return tiles[x][y];
    }

    void set_tile(int x, int y, Tile* tile) {
        tiles[x][y] = tile;
    }
};