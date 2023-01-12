class Effect {
public:
    engine::Sprite* sprite;

    engine::int_vector_2d pos;

    int appear_time;
    bool is_showing;
    
    Effect(int x, int y, engine::Sprite* spr, int time) {
        pos.x = x;
        pos.y = y;
        sprite = spr;
        appear_time = time;
    }

    void tick() {
        appear_time--;
        if(appear_time <= 0)
            is_showing = false;
    }
};