#define APPLICATION_DEF
#include "engine.h"
using namespace std;

struct Vector3D {
    float x, y, z;
};

struct Triangle {
    Vector3D pos[3];
};

struct Mesh {
    vector<Triangle> triangles;
};

struct Matrix4x4 {
    float m[4][4] = { 0 };
};

class Engine3D : public engine::Engine {
public:
    Engine3D() {
        app_name = "3D Demo";
    }

private:
    Mesh cube_mesh;
    Matrix4x4 mat_proj;

    float theta;

    void multiply_matrix_vector(Vector3D &i, Vector3D &o, Matrix4x4 &m) {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w; o.y /= w; o.z /= w;
		}
    }

public:
    bool on_create() override {
		cube_mesh.triangles = {
            // south
            { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

            // east                                                      
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

            // north                                                     
            { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

            // west                                                      
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

            // top                                                       
            { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

            // bottom                                                    
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		};

		float near = 0.1f;
		float far = 1000.0f;
		float fov = 90.0f;
		float aspect_ratio = (float)screen_height() / (float)screen_width();
		float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

		mat_proj.m[0][0] = aspect_ratio * fov_rad;
		mat_proj.m[1][1] = fov_rad;
		mat_proj.m[2][2] = far / (far - near);
		mat_proj.m[3][2] = (-far * near) / (far - near);
		mat_proj.m[2][3] = 1.0f;
		mat_proj.m[3][3] = 0.0f;

        return true;
    }

    bool on_update(float elapsed_time) override {
        fill_rect(0, 0, screen_width(), screen_height());

        Matrix4x4 mat_rot_z, mat_rot_x;
        theta += 1.0f * elapsed_time;

		mat_rot_z.m[0][0] = cosf(theta);
		mat_rot_z.m[0][1] = sinf(theta);
		mat_rot_z.m[1][0] = -sinf(theta);
		mat_rot_z.m[1][1] = cosf(theta);
		mat_rot_z.m[2][2] = 1;
		mat_rot_z.m[3][3] = 1;

		mat_rot_x.m[0][0] = 1;
		mat_rot_x.m[1][1] = cosf(theta * 0.5f);
		mat_rot_x.m[1][2] = sinf(theta * 0.5f);
		mat_rot_x.m[2][1] = -sinf(theta * 0.5f);
		mat_rot_x.m[2][2] = cosf(theta * 0.5f);
		mat_rot_x.m[3][3] = 1;

        for(auto tri : cube_mesh.triangles) {
            Triangle tri_projected, tri_translated, tri_rotated_z, tri_rotated_z_x;

			multiply_matrix_vector(tri.pos[0], tri_rotated_z.pos[0], mat_rot_z);
			multiply_matrix_vector(tri.pos[1], tri_rotated_z.pos[1], mat_rot_z);
			multiply_matrix_vector(tri.pos[2], tri_rotated_z.pos[2], mat_rot_z);

			multiply_matrix_vector(tri_rotated_z.pos[0], tri_rotated_z_x.pos[0], mat_rot_x);
			multiply_matrix_vector(tri_rotated_z.pos[1], tri_rotated_z_x.pos[1], mat_rot_x);
			multiply_matrix_vector(tri_rotated_z.pos[2], tri_rotated_z_x.pos[2], mat_rot_x);

			tri_translated = tri_rotated_z_x;
			tri_translated.pos[0].z = tri_rotated_z_x.pos[0].z + 3.0f;
			tri_translated.pos[1].z = tri_rotated_z_x.pos[1].z + 3.0f;
			tri_translated.pos[2].z = tri_rotated_z_x.pos[2].z + 3.0f;

			multiply_matrix_vector(tri_translated.pos[0], tri_projected.pos[0], mat_proj);
			multiply_matrix_vector(tri_translated.pos[1], tri_projected.pos[1], mat_proj);
			multiply_matrix_vector(tri_translated.pos[2], tri_projected.pos[2], mat_proj);

			tri_projected.pos[0].x += 1.0f; tri_projected.pos[0].y += 1.0f;
			tri_projected.pos[1].x += 1.0f; tri_projected.pos[1].y += 1.0f;
			tri_projected.pos[2].x += 1.0f; tri_projected.pos[2].y += 1.0f;
			tri_projected.pos[0].x *= 0.5f * (float)screen_width();
			tri_projected.pos[0].y *= 0.5f * (float)screen_height();
			tri_projected.pos[1].x *= 0.5f * (float)screen_width();
			tri_projected.pos[1].y *= 0.5f * (float)screen_height();
			tri_projected.pos[2].x *= 0.5f * (float)screen_width();
			tri_projected.pos[2].y *= 0.5f * (float)screen_height();

            draw_triangle(tri_projected.pos[0].x, tri_projected.pos[0].y, tri_projected.pos[1].x, tri_projected.pos[1].y, tri_projected.pos[2].x, tri_projected.pos[2].y, engine::BLACK);
        }

        return true;
    }
};

int main() {
    Engine3D demo;

    if(demo.construct(256, 240, 4, 4))
        demo.start();

    return 0;
}