#include "Mat4x4.h"
#include <cmath>

Mat4x4::Mat4x4(Vec x_t, Vec y_t, Vec z_t, Vec w_t) {
	this->x_t = x_t;
	this->y_t = y_t;
	this->z_t = z_t;
	this->w_t = w_t;
}
Mat4x4 Mat4x4::CreateIdentity() {
	return Mat4x4(
		Vec(1.f, 0.f, 0.f, 0.f),
		Vec(0.f, 1.f, 0.f, 0.f),
		Vec(0.f, 0.f, 1.f, 0.f),
		Vec(0.f, 0.f, 0.f, 1.f)
	);
}
Vec operator*(Mat4x4 mat, Vec vec) {
	return Vec(mat.x_t*vec, mat.y_t*vec, mat.z_t*vec, mat.w_t*vec);
}
Mat4x4 operator*(Mat4x4 mat1, Mat4x4 mat2) {
	Vec tran_x(mat2.x_t.x, mat2.x_t.y, mat2.x_t.z, mat2.x_t.w);
	Vec tran_y(mat2.y_t.x, mat2.y_t.y, mat2.y_t.z, mat2.y_t.w);
	Vec tran_z(mat2.z_t.x, mat2.z_t.y, mat2.z_t.z, mat2.z_t.w);
	Vec tran_w(mat2.w_t.x, mat2.w_t.y, mat2.w_t.z, mat2.w_t.w);
	return Mat4x4(
		Vec(mat1.x_t*tran_x, mat1.x_t*tran_y, mat1.x_t*tran_z, mat1.x_t*tran_w),
		Vec(mat1.y_t*tran_x, mat1.y_t*tran_y, mat1.y_t*tran_z, mat1.y_t*tran_w),
		Vec(mat1.z_t*tran_x, mat1.z_t*tran_y, mat1.z_t*tran_z, mat1.z_t*tran_w),
		Vec(mat1.w_t*tran_x, mat1.w_t*tran_y, mat1.w_t*tran_z, mat1.w_t*tran_w)
	);
}
GLfloat& Mat4x4::operator() (int x, int y) {
	return vecs[y][x];
}

Mat4x4 Mat4x4::Perspective(GLfloat fovRadians, GLfloat aspectRatio, GLfloat nearZ, GLfloat farZ) {
	GLfloat top = tan(fovRadians);
	auto xTransform = Vec(nearZ / (top * aspectRatio), 0.f, 0.f, 0.f);
	auto yTransform = Vec(0.f, nearZ / top, 0.f, 0.f);
	GLfloat minusZDiff = nearZ - farZ;
	auto zTransform = Vec(0.f, 0.f,  (farZ + nearZ) / minusZDiff, -1.f);
	auto wTransform = Vec(0.f,0.f,(2.f*nearZ*farZ) / minusZDiff, 0.f);
	return Mat4x4(
		xTransform,
		yTransform,
		zTransform,
		wTransform
	);
}