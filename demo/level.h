#include "../engine.h"
#include "../PerlinNoise.hpp"

class Level {
public:
    const siv::PerlinNoise perlin{ std::random_device{} };
    int width = 32;
    int height = 30;

    Tile* world[32][30];

    void generate() {
        for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const double noise = perlin.octave2D_01(x * 0.1f, y * 0.1f, 10);
				
				if(noise < 0.4f)
                    world[x][y] = new Tile("demo/resources/dirt.png", engine::int_vector_2d(x*8, y*8), false);
				else //if(noise < 0.7f)
                    world[x][y] = new Tile("demo/resources/grass.png", engine::int_vector_2d(x*8, y*8), false);
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