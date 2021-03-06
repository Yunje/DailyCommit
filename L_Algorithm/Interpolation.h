#pragma once
#include <cmath>
#include "data_types.h"
#include "mem_control.h"
#include "lap_v2m.h"
#include "lap_v2v.h"
#include <ctime>

#define MAX(a,b) (((a) > (b))?(a):(b))
#define MIN(a,b) (((a) < (b))?(a):(b))

MVMult1D_REF mvmult;
VVMult_REF vvmult;

void SetMult(const MVMult1D_REF& _mvmult, const VVMult_REF& _vvmult)
{
	mvmult = _mvmult;
	vvmult = _vvmult;
}

template <typename T>
void BicubicInterpolation(const Img<T>& in, Img<T>* out)
{
	float ratiox = (float)(in.w - 1) / (float)(out->w - 1);
	float ratioy = (float)(in.h - 1) / (float)(out->h - 1);

	// split planes (in): f, fx, fy, fxy
	Img<T> f, fx, fy, fxy;
	f.Zeros(in.h, in.w);
	fx.Zeros(in.h, in.w);
	fy.Zeros(in.h, in.w);
	fxy.Zeros(in.h, in.w);
	in.CopyTo(&f);

#pragma omp parallel for
	for (int i = 1; i < in.h - 1; i++){
		for (int j = 1; j < in.w - 1; j++){
			fx.set(i, j) = (f.get(i, j + 1) - f.get(i, j - 1)) / 2.0;
			fy.set(i, j) = (f.get(i + 1, j) - f.get(i - 1, j)) / 2.0;
			fxy.set(i, j) = (f.get(i + 1, j + 1) - f.get(i + 1, j - 1) 
				- f.get(i - 1, j + 1) + f.get(i - 1, j - 1)) / 4.0;
		}
	}

	// making coefs for in image
	float invA[16*16] = {
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		-3, 3, 0, 0, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		2, -2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, -3, 3, 0, 0, -2, -1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 2, -2, 0, 0, 1, 1, 0, 0,
		-3, 0, 3, 0, 0, 0, 0, 0, -2, 0, -1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, -3, 0, 3, 0, 0, 0, 0, 0, -2, 0, -1, 0,
		9, -9, -9, 9, 6, 3, -6, -3, 6, -6, 3, -3, 4, 2, 2, 1,
		-6, 6, 6, -6, -3, -3, 3, 3, -4, 4, -2, 2, -2, -2, -1, -1,
		2, 0, -2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 2, 0, -2, 0, 0, 0, 0, 0, 1, 0, 1, 0,
		-6, 6, 6, -6, -4, -2, 4, 2, -3, 3, -3, 3, -2, -1, -2, -1,
		4, -4, -4, 4, 2, 2, -2, -2, 2, -2, 2, -2, 1, 1, 1, 1
	};

	float **img = nullptr, **coefs = nullptr;
	MemAlloc<float>(&img, in.h*in.w, 16);
	MemAlloc<float>(&coefs, in.h*in.w, 16);
	MemZero<float>(img, in.h*in.w, 16);
	MemZero<float>(coefs, in.h*in.w, 16);

#pragma omp parallel for
	for (int i = 1; i < in.h - 1; i++){
		for (int j = 1; j < in.w - 1; j++){
			img[i * in.w + j][0] = f.get(i, j);
			img[i * in.w + j][1] = f.get(i, j + 1);
			img[i * in.w + j][2] = f.get(i + 1, j);
			img[i * in.w + j][3] = f.get(i + 1, j + 1);
			img[i * in.w + j][4] = fx.get(i, j);
			img[i * in.w + j][5] = fx.get(i, j + 1);
			img[i * in.w + j][6] = fx.get(i + 1, j);
			img[i * in.w + j][7] = fx.get(i + 1, j + 1);
			img[i * in.w + j][8] = fy.get(i, j);
			img[i * in.w + j][9] = fy.get(i, j + 1);
			img[i * in.w + j][10] = fy.get(i + 1, j);
			img[i * in.w + j][11] = fy.get(i + 1, j + 1);
			img[i * in.w + j][12] = fxy.get(i, j);
			img[i * in.w + j][13] = fxy.get(i, j + 1);
			img[i * in.w + j][14] = fxy.get(i + 1, j);
			img[i * in.w + j][15] = fxy.get(i + 1, j + 1);

			//MVMult1D<float>(invA, img[i * in.w + j], coefs[i * in.w + j], 16, 16);
			mvmult(invA, img[i * in.w + j], coefs[i * in.w + j], 16, 16);
		}
	}

	// fill out image
#pragma omp parallel for
	for (int i = 0; i  < out->h; i++){
		for (int j = 0; j < out->w; j++){
			float xy[16] = { 0, };
			float posx = j * ratiox;
			float posy = i * ratioy;
			int nposx = (int)posx;
			int nposy = (int)posy;
			int dx = posx - nposx;
			int dy = posy - nposy;
			float outval = 0.0;
			
			//TODO:
			xy[0] = 1;
			xy[1] = dy;
			xy[2] = dy * dy;
			xy[3] = dy * dy * dy;
			for (int i = 1; i < 4; i++){
				xy[4 * i + 0] = dx * xy[4 * (i - 1) + 0];
				xy[4 * i + 1] = dx * xy[4 * (i - 1) + 1];
				xy[4 * i + 2] = dx * xy[4 * (i - 1) + 2];
				xy[4 * i + 3] = dx * xy[4 * (i - 1) + 3];
			}

			//VVMult<float>(xy, coefs[nposy * in.w + nposx], &outval, 16);
			vvmult(xy, coefs[nposy * in.w + nposx], &outval, 16);
			out->set(i, j) = outval;

		}
	}
		
	MemFree<float>(&img, in.h*in.w);
	MemFree<float>(&coefs, in.h*in.w);
}

