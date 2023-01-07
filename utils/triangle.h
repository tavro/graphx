#include "../engine.h"
#include "vector.h"

#ifndef TIRANGLE_DEF
#define TIRANGLE_DEF

struct Triangle {
    Vector3D pos[3];
	Vector2D tex[3];
	engine::Pixel col;
};

int clip_against_plane(Vector3D plane_p, Vector3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2) {
	plane_n = plane_n.normalize();

	auto dist = [&](Vector3D &p) {
		Vector3D n = p.normalize();
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dotprod(plane_p));
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
		out_tri1.pos[1] = Vector3D::intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.tex[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.tex[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.tex[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri1.pos[2] = Vector3D::intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
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
		out_tri1.pos[2] = Vector3D::intersect_plane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
		out_tri1.tex[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
		out_tri1.tex[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
		out_tri1.tex[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

		out_tri2.pos[0] = *inside_points[1];
		out_tri2.tex[0] = *inside_tex[1];
		out_tri2.pos[1] = out_tri1.pos[2];
		out_tri2.tex[1] = out_tri1.tex[2];
		out_tri2.pos[2] = Vector3D::intersect_plane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
		out_tri2.tex[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
		out_tri2.tex[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
		out_tri2.tex[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
		return 2;
	}

	return -1;
}

#endif