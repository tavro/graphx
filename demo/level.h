#include "../engine.h"
#include "../PerlinNoise.hpp"

class Level {
public:
    const siv::PerlinNoise perlin{ std::random_device{} };
    int width = 32;
    int height = 30;

    Tile* world[32][30];

    void generate() {
        for (int y = 0; y < height; y+=2) {
			for (int x = 0; x < width; x+=2) {
				const double noise = perlin.octave2D_01(x * 0.1f, y * 0.1f, 10);
				
                if(noise < 0.25f) {
                    world[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, false, "water");
                    world[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, false, "water");
                    world[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, false, "water");
                    world[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, false, "water");
                }
				else if(noise < 0.4f) {
                    world[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, false, "dirt");
                    world[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, false, "dirt");
                    world[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, false, "dirt");
                    world[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, false, "dirt");
                }
				else if(noise < 0.7f) {
                    world[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), false, false, "grass");
                    world[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), false, false, "grass");
                    world[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), false, false, "grass");
                    world[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), false, false, "grass");
                }
                else {
                    world[x][y]     = new Tile("demo/resources/", engine::int_vector_2d(x*8, y*8), true, true, "stone");
                    world[x+1][y]   = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, y*8), true, true, "stone");
                    world[x][y+1]   = new Tile("demo/resources/", engine::int_vector_2d(x*8, (y+1)*8), true, true, "stone");
                    world[x+1][y+1] = new Tile("demo/resources/", engine::int_vector_2d((x+1)*8, (y+1)*8), true, true, "stone");
                }
			}
		}
    }

    Level() {
        generate();
    }

    Tile* get_tile(int x, int y) {
        return world[x][y];
    }
};