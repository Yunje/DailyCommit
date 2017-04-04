#pragma once
#include "data_types.h"
#define MAX(a,b) (((a) > (b))?(a):(b))
#define MIN(a,b) (((a) < (b))?(a):(b))

//template <typename T>
//void BicubicInterpolation(const Img<T>& in, Img<T>* out)
//{
//	double ratiox = (double)(in.w - 1) / (double)(out->w - 1);
//	double ratioy = (double)(in.h - 1) / (double)(out->h - 1);
//	double invA[16][16] = {
//		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		-3, 3, 0, 0, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		2, -2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, -3, 3, 0, 0, -2, -1, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 2, -2, 0, 0, 1, 1, 0, 0,
//		-3, 0, 3, 0, 0, 0, 0, 0, -2, 0, -1, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, -3, 0, 3, 0, 0, 0, 0, 0, -2, 0, -1, 0,
//		9, -9, -9, 9, 6, 3, -6, -3, 6, -6, 3, -3, 4, 2, 2, 1,
//		-6, 6, 6, -6, -3, -3, 3, 3, -4, 4, -2, 2, -2, -2, -1, -1,
//		2, 0, -2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 2, 0, -2, 0, 0, 0, 0, 0, 1, 0, 1, 0,
//		-6, 6, 6, -6, -4, -2, 4, 2, -3, 3, -3, 3, -2, -1, -2, -1,
//		4, -4, -4, 4, 2, 2, -2, -2, 2, -2, 2, -2, 1, 1, 1, 1
//	};
//	double f[16] = { 0.0 };
//	double w[16] = { 0.0 };
//	for (int y = 0; y < out->h; y++){
//		for (int x = 0; x < out->w; x++){
//			//w = invA * f;
//
//			pos_x;
//			double sum = 0.0;
//			for (int i = 0; i < 4; i++){
//				for (int j = 0; j < 4; j++){
//					sum += w[i*4+j] * 
//				}
//			}
//
//		}
//	}
//	
//
//}
//

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
	return;

}