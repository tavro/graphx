#include "plain.h"
#include <vector>

#ifndef CHUNK_DEF
#define CHUNK_DEF

using namespace std;

float get_random_float() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

class Chunk {
public:
    vector<Plain> plains;

    Chunk(float x_offset, float z_offset) {
        float r1 = get_random_float();
        float r2 = get_random_float();

        Plain plain1;
        plain1.top_right   = { 
            Vector3D(0.0f + x_offset, r1, 0.0f + z_offset), 
            Vector3D(0.0f + x_offset, get_random_float(), 1.0f + z_offset), 
            Vector3D(1.0f + x_offset, r2, 1.0f + z_offset)
        };
        plain1.bottom_left = { 
            Vector3D(0.0f + x_offset, r1, 0.0f + z_offset), 
            Vector3D(1.0f + x_offset, r2, 1.0f + z_offset), 
            Vector3D(1.0f + x_offset, get_random_float(), 0.0f + z_offset)
        };

        Plain plain2;
        plain2.top_right   = { 
            Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset)
        };
        plain2.bottom_left = { 
            Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 0.0f + z_offset)
        };

        plain1.right_neighbour = &plain2;
        plain2.set_top_right(plain1.get_top_left(true));
        plain2.set_bottom_right(plain1.get_bottom_left());

        Plain plain3;
        plain3.top_right   = { 
            Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset)
        };
        plain3.bottom_left = { 
            Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset)
        };

        plain1.left_neighbour = &plain3;
        plain3.set_top_left(plain1.get_top_right());
        plain3.set_bottom_left(plain1.get_bottom_right(true));

        Plain plain4;
        plain4.top_right   = { 
            Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset)
        };
        plain4.bottom_left = { 
            Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset)
        };

        plain1.top_neighbour = &plain4;
        plain4.set_bottom_left(plain1.get_top_left(true));
        plain4.set_bottom_right(plain1.get_top_right());

        Plain plain5;
        plain5.top_right   = { 
            Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset)
        };
        plain5.bottom_left = { 
            Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, -1.0f + z_offset)
        };
            
        plain1.bottom_neighbour = &plain5;
        plain5.set_top_left(plain1.get_bottom_left());
        plain5.set_top_right(plain1.get_bottom_right(true));

        Plain plain6;
        plain6.top_right   = { 
            Vector3D(1.0f + x_offset, 0, -1.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0, 0.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0, 0.0f + z_offset)
        };
        plain6.bottom_left = { 
            Vector3D(1.0f + x_offset, 0, -1.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0, 0.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0, -1.0f + z_offset)
        };

        plain1.bottom_left_neighbour = &plain6;
        plain6.set_top_right(plain1.get_bottom_left());

        Plain plain7;
        plain7.top_right   = { 
            Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(1.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 2.0f + z_offset)
        };
        plain7.bottom_left = { 
            Vector3D(1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(2.0f + x_offset, 0.0f, 1.0f + z_offset)
        };
            
        plain1.top_left_neighbour = &plain7;
        plain7.set_bottom_right(plain1.get_top_left(true));

        Plain plain8;
        plain8.top_right   = { 
            Vector3D(-1.0f + x_offset, 0.0f, -1.0f + z_offset), 
            Vector3D(-1.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset)
        };
        plain8.bottom_left = { 
            Vector3D(-1.0f + x_offset, 0.0f, -1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 0.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, -1.0f + z_offset)
        };
            
        plain1.bottom_right_neighbour = &plain8;
        plain8.set_top_left(plain1.get_bottom_right(true));

        Plain plain9;
        plain9.top_right   = { 
            Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(-1.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset)
        };
        plain9.bottom_left = { 
            Vector3D(-1.0f + x_offset, 0.0f, 1.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 2.0f + z_offset), 
            Vector3D(0.0f + x_offset, 0.0f, 1.0f + z_offset)
        };
            
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