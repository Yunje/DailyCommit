#include "data_types.h"
#include <iostream>
#include <vector>
#include "lap_define.h"
#include "SIMD.h"
//#include <ctime>
#include "Interpolation.h"
#include "segmentation.h"

#define ARR_SIZE 3000000

float arr1[ARR_SIZE];
float arr2[ARR_SIZE];
float pResultSSE[ARR_SIZE];
float pResult[ARR_SIZE];


int main()
{
	InterpPrac();
	/*int arr2d[4*4] = {
		1,0,1,1,
		0,1,0,1,
		1,0,1,1,
		0,1,0,1
	};
	int out[4*4];
	FloodFill ff;
	ff.Init(arr2d, 4, 4);
	ff.Run();
	ff.Get(out);

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			std::cout << out[i * 4 + j] << " ";
		}
		std::cout << std::endl;
	}*/


	/*int nSize = (int)ARR_SIZE;
	for (int i = 0; i < ARR_SIZE; i++){
		arr1[i] = 1.f;
		arr2[i] = 2.f;
		pResult[i] = 0.f;
		pResultSSE[i] = 0.f;
	}

	std::clock_t start, end;

	start = std::clock();
	ComputeArraySSE(arr1, arr2, pResultSSE, nSize);
	end = std::clock();

	std::cout << "pResultSSE time: "<< end - start << std::endl;
	for (int i = 0; i < 100; i++){
		std::cout << pResultSSE[i] << " ";
	}
	std::cout << std::endl;
	
	

	start = std::clock();
	ComputeArray(arr1, arr2, pResult, nSize);
	end = std::clock();

	std::cout << "pResult time: " << end - start << std::endl;
	for (int i = 0; i < 100; i++){
		std::cout << pResult[i] << " ";
	}
	std::cout << std::endl;*/
	//bool bRet = true;
	//string str;
	//// step1: abs(target - LPC simulation)
	//Mat img = imread("src_img.bmp", CV_8U);
	//img.convertTo(img, CV_32FC1);

	//Vect<float> vec1;
	//vec1.MemAlloc(100);
	//vec1.MemZeros();
	//vec1.Print();
	//vec1.Transpose();
	//vec1.Print();
	//Mat input_img = Mat::zeros(img.rows, img.cols, CV_32FC1);
		
	//Imgf in(img.rows, img.cols);
	//in.CopyFromMat(img);
	//Imgu out(in.h, in.w);
	//// step2: segmentation
	//if (bRet){
	//	bRet = Segmentation(in, &out);
	//}
	//LogAdd("Segmentation", bRet);
	//out.Print();
	//	
	//// step3: Labelling
	//if (bRet){
	//	bRet = Labelling(img, &img);
	//}
	//LogAdd("Labelling", bRet);

	//// step4: Select Labels using DP
	//if (bRet){
	//	bRet = Selection(img, &img);
	//}
	//LogAdd("Selection", bRet);



	return 0;
}