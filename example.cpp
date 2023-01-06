#define APPLICATION_DEF
#include "engine.h"

#include <fstream>
#include <strstream>
#include <algorithm>

using namespace std;

struct Vector2D {
	float u = 0;
	float v = 0;
	float w = 1;
};

struct Vector3D {
    float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

struct Triangle {
    Vector3D pos[3];
	Vector2D tex[3];
	engine::Pixel col;
};

struct Mesh {
    vector<Triangle> triangles;

	bool load_from_obj_file(string file_name, bool has_texture = false) {
		ifstream file(file_name);

		if(!file.is_open())
			return false;

		vector<Vector3D> verts;
		vector<Vector2D> texs;
		while(!file.eof()) {
			char line[128];
			file.getline(line, 128);

			strstream s;
			s << line;

			char tmp;

			if(line[0] == 'v') {
				if (line[1] == 't') {
					Vector2D v;
					s >> tmp >> tmp >> v.u >> v.v;
					texs.push_back(v);
				}
				else {
					Vector3D vert;
					s >> tmp >> vert.x >> vert.y >> vert.z;
					verts.push_back(vert);
				}
			}

			if(!has_texture) {
				if(line[0] == 'f') {
					int f[3];
					s >> tmp >> f[0] >> f[1] >> f[2];
					triangles.push_back({
						verts[f[0] - 1],
						verts[f[1] - 1],
						verts[f[2] - 1]
					});
				}
				else {
					if (line[0] == 'f') {
						s >> tmp;
						string tokens[6];
						int token_count = -1;

						while (!s.eof()) {
							char c = s.get();
							if (c == ' ' || c == '/')
								token_count++;
							else
								tokens[token_count].append(1, c);
						}
						tokens[token_count].pop_back();

						triangles.push_back({ 
							verts[stoi(tokens[0]) - 1], 
							verts[stoi(tokens[2]) - 1], 
							verts[stoi(tokens[4]) - 1],
							texs[stoi(tokens[1]) - 1], 
							texs[stoi(tokens[3]) - 1], 
							texs[stoi(tokens[5]) - 1] 
						});
					}
				}
			}
		}

		return true;
	}
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

	Vector3D camera;
	Vector3D look_dir;

	float yaw;
    float theta;

	engine::Sprite *spr_tex1;

    Vector3D matrix_multiply_vector(Matrix4x4 &m, Vector3D &i) {
		Vector3D v;

		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
		
		return v;
	}

