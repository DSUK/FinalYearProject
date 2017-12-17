#include "Vec.h"

Vec::Vec() {
	Vec(0,0,0);
}
Vec::Vec (__m128 input) {
	data = input;
}
Vec::Vec(GLfloat ix, GLfloat iy, GLfloat iz) {
	x = ix;
	y = iy;
	z = iz;
}
void Vec::normalise() {
	w = 0;
	__m128 temp = _mm_mul_ps(data,data); //multiply data with self
	temp = _mm_hadd_ps(temp,temp);	   // returns (x + y, z + w, x + y, z + w)
	temp = _mm_hadd_ps(temp,temp);	   // returns (x + y + z + w, x + y + z + w, x + y + z + w, x + y + z + w)
	temp = _mm_rsqrt_ps(temp);		   //inverse square root
	data = _mm_mul_ps(data, temp);
}
Vec Vec::toUnit() {
	Vec temp(data);
	temp.normalise();
	return temp;
}
void Vec::operator = (Vec rhs) {
	data = rhs.data;
}
void Vec::operator += (Vec rhs) {
	data = _mm_add_ps(data,rhs.data);
}




Vec operator + (Vec lhs, Vec rhs) {
	return Vec(_mm_add_ps(lhs.data,rhs.data));
}
GLfloat operator * (Vec lhs, Vec rhs) { //dot product
	lhs.w = 0.0f;
	__m128 temp = _mm_mul_ps(lhs.data,rhs.data);
	temp = _mm_hadd_ps(temp,temp);	   // returns (x + y, z + w, x + y, z + w)
	temp = _mm_hadd_ps(temp,temp);	   // returns (x + y + z + w, x + y + z + w, x + y + z + w, x + y + z + w)
	GLfloat t;
	_mm_store_ss(&t,temp);
	return t;
}
Vec operator - (Vec rhs) {
	return Vec(-rhs.x,-rhs.y,-rhs.z);
}
Vec operator - (Vec lhs, Vec rhs) {
	return Vec(_mm_sub_ps(lhs.data,rhs.data));
}
Vec operator % (Vec lhs, Vec rhs) { //cross product
	__m128 temp1, temp2;

	temp1 = _mm_mul_ps(_mm_shuffle_ps(lhs.data,lhs.data,_MM_SHUFFLE(1,3,2,0)),
		_mm_shuffle_ps(rhs.data,rhs.data,_MM_SHUFFLE(2,1,3,0)));

	temp2 = _mm_mul_ps(_mm_shuffle_ps(lhs.data,lhs.data,_MM_SHUFFLE(2,1,3,0)),
		_mm_shuffle_ps(rhs.data,rhs.data,_MM_SHUFFLE(1,3,2,0)));

	return Vec(_mm_sub_ps(temp1,temp2));
}
Vec operator * (GLfloat lhs,Vec rhs) {
	__m128 temp = _mm_set_ps1(lhs);
	return Vec(_mm_mul_ps(temp, rhs.data));
}
Vec operator / (Vec lhs, GLfloat rhs) {
	__m128 temp = _mm_set_ps1(rhs);
	return Vec(_mm_div_ps(lhs.data,temp));
}

