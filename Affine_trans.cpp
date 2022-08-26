#include "Affine_trans.h"
#include <cmath>

double Pi = 3.14159265;

Affine_trans::Affine_trans() {}

Affine_trans::~Affine_trans() {}

double Affine_trans::Rad(double degree) { return degree * Pi / 180; }

void Affine_trans::identity_matrix(Matrix4& mat) {
	Matrix4 v;
	v.m[0][0] = 1;
	v.m[1][1] = 1;
	v.m[2][2] = 1;
	v.m[3][3] = 1;
	mat = v;
}

void Affine_trans::scale(
  Matrix4& mat, float expansion_rate_x, float expansion_rate_y, float expansion_rate_z) {
	identity_matrix(mat);
	mat.m[0][0] = expansion_rate_x;
	mat.m[1][1] = expansion_rate_y;
	mat.m[2][2] = expansion_rate_z;
	mat.m[3][3] = 1;
}

void Affine_trans::translation(Matrix4& mat, float Tx, float Ty, float Tz) {
	identity_matrix(mat);
	mat.m[3][0] = Tx;
	mat.m[3][1] = Ty;
	mat.m[3][2] = Tz;
}

void Affine_trans::rotateX(Matrix4& mat, float frequency_x) {
	// XÇÃâÒì]
	float sin = std::sin(frequency_x);
	float cos = std::cos(frequency_x);

	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos , sin , 0.0f,
		0.0f, -sin, cos , 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat = result;
}

void Affine_trans::rotateY(Matrix4& mat, float frequency_y) {
	// YÇÃâÒì]
	float sin = std::sin(frequency_y);
	float cos = std::cos(frequency_y);

	Matrix4 result
	{
		cos , 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    sin , 0.0f, cos , 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat = result;
}

void Affine_trans::rotateZ(Matrix4& mat, float frequency_z) {
	// ZÇÃâÒì]
	float sin = std::sin(frequency_z);
	float cos = std::cos(frequency_z);

	Matrix4 result
	{
		cos,  sin,  0.0f, 0.0f, 
		-sin, cos,  0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
	    0.0f, 0.0f, 0.0f, 1.0f
	};

	mat = result;
}



void Affine_trans::Inverse(Matrix4& mat) {
	float mat4[4][8] = {
	  {mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3], 1, 0, 0, 0},
	  {mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3], 0, 1, 0, 0},
	  {mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3], 0, 0, 1, 0},
	  {mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3], 0, 0, 0, 1}
    };

	//çsóÒÇÃåä∑

	//çsóÒÇë„ì¸
}

void Affine_trans::Vec3conversion_W_Included(Vector3& v, const Matrix4& m) {
	float W = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
	Vector3 result
	{
	  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / W,
	  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / W,
	  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / W
	};

	v = result;
}

void Affine_trans::Vec3conversion_W_Notincluded(Vector3& v, const Matrix4& m) {
	Vector3 result
	{
	  (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0]),
	  (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1]),
	  (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2])
	};
	v = result;
}



void Affine_trans::rotate(Matrix4& mat, float frequency_x, float frequency_y, float frequency_z) {
	Matrix4 RotX, RotY, RotZ, Rot;

	identity_matrix(RotX);
	identity_matrix(RotY);
	identity_matrix(RotZ);
	identity_matrix(Rot);

	rotateX(RotX, frequency_x);
	rotateY(RotY, frequency_y);
	rotateZ(RotZ, frequency_z);



	mat = RotZ * RotX * RotY;
}

void Affine_trans::scale(Matrix4& mat, Vector3& scale) {
	identity_matrix(mat);
	mat.m[0][0] = scale.x;
	mat.m[1][1] = scale.y;
	mat.m[2][2] = scale.z;
	mat.m[3][3] = 1;
}

void Affine_trans::translation(Matrix4& mat, Vector3& translation) {
	Matrix4 result
	{
		1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		translation.x, translation.y, translation.z, 1.0f
	};

	mat = result;
}

void Affine_trans::rotate(Matrix4& mat, Vector3& rotate) {
	Matrix4 RotX, RotY, RotZ, Rot;

	identity_matrix(RotX);
	identity_matrix(RotY);
	identity_matrix(RotZ);
	identity_matrix(Rot);

	rotateX(RotX, rotate.x);
	rotateY(RotY, rotate.y);
	rotateZ(RotZ, rotate.z);

	Rot = RotZ * RotX * RotY;

	mat = Rot;
}

void Affine_trans::Affine_Trans(Matrix4& mat, Vector3& scale, Vector3& rotate, Vector3& translation) 
{
	
	Matrix4 RotX, RotY, RotZ, Rot, Scale, Translation;
	identity_matrix(RotX);
	identity_matrix(RotY);
	identity_matrix(RotZ);
	identity_matrix(Rot);
	identity_matrix(Scale);
	identity_matrix(Translation);

	Scale.m[0][0] = scale.x;
	Scale.m[1][1] = scale.y;
	Scale.m[2][2] = scale.z;
	Scale.m[3][3] = 1;

	rotateX(RotX, rotate.x);
	rotateY(RotY, rotate.y);
	rotateZ(RotZ, rotate.z);

	Rot = RotZ * RotX * RotY;

	Translation.m[3][0] = translation.x;
	Translation.m[3][1] = translation.y;
	Translation.m[3][2] = translation.z;

	mat = Scale * Rot * Translation;

}

// void Affine_trans::Affine_Mul_Vel(Matrix4& mat, Vector3& scale, Vector3& rotate, Vector3&
// translation, Vector3& velocity)
//{
//	identity_matrix(mat);
//
//	mat.m[0][0] = scale.x;
//	mat.m[1][1] = scale.y;
//	mat.m[2][2] = scale.z;
//	mat.m[3][3] = 1;
//
//	Matrix4 RotX, RotY, RotZ;
//
//	rotateX(RotX, rotate.x);
//	rotateY(RotY, rotate.y);
//	rotateZ(RotZ, rotate.z);
//
//	mat = RotZ * RotX * RotY;
//
//	mat.m[3][0] = translation.x;
//	mat.m[3][1] = translation.y;
//	mat.m[3][2] = translation.z;
//
//	velocity * mat;
// }
