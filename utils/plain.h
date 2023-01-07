#include "triangle.h"
#include "vector.h"

#ifndef PLAIN_DEF
#define PLAIN_DEF

struct Plain {
    Triangle top_right;
    Triangle bottom_left;

    Plain *left_neighbour;
    Plain *top_neighbour;
    Plain *right_neighbour;
    Plain *bottom_neighbour;

    Plain *bottom_right_neighbour;
    Plain *top_right_neighbour;
    Plain *bottom_left_neighbour;
    Plain *top_left_neighbour;

    Vector3D get_top_right() {
        return top_right.pos[1];
    }

    Vector3D get_bottom_right(bool top) {
        if(top)
            return top_right.pos[0];
        else
            return bottom_left.pos[0];
    }

    Vector3D get_top_left(bool top) {
        if(top)
            return top_right.pos[2];
        else
            return bottom_left.pos[1];
    }

    Vector3D get_bottom_left() {
        return bottom_left.pos[2];
    }

    void set_top_right(Vector3D pos) {
        top_right.pos[1] = pos;
    }

    void set_bottom_right(Vector3D pos) {
        top_right.pos[0] = pos;
        bottom_left.pos[0] = pos;
    }

    void set_top_left(Vector3D pos) {
        top_right.pos[2] = pos;
        bottom_left.pos[1] = pos;
    }

    void set_bottom_left(Vector3D pos) {
        bottom_left.pos[2] = pos;
    }
};

#endif