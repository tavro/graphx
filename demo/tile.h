#include <string>

class Tile {
public:
    engine::int_vector_2d pos;

    std::string base_path;
    std::string block_type;

    bool is_solid;
    bool is_breakable;

    int hitpoints = 3;

    Tile(std::string path, engine::int_vector_2d p, bool solid, bool breakable, std::string type) {
        base_path = path;
        pos = p;
        is_solid = solid;
        is_breakable = breakable;
        block_type = type;
    }

    void hit(int amount) {
        if(is_breakable) {
            if(--hitpoints <= 0) {
                block_type = "dirt";
                is_solid = false;
                is_breakable = false;
            }
        }
    }

    engine::Sprite* get_sprite(Tile* ut, Tile* dt, Tile* lt, Tile* rt) {
        if(block_type == "stone" || block_type == "grass") {
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
        return new engine::Sprite(base_path + block_type + ".png");
    }

private:
};