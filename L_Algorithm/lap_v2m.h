#ifndef _LAP_V2M_H_
#define _LAP_V2M_H_

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

#endif