#ifndef TILE_DEF
#define TILE_DEF

#include <string>

class Tile {
public:
    engine::int_vector_2d pos;

    std::string base_path;
    std::string block_type;

    bool is_solid;
    bool is_enterable = false;

    Tile(std::string path, engine::int_vector_2d p, bool solid, std::string type) {
        base_path = path;
        pos = p;
        is_solid = solid;
        block_type = type;
    }

    virtual ~Tile() = default;

    engine::Sprite* get_sprite(Tile* ut, Tile* dt, Tile* lt, Tile* rt) {
        if(block_type == "stone" || block_type == "grass" || block_type == "hardened-stone") {
            bool u, d, l, r;

            u = (ut->block_type == block_type);
            d = (dt->block_type == block_type);
            l = (lt->block_type == block_type);
            r = (rt->block_type == block_type);

            if (!u && !l)
                return new engine::Sprite(base_path + block_type + "-top-left.png");
            else {
                if(u && d && !l)
                    return new engine::Sprite(base_path + block_type + "-left.png");
                else if (l && r && !u)
                    return new engine::Sprite(base_path + block_type + "-top.png");
            }

            if (!u && !r)
                return new engine::Sprite(base_path + block_type + "-top-right.png");

            if (!d && !l)
                return new engine::Sprite(base_path + block_type + "-bottom-left.png");

            if (!d && !r)
                return new engine::Sprite(base_path + block_type + "-bottom-right.png");
            else {
                if(u && d && !r)
                    return new engine::Sprite(base_path + block_type + "-right.png");
                else if (l && r && !d)
                    return new engine::Sprite(base_path + block_type + "-bottom.png");
            }
        }
        else if(block_type == "tree" || block_type == "stair" || block_type == "stair-up" || block_type == "oven" || block_type == "chest" || block_type == "crafting-table" || block_type == "cobble" || block_type == "ore") {
            if(pos.x%16 == 8 && pos.y%16 == 8)
                return new engine::Sprite(base_path + block_type + "-bottom-right.png");
            else if(pos.x%16 == 8 && pos.y%16 == 0)
                return new engine::Sprite(base_path + block_type + "-top-right.png");
            else if(pos.x%16 == 0 && pos.y%16 == 8)
                return new engine::Sprite(base_path + block_type + "-bottom-left.png");
            else if(pos.x%16 == 0 && pos.y%16 == 0)
                return new engine::Sprite(base_path + block_type + "-top-left.png");
        }
        return new engine::Sprite(base_path + block_type + ".png");
    }

private:
};

#endif