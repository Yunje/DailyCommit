#include "data_types.h"
#include <iostream>
#include <vector>
#include "lap_define.h"



int main()
{
	bool bRet = true;
	string str;
	// step1: abs(target - LPC simulation)
	Mat img = imread("src_img.bmp", CV_8U);
	img.convertTo(img, CV_32FC1);

	Vect<float> vec1;
	vec1.MemAlloc(100);
	vec1.MemZeros();
	vec1.Print();
	vec1.Transpose();
	vec1.Print();
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