template <typename T>
void BilinearInterpolation(const Img<T>& in, Img<T>* out)
{	
	double ratiox = (double) (in.w - 1) / (double)(out->w - 1);
	double ratioy = (double) (in.h - 1) / (double)(out->h - 1);

	double f[4] = { 0.0, 0.0, 0.0, 0.0 };
	double w[4] = { 0.0, 0.0, 0.0, 0.0 };

	for (int y = 0; y < out->h; y++){
		for (int x = 0; x < out->w; x++){
			double pos_y = (y * ratioy);
			double pos_x = (x * ratiox);
			double w_y = pos_y - (int)pos_y;
			double w_x = pos_x - (int)pos_x;
			
 			f[0] = in.get((int)(pos_y), (int)(pos_x));
			f[1] = in.get((int)(pos_y), (int) MIN((pos_x)+1, in.w - 1));
			f[2] = in.get((int) MIN((pos_y)+1, in.h - 1), (int)(pos_x));
			f[3] = in.get((int) MIN((pos_y)+1, in.h - 1), (int) MIN((pos_x)+1, in.w - 1));

			w[0] = (1.0 - w_y) * (1.0 - w_x);
			w[1] = (1.0 - w_y) * w_x;
			w[2] = w_y * (1.0 - w_x);
			w[3] = w_y * w_x;

			double sum = 0.0;
			for (int i = 0; i < 4; i++) sum += f[i] * w[i];
			out->set(y, x) = static_cast<T>(sum);
		}
	}
}

void InterpPrac(void)
{
	std::clock_t start, end;
	
	SetMult(MVMult1D_SSE, VVMult_SSE);
	//SetMult(MVMult1D<float>, VVMult<float>);

	Imguc temp("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.gray.jpg");
	temp.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);
	
	Imgf src_img;
	src_img.ConvertTypeFrom(temp);
		
	src_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);
	Imgf out_img(src_img.h * 4, src_img.w * 4);
	start = std::clock();
	BilinearInterpolation(src_img, &out_img);
	end = std::clock();
	std::cout << "Bilinear Interpolation: " << end - start << std::endl;

	out_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);

	start = std::clock();
	BicubicInterpolation(src_img, &out_img);
	end = std::clock();
	std::cout << "Bicubic Interpolation: " << end - start << std::endl;

	out_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bicubic.bmp", UNSIGNED_CHAR);
	return;

}