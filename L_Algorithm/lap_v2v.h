// lap_v2v.h
// Linear Algebra Package
// Written by Yunje Cho. 4/1/2017
// This header defines linear algebra calculation between vector and vector.
#ifndef LAP_V2V_H__
#define LAP_V2V_H__
#include "lap_define.h"

template <typename T>
void Swap(Vect<T> in1, Vect<T> in2);

template <typename T>
void Copy(const Vect<T>& in, Vect<T>* out);

template <typename T>
void Axpy(const Vect<T>& a, const Vect<T>& x, Vect<T>* y);

template <typename T>
double Norm(const Vect<T>& in, NormType type = L2);

template <typename T>
void VVMult(const T* in1, const T* in2, T* out, int size)
{
	T sum = 0.0;
	for (int i = 0; i < size; i++){
		sum += in1[i] * in2[i];
	}
	(*out) = sum;
}

void VVMult_SSE(const float* in1, const float* in2, float* out, int size)
{
	float z = 0.f;
	float fres = 0.f;
	__declspec(align(16)) float ftmp[4] = { 0.f, 0.f, 0.f, 0.f };
	FloatIntrinsic mres;

	if ((size / 4) != 0){
		mres = _mm_load_ss(&z);
		for (unsigned int i = 0; i < size / 4; i++){
			mres = _mm_add_ps(mres, _mm_mul_ps(_mm_loadu_ps(&in1[4 * i]), _mm_loadu_ps(&in2[4 * i])));
		}
		FloatIntrinsic mv1 = _mm_movelh_ps(mres, mres);
		FloatIntrinsic mv2 = _mm_movehl_ps(mres, mres);
		mres = _mm_add_ps(mv1, mv2);

		_mm_store_ps(ftmp, mres);
		fres = ftmp[0] + ftmp[1];
	}

	if ((size % 4) != 0){
		for (unsigned int i = size - size % 4; i < size; i++){
			fres += in1[i] * in2[i];
		}
	}
	(*out) = fres;
}

#endif