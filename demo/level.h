#include "../engine.h"
#include "item.h"
#include "breakable.h"
#include "../PerlinNoise.hpp"

class Chunk {
public:
    const siv::PerlinNoise perlin{ std::random_device{} };
    int width = 32;
    int height = 30;

    Tile* tiles[32][30];

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
                    tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, "dirt");
                    tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, "dirt");
                    tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, "dirt");
                    tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, "dirt");
                }
				else if(noise < 0.7f) {
                    tiles[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, "grass");
                    tiles[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, "grass");
                    tiles[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, "grass");
                    tiles[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, "grass");
                }
                else if(noise < 0.8f) {
                    tiles[x][y]     = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, y*8), true, "tree", new Item(x*8, y*8, new engine::Sprite("demo/resources/wood.png"), "wood"));
                    tiles[x+1][y]   = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), true, "tree", new Item((x+1)*8, y*8, new engine::Sprite("demo/resources/wood.png"), "wood"));
                    tiles[x][y+1]   = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), true, "tree", new Item(x*8, (y+1)*8, new engine::Sprite("demo/resources/wood.png"), "wood"));
                    tiles[x+1][y+1] = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), true, "tree", new Item((x+1)*8, (y+1)*8, new engine::Sprite("demo/resources/wood.png"), "wood"));
                }
                else {
                    tiles[x][y]     = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, y*8), true, "stone", new Item(x*8, y*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x+1][y]   = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), true, "stone", new Item((x+1)*8, y*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x][y+1]   = new BreakableTile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), true, "stone", new Item(x*8, (y+1)*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                    tiles[x+1][y+1] = new BreakableTile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), true, "stone", new Item((x+1)*8, (y+1)*8, new engine::Sprite("demo/resources/rock.png"), "rock"));
                }
			}
		}
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