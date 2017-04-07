#pragma once
#include <cmath>
#include "data_types.h"
#include "mem_control.h"
#include "lap_v2m.h"
#include "lap_v2v.h"

#define MAX(a,b) (((a) > (b))?(a):(b))
#define MIN(a,b) (((a) < (b))?(a):(b))

template <typename T>
void BicubicInterpolation(const Img<T>& in, Img<T>* out)
{
	double ratiox = (double)(in.w - 1) / (double)(out->w - 1);
	double ratioy = (double)(in.h - 1) / (double)(out->h - 1);

	// split planes (in): f, fx, fy, fxy
	Img<T> f, fx, fy, fxy;
	f.Zeros(in.h, in.w);
	fx.Zeros(in.h, in.w);
	fy.Zeros(in.h, in.w);
	fxy.Zeros(in.h, in.w);
	in.CopyTo(f);
	for (int i = 1; i < in.h - 1; i++){
		for (int j = 1; j < in.w - 1; j++){
			fx.in(i, j) = (f.out(i, j + 1) - f.out(i, j - 1)) / 2.0;
			fy.in(i, j) = (f.out(i + 1, j) - f.out(i - 1, j) / 2.0;
			fxy.in(i, j) = (f.out(i + 1, j + 1) - f.out(i + 1, j - 1) 
				- f.out(i - 1, j + 1) + f.out(i - 1, j - 1)) / 4.0;
		}
	}

	// making coefs for in image
	double invA[16*16] = {
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

	double **img = nullptr, **coefs = nullptr;
	MemAlloc<double>(img, in.h*in.w, 16);
	MemAlloc<double>(coefs, in.h*in.w, 16);
	MemZero<double>(img, in.h*in.w, 16);
	MemZero<double>(coefs, in.h*in.w, 16);
	for (int i = 1; i < in.h - 1; i++){
		for (int j = 1; j < in.w - 1; j++){
			img[i * in.w + j][0] = f.out(i, j);
			img[i * in.w + j][1] = f.out(i, j + 1);
			img[i * in.w + j][2] = f.out(i + 1, j);
			img[i * in.w + j][3] = f.out(i + 1, j + 1);
			img[i * in.w + j][4] = fx.out(i, j);
			img[i * in.w + j][5] = fx.out(i, j + 1);
			img[i * in.w + j][6] = fx.out(i + 1, j);
			img[i * in.w + j][7] = fx.out(i + 1, j + 1);
			img[i * in.w + j][8] = fy.out(i, j);
			img[i * in.w + j][9] = fy.out(i, j + 1);
			img[i * in.w + j][10] = fy.out(i + 1, j);
			img[i * in.w + j][11] = fy.out(i + 1, j + 1);
			img[i * in.w + j][12] = fxy.out(i, j);
			img[i * in.w + j][13] = fxy.out(i, j + 1);
			img[i * in.w + j][14] = fxy.out(i + 1, j);
			img[i * in.w + j][15] = fxy.out(i + 1, j + 1);

			MVMult1D<double>(invA, img[i * in.w + j], coefs[i * in.w + j], 16, 16);
		}
	}

	// fill out image

	for (int i = 0; i < out->h; i++){
		for (int j = 0; j < out->w; j++){
			double posx = j * ratiox;
			double posy = i * ratioy;
			int dx = (int)posx - posx;
			int dy = (int)posy - posy;
			double outval = 0.0;
			//TODO:
			VVMult<double>(???, coefs, &outval, 16);
			out->in(i, j) = outval;

		}
	}
		
	MemFree<double>(img, in.h*in.w);
	MemFree<double>(coefs, in.h*in.w);
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
			
			f[0] = in.out((int)(pos_y), (int)(pos_x));
			f[1] = in.out((int)(pos_y), (int) MIN((pos_x)+1, in.w - 1));
			f[2] = in.out((int) MIN((pos_y)+1, in.h - 1), (int)(pos_x));
			f[3] = in.out((int) MIN((pos_y)+1, in.h - 1), (int) MIN((pos_x)+1, in.w - 1));

			w[0] = (1.0 - w_y) * (1.0 - w_x);
			w[1] = (1.0 - w_y) * w_x;
			w[2] = w_y * (1.0 - w_x);
			w[3] = w_y * w_x;

			double sum = 0.0;
			for (int i = 0; i < 4; i++) sum += f[i] * w[i];
			out->in(y, x) = static_cast<T>(sum);
		}
	}
}

void InterpPrac(void)
{
	Imguc temp("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.gray.jpg");
	temp.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);
	
	Imgf src_img;
	src_img.ConvertTypeFrom(temp);
		
	src_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);
	Imgf out_img(src_img.h * 4, src_img.w * 4);
	BilinearInterpolation(src_img, &out_img);
	out_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp", UNSIGNED_CHAR);

	BicubicInterpolation(src_img, &out_img);
	out_img.SaveImage("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bicubic.bmp", UNSIGNED_CHAR);
	return;

}