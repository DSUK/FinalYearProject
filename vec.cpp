#include "includes.h"
#include <xmmintrin.h> //libry for SSE extention
#include <pmmintrin.h> //library for more SSE

struct vec
{
    union
    {
        __m128 data;
        struct
        {
            GLfloat w,x,y,z; //4 floating points now have to be used
        } pos;
    };
	vec()
	{
        vec(0,0,0);
	}
	vec (__m128 input)
	{
	    data = input;
	}
    vec(GLfloat ix, GLfloat iy, GLfloat iz)
    {
        pos.x = ix;
        pos.y = iy;
        pos.z = iz;
    }
    void normalise()
    {
        pos.w = 0;
        __m128 temp = _mm_mul_ps(data,data); //multiply data with self
        temp = _mm_hadd_ps(temp,temp);       // returns (x + y, z + w, x + y, z + w)
        temp = _mm_hadd_ps(temp,temp);       // returns (x + y + z + w, x + y + z + w, x + y + z + w, x + y + z + w)
        temp = _mm_rsqrt_ps(temp);           //inverse square root
        data = _mm_mul_ps(data, temp);
    }
    vec toUnit()
	{
	    vec temp(data);
        temp.normalise();
        return temp;
	}
    void operator = (vec rhs)
    {
      data = rhs.data;
    }
    void operator += (vec rhs)
    {
        data = _mm_add_ps(data,rhs.data);
    }
};




vec operator + (vec lhs, vec rhs)
{
    return vec(_mm_add_ps(lhs.data,rhs.data));
}
GLfloat operator * (vec lhs, vec rhs) //dot product
{
    lhs.pos.w = 0.0f;
    __m128 temp = _mm_mul_ps(lhs.data,rhs.data);
    temp = _mm_hadd_ps(temp,temp);       // returns (x + y, z + w, x + y, z + w)
    temp = _mm_hadd_ps(temp,temp);       // returns (x + y + z + w, x + y + z + w, x + y + z + w, x + y + z + w)
    GLfloat t;
    _mm_store_ss(&t,temp);
    return t;
}
vec operator - (vec rhs)
{
    return vec(-rhs.pos.x,-rhs.pos.y,-rhs.pos.z);
}
vec operator - (vec lhs, vec rhs)
{
    return vec(_mm_sub_ps(lhs.data,rhs.data));
}
vec operator % (vec lhs, vec rhs)  //cross product 
{
    __m128 temp1, temp2;
    temp1 = _mm_mul_ps(_mm_shuffle_ps(lhs.data,lhs.data,_MM_SHUFFLE(1,3,2,0)),_mm_shuffle_ps(rhs.data,rhs.data,_MM_SHUFFLE(2,1,3,0)));
    temp2 = _mm_mul_ps(_mm_shuffle_ps(lhs.data,lhs.data,_MM_SHUFFLE(2,1,3,0)),_mm_shuffle_ps(rhs.data,rhs.data,_MM_SHUFFLE(1,3,2,0)));
    return vec(_mm_sub_ps(temp1,temp2));
}
vec operator * (GLfloat lhs,vec rhs)
{
    __m128 temp = _mm_set_ps1(lhs);
    return vec(_mm_mul_ps(temp, rhs.data));
}
vec operator / (vec lhs, GLfloat rhs)
{
   __m128 temp = _mm_set_ps1(rhs);
    return vec(_mm_div_ps(lhs.data,temp));
}

