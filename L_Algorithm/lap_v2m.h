#ifndef _LAP_V2M_H_
#define _LAP_V2M_H_

#include <xmmintrin.h>
#define M_CHECK_ALIGNMENT(lObject, luAlignment) \
{ \
    assert( (luAlignment) ); \
    assert( ( (luAlignment) & ( (luAlignment) - 1 ) ) == 0 ); \
    assert( (reinterpret_cast<intptr_t>( &(lObject) ) & ( (luAlignment) - 1 )) == 0 ); \
}
typedef __m128 FloatIntrinsic;

template <typename T>
void MVMult(const T** mat, const T* vec, T* vec_out, int h, int w)
{
	for (int i = 0; i < h; i++){
		double sum = 0.0;
		for (int j = 0; j < w; j++){
			sum += mat[i][j] * vec[j];
		}
		vec_out[i] = sum;
	}
}


template <typename T>
void MVMult1D(const T* mat, const T* vec, T* vec_out, int h, int w)
{
	for (int i = 0; i < h; i++){
		double sum = 0.0;
		for (int j = 0; j < w; j++){
			sum += mat[i*w+j] * vec[j];
		}
		vec_out[i] = sum;
	}
}

void MVMult1D_SSE(const float* mat, const float* vec, float* vec_out, int h, int w)
{
	float z = 0.f;
	float fres = 0.f;
	__declspec(align(16)) float ftmp[4] = { 0.f, 0.f, 0.f, 0.f };
	FloatIntrinsic mres;

	for (int i = 0; i < h; i++){
		if ((w / 4) != 0){
			mres = _mm_load_ss(&z);
			for (unsigned int j = 0; j < w / 4; j++){
				mres = _mm_add_ps(mres, _mm_mul_ps(_mm_loadu_ps(&mat[i * w + 4 * j]), 
					_mm_loadu_ps(&vec[4 * j])));
			}
			FloatIntrinsic mv1 = _mm_movelh_ps(mres, mres);
			FloatIntrinsic mv2 = _mm_movehl_ps(mres, mres);
			mres = _mm_add_ps(mv1, mv2);

			_mm_store_ps(ftmp, mres);
			fres = ftmp[0] + ftmp[1];
		}

		if ((w % 4) != 0){
			for (unsigned int j = w - w % 4; j < w; j++){
				fres += mat[i * w + j] * vec[j];
			}
		}
		vec_out[i] = fres;
	}
}


#endif