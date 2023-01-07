#include <stdio.h>
#include <math.h>

#ifndef VECTOR_DEF
#define VECTOR_DEF

struct Vector2D {
	float u = 0;
	float v = 0;
	float w = 1;
};

class Vector3D {
public:
    float x;
    float y;
    float z;
    float w;

    Vector3D(float x_coord, float y_coord, float z_coord) {
        x = x_coord;
        y = y_coord;
        z = z_coord;
        w = 1.0f;
    }

    Vector3D(const Vector3D &vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = 1.0f;
    }

    Vector3D() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    }

    Vector3D operator+(Vector3D other) {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D& operator+=(Vector3D other){
        x = x + other.x;
        y = y + other.y;
        z = z + other.z;
        return *this;
    };

    Vector3D operator-(Vector3D other) {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D& operator-=(Vector3D other){
        x = x - other.x;
        y = y - other.y;
        z = z - other.z;
        return *this;
    };

    Vector3D operator=(Vector3D other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    Vector3D operator/(const float &k) {
        return Vector3D(x / k, y / k, z / k);
    }

    Vector3D operator*(const float &k) {
        return Vector3D(x * k, y * k, z * k);
    }

    float dotprod(Vector3D other) {
        return x * other.x + y * other.y + z * other.z;
    }

    float len() {
        return sqrtf(dotprod(*this));
    }

    Vector3D normalize() {
        return Vector3D(x / len(), y / len(), z / len());
    }

    Vector3D crossprod(Vector3D other) {
		float vx = y * other.z - z * other.y;
		float vy = z * other.x - x * other.z;
		float vz = x * other.y - y * other.x;
		return Vector3D(vx, vy, vz);
	}

    static Vector3D intersect_plane(Vector3D &plane_p, Vector3D &plane_n, Vector3D &line_start, Vector3D &line_end, float &t) {
		plane_n = plane_n.normalize();

		float plane_d = -plane_n.dotprod(plane_p);
		float ad = line_start.dotprod(plane_n);
		float bd = line_end.dotprod(plane_n);

		t = (-plane_d - ad) / (bd - ad);

		Vector3D line_start_to_end = line_end - line_start;
		Vector3D line_to_intersect = line_start_to_end * t;

		return line_start + line_to_intersect;
	}
};

#endif