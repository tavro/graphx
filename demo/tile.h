#include <string>

class Tile {
public:
    engine::Sprite* sprite;
    engine::int_vector_2d pos;

    bool is_solid;

    Tile(std::string sprite_path, engine::int_vector_2d p, bool solid) {
        sprite = new engine::Sprite(sprite_path);
        pos = p;
        is_solid = solid;
    }

private:
};