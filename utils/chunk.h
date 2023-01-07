#include "plain.h"
#include <vector>

#ifndef CHUNK_DEF
#define CHUNK_DEF

using namespace std;

class Chunk {
public:
    vector<Plain> plains;

    Plain plain1;
    Plain plain2;
    Plain plain3;
    Plain plain4;
    Plain plain5;
    Plain plain6;
    Plain plain7;
    Plain plain8;
    Plain plain9;

    Chunk(float x_offset, float z_offset) {
        float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        plain1.top_right   = { Vector3D(0.0f + x_offset, r1, 0.0f + z_offset), Vector3D(0.0f + x_offset, r3, 1.0f + z_offset), Vector3D(1.0f + x_offset, r2, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain1.bottom_left = { Vector3D(0.0f + x_offset, r1, 0.0f + z_offset), Vector3D(1.0f + x_offset, r2, 1.0f + z_offset), Vector3D(1.0f + x_offset, r4, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

        plain2.top_right   = { Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain2.bottom_left = { Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

        plain1.right_neighbour = &plain2;
        plain2.set_top_right(plain1.get_top_left(true));
        plain2.set_bottom_right(plain1.get_bottom_left());

        plain3.top_right   = { Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain3.bottom_left = { Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,};

        plain1.left_neighbour = &plain3;
        plain3.set_top_left(plain1.get_top_right());
        plain3.set_bottom_left(plain1.get_bottom_right(true));

        plain4.top_right   = { Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain4.bottom_left = { Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

        plain1.top_neighbour = &plain4;
        plain4.set_bottom_left(plain1.get_top_left(true));
        plain4.set_bottom_right(plain1.get_top_right());

        plain5.top_right   = { Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain5.bottom_left = { Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, -1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};
            
        plain1.bottom_neighbour = &plain5;
        plain5.set_top_left(plain1.get_bottom_left());
        plain5.set_top_right(plain1.get_bottom_right(true));

        plain6.top_right   = { Vector3D(1.0f + x_offset, 0, -1.0f + z_offset), Vector3D(1.0f + x_offset, 0, 0.0f + z_offset), Vector3D(2.0f + x_offset, 0, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain6.bottom_left = { Vector3D(1.0f + x_offset, 0, -1.0f + z_offset), Vector3D(2.0f + x_offset, 0, 0.0f + z_offset), Vector3D(2.0f + x_offset, 0, -1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};

        plain1.bottom_left_neighbour = &plain6;
        plain6.set_top_right(plain1.get_bottom_left());

        plain7.top_right   = { Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 2.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain7.bottom_left = { Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};
            
        plain1.top_left_neighbour = &plain7;
        plain7.set_bottom_right(plain1.get_top_left(true));

        plain8.top_right   = { Vector3D(-1.0f + x_offset, 0.0f, -1.0f + z_offset), Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain8.bottom_left = { Vector3D(-1.0f + x_offset, 0.0f, -1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};
            
        plain1.bottom_right_neighbour = &plain8;
        plain8.set_top_left(plain1.get_bottom_right(true));

        plain9.top_right   = { Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(-1.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset), 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,};
        plain9.bottom_left = { Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset), Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,};
            
        plain1.top_right_neighbour = &plain9;
        plain9.set_bottom_left(plain1.get_top_right());

        plains.push_back(plain1);
        plains.push_back(plain2);
        plains.push_back(plain3);
        plains.push_back(plain4);
        plains.push_back(plain5);
        plains.push_back(plain6);
        plains.push_back(plain7);
        plains.push_back(plain8);
        plains.push_back(plain9);
    }
};

#endif