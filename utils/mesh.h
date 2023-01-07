#include "triangle.h"
#include "vector.h"

#include <string>
#include <vector>
#include <fstream>
#include <strstream>

using namespace std;

#ifndef MESH_DEF
#define MESH_DEF

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

#endif