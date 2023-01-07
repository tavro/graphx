#include "vector.h"

#include <math.h>

#ifndef MATRIX_DEF
#define MATRIX_DEF

class Matrix4x4 {
public:
    float m[4][4] = { 0 };
    
    Matrix4x4() {

    }

    Vector3D multiply_vector(Vector3D &i) {
		Vector3D v;

		v.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + i.w * m[3][0];
		v.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + i.w * m[3][1];
		v.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + i.w * m[3][2];
		v.w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + i.w * m[3][3];
		
		return v;
	}

	static Matrix4x4 make_identity() {
		Matrix4x4 matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	static Matrix4x4 make_rotation_x(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(angle_rad);
		matrix.m[1][2] = sinf(angle_rad);
		matrix.m[2][1] = -sinf(angle_rad);
		matrix.m[2][2] = cosf(angle_rad);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	static Matrix4x4 make_rotation_y(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = cosf(angle_rad);
		matrix.m[0][2] = sinf(angle_rad);
		matrix.m[2][0] = -sinf(angle_rad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(angle_rad);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	static Matrix4x4 make_rotation_z(float angle_rad) {
		Matrix4x4 matrix;

		matrix.m[0][0] = cosf(angle_rad);
		matrix.m[0][1] = sinf(angle_rad);
		matrix.m[1][0] = -sinf(angle_rad);
		matrix.m[1][1] = cosf(angle_rad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	static Matrix4x4 make_translation(float x, float y, float z) {
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

	static Matrix4x4 make_projection(float fov_degrees, float aspect_ratio, float near, float far) {
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

    Matrix4x4 operator * (const Matrix4x4 &other) {
		Matrix4x4 matrix;

        for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m[r][0] * other.m[0][c] + m[r][1] * other.m[1][c] + m[r][2] * other.m[2][c] + m[r][3] * other.m[3][c];

        return matrix;
    }

	static Matrix4x4 point_at(Vector3D &pos, Vector3D &target, Vector3D &up) {
		Vector3D new_forward = target - pos;
		new_forward = new_forward.normalize();

		Vector3D a = new_forward * up.dotprod(new_forward);
		Vector3D new_up = up - a;
		new_up = new_up.normalize();

		Vector3D new_right = new_up.crossprod(new_forward);

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

	Matrix4x4 quick_inverse() {
		Matrix4x4 matrix;

		matrix.m[0][0] = m[0][0]; 
		matrix.m[0][1] = m[1][0]; 
		matrix.m[0][2] = m[2][0]; 
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = m[0][1]; 
		matrix.m[1][1] = m[1][1]; 
		matrix.m[1][2] = m[2][1]; 
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = m[0][2]; 
		matrix.m[2][1] = m[1][2]; 
		matrix.m[2][2] = m[2][2]; 
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

};

#endif