	Matrix4x4 matrix_make_identity() {
		Matrix4x4 matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Matrix4x4 matrix_make_rotation_x(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(angle_rad);
		matrix.m[1][2] = sinf(angle_rad);
		matrix.m[2][1] = -sinf(angle_rad);
		matrix.m[2][2] = cosf(angle_rad);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Matrix4x4 matrix_make_rotation_y(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = cosf(angle_rad);
		matrix.m[0][2] = sinf(angle_rad);
		matrix.m[2][0] = -sinf(angle_rad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(angle_rad);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Matrix4x4 matrix_make_rotation_z(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = cosf(angle_rad);
		matrix.m[0][1] = sinf(angle_rad);
		matrix.m[1][0] = -sinf(angle_rad);
		matrix.m[1][1] = cosf(angle_rad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Matrix4x4 matrix_make_translation(float x, float y, float z) {
		Matrix4x4 matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;

		return matrix;
	}

	Matrix4x4 matrix_make_projection(float fov_degrees, float aspect_ratio, float near, float far) {
		float fov_rad = 1.0f / tanf(fov_degrees * 0.5f / 180.0f * 3.14159f);
		Matrix4x4 matrix;

		matrix.m[0][0] = aspect_ratio * fov_rad;
		matrix.m[1][1] = fov_rad;
		matrix.m[2][2] = far / (far - near);
		matrix.m[3][2] = (-far * near) / (far - near);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;

		return matrix;
	}

	Matrix4x4 matrix_multiply_matrix(Matrix4x4 &m1, Matrix4x4 &m2) {
		Matrix4x4 matrix;

		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		
		return matrix;
	}

	Matrix4x4 matrix_point_at(Vector3D &pos, Vector3D &target, Vector3D &up) {
		Vector3D new_forward = vector_sub(target, pos);
		new_forward = vector_norm(new_forward);

		Vector3D a = vector_mult(new_forward, vector_dotprod(up, new_forward));
		Vector3D new_up = vector_sub(up, a);
		new_up = vector_norm(new_up);

		Vector3D new_right = vector_crossprod(new_up, new_forward);

		Matrix4x4 matrix;
		
		matrix.m[0][0] = new_right.x;	
		matrix.m[0][1] = new_right.y;	
		matrix.m[0][2] = new_right.z;	
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = new_up.x;		
		matrix.m[1][1] = new_up.y;		
		matrix.m[1][2] = new_up.z;		
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = new_forward.x;	
		matrix.m[2][1] = new_forward.y;	
		matrix.m[2][2] = new_forward.z;	
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = pos.x;			
		matrix.m[3][1] = pos.y;			
		matrix.m[3][2] = pos.z;			
		matrix.m[3][3] = 1.0f;
		
		return matrix;
	}

	Matrix4x4 matrix_quick_inverse(Matrix4x4 &m) {
		Matrix4x4 matrix;

		matrix.m[0][0] = m.m[0][0]; 
		matrix.m[0][1] = m.m[1][0]; 
		matrix.m[0][2] = m.m[2][0]; 
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = m.m[0][1]; 
		matrix.m[1][1] = m.m[1][1]; 
		matrix.m[1][2] = m.m[2][1]; 
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = m.m[0][2]; 
		matrix.m[2][1] = m.m[1][2]; 
		matrix.m[2][2] = m.m[2][2]; 
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	Vector3D vector_add(Vector3D &v1, Vector3D &v2) {
		return {
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z
		};
	}

	Vector3D vector_sub(Vector3D &v1, Vector3D &v2) {
		return {
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z
		};
	}

	Vector3D vector_div(Vector3D &v1, float k) {
		return {
			v1.x / k,
			v1.y / k,
			v1.z / k
		};
	}

	Vector3D vector_mult(Vector3D &v1, float k) {
		return {
			v1.x * k,
			v1.y * k,
			v1.z * k
		};
	}

	float vector_dotprod(Vector3D &v1, Vector3D &v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float vector_len(Vector3D &v) {
		return sqrtf(vector_dotprod(v, v));
	}

	Vector3D vector_norm(Vector3D &v) {
		float len = vector_len(v);
		return {
			v.x / len,
			v.y / len,
			v.z / len
		};
	}

	Vector3D vector_crossprod(Vector3D &v1, Vector3D &v2) {
		Vector3D v;
		
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;

		return v;
	}

	Vector3D vector_intersect_plane(Vector3D &plane_p, Vector3D &plane_n, Vector3D &line_start, Vector3D &line_end, float &t) {
		plane_n = vector_norm(plane_n);

		float plane_d = -vector_dotprod(plane_n, plane_p);
		float ad = vector_dotprod(line_start, plane_n);
		float bd = vector_dotprod(line_end, plane_n);

		t = (-plane_d - ad) / (bd - ad);
		Vector3D line_start_to_end = vector_sub(line_end, line_start);
		Vector3D line_to_intersect = vector_mult(line_start_to_end, t);
		return vector_add(line_start, line_to_intersect);
	}

	int triangle_clip_against_plane(Vector3D plane_p, Vector3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2) {
		plane_n = vector_norm(plane_n);

		auto dist = [&](Vector3D &p) {
			Vector3D n = vector_norm(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - vector_dotprod(plane_n, plane_p));
		};

		Vector3D* inside_points[3];  
		int inside_point_count = 0;

		Vector3D* outside_points[3]; 
		int outside_point_count = 0;

		Vector2D* inside_tex[3]; 
		int inside_tex_count = 0;

		Vector2D* outside_tex[3]; 
		int outside_tex_count = 0;

		float d0 = dist(in_tri.pos[0]);
		float d1 = dist(in_tri.pos[1]);
		float d2 = dist(in_tri.pos[2]);

		if (d0 >= 0) { 
			inside_points[inside_point_count++] = &in_tri.pos[0]; 
			inside_tex[inside_tex_count++] = &in_tri.tex[0]; 
		}
		else {
			outside_points[outside_point_count++] = &in_tri.pos[0]; 
			outside_tex[outside_tex_count++] = &in_tri.tex[0];
		}

		if (d1 >= 0) {
			inside_points[inside_point_count++] = &in_tri.pos[1]; 
			inside_tex[inside_tex_count++] = &in_tri.tex[1];
		}
		else {
			outside_points[outside_point_count++] = &in_tri.pos[1];  
			outside_tex[outside_tex_count++] = &in_tri.tex[1];
		}

		if (d2 >= 0) {
			inside_points[inside_point_count++] = &in_tri.pos[2]; 
			inside_tex[inside_tex_count++] = &in_tri.tex[2];
		}
		else {
			outside_points[outside_point_count++] = &in_tri.pos[2];  
			outside_tex[outside_tex_count++] = &in_tri.tex[2];
		}

		if (inside_point_count == 0) {
			return 0;
		}

		if (inside_point_count == 3) {
			out_tri1 = in_tri;
			return 1;
		}

		if (inside_point_count == 1 && outside_point_count == 2) {
			out_tri1.col =  in_tri.col;

			out_tri1.pos[0] = *inside_points[0];
			out_tri1.tex[0] = *inside_tex[0];

			float t;
			out_tri1.pos[1] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.tex[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.tex[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.tex[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri1.pos[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.tex[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.tex[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.tex[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

			return 1;
		}

		if (inside_point_count == 2 && outside_point_count == 1) {
			out_tri1.col =  in_tri.col;
			out_tri2.col =  in_tri.col;

			out_tri1.pos[0] = *inside_points[0];
			out_tri1.pos[1] = *inside_points[1];
			out_tri1.tex[0] = *inside_tex[0];
			out_tri1.tex[1] = *inside_tex[1];

			float t;
			out_tri1.pos[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.tex[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
			out_tri1.tex[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
			out_tri1.tex[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

			out_tri2.pos[0] = *inside_points[1];
			out_tri2.tex[0] = *inside_tex[1];
			out_tri2.pos[1] = out_tri1.pos[2];
			out_tri2.tex[1] = out_tri1.tex[2];
			out_tri2.pos[2] = vector_intersect_plane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
			out_tri2.tex[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
			out_tri2.tex[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
			out_tri2.tex[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
			return 2;
		}
	}

	engine::Pixel get_color(float lum) {
		engine::Pixel col;
		int pixel_bw = (int)(13.0f * lum);

		switch (pixel_bw) {
			case 0: 
				col = engine::BLACK;
				break;
			case 1:
			case 2:
			case 3: 
			case 4: 
				col = engine::DARK_GREY;
				break;
			case 5:
			case 6:
			case 7: 
			case 8:
				col = engine::GREY;
				break;
			case 9:  
			case 10: 
			case 11: 
			case 12: 
				col = engine::LIGHT_GREY;
				break;
			default:
				col = engine::BLACK;
		}

		return col;
	}

public:
	float *depth_buffer = nullptr;
	bool on_create() override {
		cube_mesh.load_from_obj_file("teapot.obj", false);
		depth_buffer = new float[screen_width() * screen_height()];
		
		/*
		cube_mesh.triangles = {
			{ 0.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,}, 
			{ 0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ 1.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ 0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ 1.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
		};*/

		mat_proj = matrix_make_projection(90.0f, (float)screen_height() / (float)screen_width(), 0.1f, 1000.0f);

		// spr_tex1 = new engine::Sprite("stone.png");
		return true;
	}

	bool on_update(float elapsed_time) override {
		if (get_key(engine::Key::UP).held)
			camera.y += 8.0f * elapsed_time;

		if (get_key(engine::Key::DOWN).held)
			camera.y -= 8.0f * elapsed_time;
		
		if (get_key(engine::Key::LEFT).held)
			camera.x -= 8.0f * elapsed_time;

		if (get_key(engine::Key::RIGHT).held)
			camera.x += 8.0f * elapsed_time;

		Vector3D forward = vector_mult(look_dir, 8.0f * elapsed_time);

		if (get_key(engine::Key::W).held)
			camera = vector_add(camera, forward);

		if (get_key(engine::Key::S).held)
			camera = vector_sub(camera, forward);

		if (get_key(engine::Key::A).held)
			yaw -= 2.0f * elapsed_time;

		if (get_key(engine::Key::D).held)
			yaw += 2.0f * elapsed_time;

		Matrix4x4 mat_rot_z, mat_rot_x;

		mat_rot_z = matrix_make_rotation_z(theta * 0.5f);
		mat_rot_x = matrix_make_rotation_x(theta);

		Matrix4x4 mat_trans;
		mat_trans = matrix_make_translation(0.0f, 0.0f, 5.0f);

		Matrix4x4 mat_world;
		mat_world = matrix_make_identity();
		mat_world = matrix_multiply_matrix(mat_rot_z, mat_rot_x);
		mat_world = matrix_multiply_matrix(mat_world, mat_trans);

		Vector3D up = { 0,1,0 };
		Vector3D target = { 0,0,1 };
		Matrix4x4 mat_cam_rot = matrix_make_rotation_y(yaw);
		look_dir = matrix_multiply_vector(mat_cam_rot, target);
		target = vector_add(camera, look_dir);
		Matrix4x4 mat_cam = matrix_point_at(camera, target, up);
		Matrix4x4 mat_view = matrix_quick_inverse(mat_cam);

		vector<Triangle> triangles_to_raster;

		for (auto tri : cube_mesh.triangles) {
			Triangle tri_proj, tri_transformed, tri_viewed;

			tri_transformed.pos[0] = matrix_multiply_vector(mat_world, tri.pos[0]);
			tri_transformed.pos[1] = matrix_multiply_vector(mat_world, tri.pos[1]);
			tri_transformed.pos[2] = matrix_multiply_vector(mat_world, tri.pos[2]);
			tri_transformed.tex[0] = tri.tex[0];
			tri_transformed.tex[1] = tri.tex[1];
			tri_transformed.tex[2] = tri.tex[2];

			Vector3D normal, line1, line2;

			line1 = vector_sub(tri_transformed.pos[1], tri_transformed.pos[0]);
			line2 = vector_sub(tri_transformed.pos[2], tri_transformed.pos[0]);

			normal = vector_crossprod(line1, line2);

			normal = vector_norm(normal);
			
			Vector3D cam_ray = vector_sub(tri_transformed.pos[0], camera);
			if (vector_dotprod(normal, cam_ray) < 0.0f) {
				Vector3D light_dir = { 0.0f, 1.0f, -1.0f };
				light_dir = vector_norm(light_dir);

				float dp = max(0.1f, vector_dotprod(light_dir, normal));
				tri_transformed.col = get_color(dp);

				tri_viewed.pos[0] = matrix_multiply_vector(mat_view, tri_transformed.pos[0]);
				tri_viewed.pos[1] = matrix_multiply_vector(mat_view, tri_transformed.pos[1]);
				tri_viewed.pos[2] = matrix_multiply_vector(mat_view, tri_transformed.pos[2]);
				tri_viewed.col = tri_transformed.col;
				tri_viewed.tex[0] = tri_transformed.tex[0];
				tri_viewed.tex[1] = tri_transformed.tex[1];
				tri_viewed.tex[2] = tri_transformed.tex[2];

				int clipped_triangles = 0;
				Triangle clipped[2];
				clipped_triangles = triangle_clip_against_plane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, tri_viewed, clipped[0], clipped[1]);
				for (int n = 0; n < clipped_triangles; n++) {
					tri_proj.pos[0] = matrix_multiply_vector(mat_proj, clipped[n].pos[0]);
					tri_proj.pos[1] = matrix_multiply_vector(mat_proj, clipped[n].pos[1]);
					tri_proj.pos[2] = matrix_multiply_vector(mat_proj, clipped[n].pos[2]);
					tri_proj.col = clipped[n].col;
					tri_proj.tex[0] = clipped[n].tex[0];
					tri_proj.tex[1] = clipped[n].tex[1];
					tri_proj.tex[2] = clipped[n].tex[2];

					tri_proj.tex[0].u = tri_proj.tex[0].u / tri_proj.pos[0].w;
					tri_proj.tex[1].u = tri_proj.tex[1].u / tri_proj.pos[1].w;
					tri_proj.tex[2].u = tri_proj.tex[2].u / tri_proj.pos[2].w;

					tri_proj.tex[0].v = tri_proj.tex[0].v / tri_proj.pos[0].w;
					tri_proj.tex[1].v = tri_proj.tex[1].v / tri_proj.pos[1].w;
					tri_proj.tex[2].v = tri_proj.tex[2].v / tri_proj.pos[2].w;

					tri_proj.tex[0].w = 1.0f / tri_proj.pos[0].w;
					tri_proj.tex[1].w = 1.0f / tri_proj.pos[1].w;
					tri_proj.tex[2].w = 1.0f / tri_proj.pos[2].w;

					tri_proj.pos[0] = vector_div(tri_proj.pos[0], tri_proj.pos[0].w);
					tri_proj.pos[1] = vector_div(tri_proj.pos[1], tri_proj.pos[1].w);
					tri_proj.pos[2] = vector_div(tri_proj.pos[2], tri_proj.pos[2].w);

					tri_proj.pos[0].x *= -1.0f;
					tri_proj.pos[1].x *= -1.0f;
					tri_proj.pos[2].x *= -1.0f;
					tri_proj.pos[0].y *= -1.0f;
					tri_proj.pos[1].y *= -1.0f;
					tri_proj.pos[2].y *= -1.0f;

					Vector3D offset_view = { 1, 1, 0 };
					tri_proj.pos[0] = vector_add(tri_proj.pos[0], offset_view);
					tri_proj.pos[1] = vector_add(tri_proj.pos[1], offset_view);
					tri_proj.pos[2] = vector_add(tri_proj.pos[2], offset_view);
					tri_proj.pos[0].x *= 0.5f * (float)screen_width();
					tri_proj.pos[0].y *= 0.5f * (float)screen_height();
					tri_proj.pos[1].x *= 0.5f * (float)screen_width();
					tri_proj.pos[1].y *= 0.5f * (float)screen_height();
					tri_proj.pos[2].x *= 0.5f * (float)screen_width();
					tri_proj.pos[2].y *= 0.5f * (float)screen_height();

					triangles_to_raster.push_back(tri_proj);
				}			
			}
		}

		sort(triangles_to_raster.begin(), triangles_to_raster.end(), [](Triangle &t1, Triangle &t2) {
			float z1 = (t1.pos[0].z + t1.pos[1].z + t1.pos[2].z) / 3.0f;
			float z2 = (t2.pos[0].z + t2.pos[1].z + t2.pos[2].z) / 3.0f;
			return z1 > z2;
		});

		fill_rect(0, 0, screen_width(), screen_height(), engine::BLACK);

		for (int i = 0; i < screen_width()*screen_height(); i++)
			depth_buffer[i] = 0.0f;

		for (auto &tri_to_raster : triangles_to_raster) {
			Triangle clipped[2];
			list<Triangle> list_triangles;

			list_triangles.push_back(tri_to_raster);
			int new_triangles = 1;

			for (int p = 0; p < 4; p++) {
				int tris_to_add = 0;
				while (new_triangles > 0) {
					Triangle test = list_triangles.front();
					list_triangles.pop_front();
					new_triangles--;

					switch (p) {
						case 0:	
							tris_to_add = triangle_clip_against_plane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 1:	
							tris_to_add = triangle_clip_against_plane({ 0.0f, (float)screen_height() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 2:	
							tris_to_add = triangle_clip_against_plane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 3:	
							tris_to_add = triangle_clip_against_plane({ (float)screen_width() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
					}

					for (int w = 0; w < tris_to_add; w++)
						list_triangles.push_back(clipped[w]);
				}
				new_triangles = list_triangles.size();
			}

			for (auto &t : list_triangles) {
				/*
				textured_triangle(t.pos[0].x, t.pos[0].y, t.tex[0].u, t.tex[0].v, t.tex[0].w,
					t.pos[1].x, t.pos[1].y, t.tex[1].u, t.tex[1].v, t.tex[1].w,
					t.pos[2].x, t.pos[2].y, t.tex[2].u, t.tex[2].v, t.tex[2].w, spr_tex1);
				*/
				fill_triangle(t.pos[0].x, t.pos[0].y, t.pos[1].x, t.pos[1].y, t.pos[2].x, t.pos[2].y, t.col);
				//draw_triangle(t.pos[0].x, t.pos[0].y, t.pos[1].x, t.pos[1].y, t.pos[2].x, t.pos[2].y, engine::WHITE);
			}
		}

		return true;
	}

	void textured_triangle( int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3, engine::Sprite *tex) {
		if (y2 < y1) {
			swap(y1, y2);
			swap(x1, x2);
			swap(u1, u2);
			swap(v1, v2);
			swap(w1, w2);
		}

		if (y3 < y1) {
			swap(y1, y3);
			swap(x1, x3);
			swap(u1, u3);
			swap(v1, v3);
			swap(w1, w3);
		}

		if (y3 < y2) {
			swap(y2, y3);
			swap(x2, x3);
			swap(u2, u3);
			swap(v2, v3);
			swap(w2, w3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;
		float dv1 = v2 - v1;
		float du1 = u2 - u1;
		float dw1 = w2 - w1;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;
		float dv2 = v3 - v1;
		float du2 = u3 - u1;
		float dw2 = w3 - w1;

		float tex_u, tex_v, tex_w;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step=0, dw2_step=0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1) {
			for (int i = y1; i <= y2; i++) {
				int ax = x1 + (float)(i - y1) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx) {
					swap(ax, bx);
					swap(tex_su, tex_eu);
					swap(tex_sv, tex_ev);
					swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++) {
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					if (tex_w > depth_buffer[i*screen_width() + j]) {
						draw(j, i, tex->sample(tex_u / tex_w, tex_v / tex_w));
						depth_buffer[i*screen_width() + j] = tex_w;
					}
					t += tstep;
				}

			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		dw1 = w3 - w2;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0, dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1) {
			for (int i = y2; i <= y3; i++) {
				int ax = x2 + (float)(i - y2) * dax_step;
				int bx = x1 + (float)(i - y1) * dbx_step;

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx) {
					swap(ax, bx);
					swap(tex_su, tex_eu);
					swap(tex_sv, tex_ev);
					swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++) {
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					if (tex_w > depth_buffer[i*screen_width() + j]) {
						draw(j, i, tex->sample(tex_u / tex_w, tex_v / tex_w));
						depth_buffer[i*screen_width() + j] = tex_w;
					}
					t += tstep;
				}
			}	
		}		
	}
};

int main() {
    Engine3D demo;

    if(demo.construct(256, 240, 4, 4))
        demo.start();

    return 0;
}