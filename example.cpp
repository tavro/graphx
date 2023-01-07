#define APPLICATION_DEF
#include "engine.h"

#include <fstream>
#include <strstream>
#include <algorithm>

#include "utils/vector.h"
#include "utils/triangle.h"
#include "utils/matrix.h"
#include "utils/mesh.h"

using namespace std;

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
		//cube_mesh.load_from_obj_file("teapot.obj", false);
		depth_buffer = new float[screen_width() * screen_height()];
		
		cube_mesh.triangles = {
			{ Vector3D(0.0f, 0.0f, 0.0f),    Vector3D(0.0f, 1.0f, 0.0f),    Vector3D(1.0f, 1.0f, 0.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,}, 
			{ Vector3D(0.0f, 0.0f, 0.0f),    Vector3D(1.0f, 1.0f, 0.0f),    Vector3D(1.0f, 0.0f, 0.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ Vector3D(1.0f, 0.0f, 0.0f),    Vector3D(1.0f, 1.0f, 0.0f),    Vector3D(1.0f, 1.0f, 1.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ Vector3D(1.0f, 0.0f, 0.0f),    Vector3D(1.0f, 1.0f, 1.0f),    Vector3D(1.0f, 0.0f, 1.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ Vector3D(1.0f, 0.0f, 1.0f),    Vector3D(1.0f, 1.0f, 1.0f),    Vector3D(0.0f, 1.0f, 1.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ Vector3D(1.0f, 0.0f, 1.0f),    Vector3D(0.0f, 1.0f, 1.0f),    Vector3D(0.0f, 0.0f, 1.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ Vector3D(0.0f, 0.0f, 1.0f),    Vector3D(0.0f, 1.0f, 1.0f),    Vector3D(0.0f, 1.0f, 0.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ Vector3D(0.0f, 0.0f, 1.0f),    Vector3D(0.0f, 1.0f, 0.0f),    Vector3D(0.0f, 0.0f, 0.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ Vector3D(0.0f, 1.0f, 0.0f),    Vector3D(0.0f, 1.0f, 1.0f),    Vector3D(1.0f, 1.0f, 1.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ Vector3D(0.0f, 1.0f, 0.0f),    Vector3D(1.0f, 1.0f, 1.0f),    Vector3D(1.0f, 1.0f, 0.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
																										
			{ Vector3D(1.0f, 0.0f, 1.0f),    Vector3D(0.0f, 0.0f, 1.0f),    Vector3D(0.0f, 0.0f, 0.0f),		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 1.0f,},
			{ Vector3D(1.0f, 0.0f, 1.0f),    Vector3D(0.0f, 0.0f, 0.0f),    Vector3D(1.0f, 0.0f, 0.0f),		0.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,},
		};
		//spr_tex1 = new engine::Sprite("stone.png");

		mat_proj = Matrix4x4::make_projection(90.0f, (float)screen_height() / (float)screen_width(), 0.1f, 1000.0f);

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

		float speed = 8.0f * elapsed_time;
		Vector3D forward(look_dir * speed);

		if (get_key(engine::Key::W).held)
			camera += forward;

		if (get_key(engine::Key::S).held)
			camera -= forward;

		if (get_key(engine::Key::A).held)
			yaw -= 2.0f * elapsed_time;

		if (get_key(engine::Key::D).held)
			yaw += 2.0f * elapsed_time;

		Matrix4x4 mat_rot_z, mat_rot_x;

		mat_rot_z = Matrix4x4::make_rotation_z(theta * 0.5f);
		mat_rot_x = Matrix4x4::make_rotation_x(theta);

		Matrix4x4 mat_trans;
		mat_trans = Matrix4x4::make_translation(0.0f, 0.0f, 5.0f);

		Matrix4x4 mat_world;
		mat_world = Matrix4x4::make_identity();
		mat_world = mat_rot_z * mat_rot_x;
		mat_world = mat_world * mat_trans;

		Vector3D up(0.0f, 1.0f, 0.0f);
		Vector3D target(0.0f, 0.0f, 1.0f);
		Matrix4x4 mat_cam_rot = Matrix4x4::make_rotation_y(yaw);
		look_dir = mat_cam_rot.multiply_vector(target);
		target = camera + look_dir;
		Matrix4x4 mat_cam = Matrix4x4::point_at(camera, target, up);
		Matrix4x4 mat_view = mat_cam.quick_inverse();

		vector<Triangle> triangles_to_raster;

		for (auto tri : cube_mesh.triangles) {
			Triangle tri_proj, tri_transformed, tri_viewed;

			tri_transformed.pos[0] = mat_world.multiply_vector(tri.pos[0]);
			tri_transformed.pos[1] = mat_world.multiply_vector(tri.pos[1]);
			tri_transformed.pos[2] = mat_world.multiply_vector(tri.pos[2]);
			tri_transformed.tex[0] = tri.tex[0];
			tri_transformed.tex[1] = tri.tex[1];
			tri_transformed.tex[2] = tri.tex[2];

			Vector3D normal, line1, line2;
			line1 = tri_transformed.pos[1] - tri_transformed.pos[0];
			line2 = tri_transformed.pos[2] - tri_transformed.pos[0];

			normal = line1.crossprod(line2);
			normal = normal.normalize();
			
			Vector3D cam_ray = tri_transformed.pos[0] - camera;
			if (normal.dotprod(cam_ray) < 0.0f) {
				Vector3D light_dir(0.0f, 1.0f, -1.0f);
				light_dir = light_dir.normalize();

				float dp = max(0.1f, light_dir.dotprod(normal));
				tri_transformed.col = get_color(dp);

				tri_viewed.pos[0] = mat_view.multiply_vector(tri_transformed.pos[0]);
				tri_viewed.pos[1] = mat_view.multiply_vector(tri_transformed.pos[1]);
				tri_viewed.pos[2] = mat_view.multiply_vector(tri_transformed.pos[2]);
				tri_viewed.col = tri_transformed.col;

				tri_viewed.tex[0] = tri_transformed.tex[0];
				tri_viewed.tex[1] = tri_transformed.tex[1];
				tri_viewed.tex[2] = tri_transformed.tex[2];

				int clipped_triangles = 0;
				Triangle clipped[2];
				clipped_triangles = clip_against_plane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, tri_viewed, clipped[0], clipped[1]);
				for (int n = 0; n < clipped_triangles; n++) {
					tri_proj.pos[0] = mat_proj.multiply_vector(clipped[n].pos[0]);
					tri_proj.pos[1] = mat_proj.multiply_vector(clipped[n].pos[1]);
					tri_proj.pos[2] = mat_proj.multiply_vector(clipped[n].pos[2]);
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

					tri_proj.pos[0] = tri_proj.pos[0] / tri_proj.pos[0].w;
					tri_proj.pos[1] = tri_proj.pos[1] / tri_proj.pos[1].w;
					tri_proj.pos[2] = tri_proj.pos[2] / tri_proj.pos[2].w;

					tri_proj.pos[0].x *= -1.0f;
					tri_proj.pos[1].x *= -1.0f;
					tri_proj.pos[2].x *= -1.0f;
					tri_proj.pos[0].y *= -1.0f;
					tri_proj.pos[1].y *= -1.0f;
					tri_proj.pos[2].y *= -1.0f;

					Vector3D offset_view(1.0f, 1.0f, 0.0f);
					tri_proj.pos[0] = tri_proj.pos[0] + offset_view;
					tri_proj.pos[1] = tri_proj.pos[1] + offset_view;
					tri_proj.pos[2] = tri_proj.pos[2] + offset_view;

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
							tris_to_add = clip_against_plane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 1:	
							tris_to_add = clip_against_plane({ 0.0f, (float)screen_height() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 2:	
							tris_to_add = clip_against_plane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
						case 3:	
							tris_to_add = clip_against_plane({ (float)screen_width() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); 
							break;
					}

					for (int w = 0; w < tris_to_add; w++)
						list_triangles.push_back(clipped[w]);
				}
				new_triangles = list_triangles.size();
			}

			for (auto &t : list_triangles) {
				fill_triangle(t.pos[0].x, t.pos[0].y, t.pos[1].x, t.pos[1].y, t.pos[2].x, t.pos[2].y, t.col);
				/*
				textured_triangle(t.pos[0].x, t.pos[0].y, t.tex[0].u, t.tex[0].v, t.tex[0].w,
					t.pos[1].x, t.pos[1].y, t.tex[1].u, t.tex[1].v, t.tex[1].w,
					t.pos[2].x, t.pos[2].y, t.tex[2].u, t.tex[2].v, t.tex[2].w, spr_tex1);
				*/
				draw_triangle(t.pos[0].x, t.pos[0].y, t.pos[1].x, t.pos[1].y, t.pos[2].x, t.pos[2].y, engine::BLACK);
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