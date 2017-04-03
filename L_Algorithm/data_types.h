#pragma once
#include <opencv.hpp>
#include <iostream>
using namespace cv;

typedef enum _img_type{
	UNSIGNED_CHAR = 1, UNSIGNED_SHORT, INT, FLOAT, DOUBLE
} img_type;

template <typename T>
class Img{
private:
	T* im_;
public:
	Img();
	Img(int _h, int _w);
	Img(const std::string& filepath);
	~Img();
	int w, h;
	void Zeros(int _h, int _w);
	void MemFree();
	void CopyFromMat(const Mat& _in);
	void CopyToMat(Mat* out, img_type type);
	void ConvertTypeFrom(const Img<unsigned char>& _in);
	void ConvertTypeFrom(const Img<unsigned short>& _in);
	void ConvertTypeFrom(const Img<int>& _in);
	void ConvertTypeFrom(const Img<float>& _in);
	void ConvertTypeFrom(const Img<double>& _in);
	T& in(int i, int j);
	T out(int i, int j) const;
	void CopyTo(Img<T>* _out);
	void Print();
	void LoadImage(const std::string& filepath);
	void SaveImage(const std::string& filepath, img_type type);
};

template <typename T>
Img<T>::Img()
{
	w = 0;
	h = 0;
	im_ = nullptr;
}

template <typename T>
Img<T>::Img(int _h, int _w)
{
	Zeros(_h, _w);
}

template <typename T>
Img<T>::Img(const std::string& filepath)
{
	LoadImage(filepath);
}

template <typename T>
Img<T>::~Img()
{
	if (im_ != nullptr)
	{
		MemFree();
	}
}

template <typename T>
void Img<T>::Zeros(int _h, int _w){
	h = _h;
	w = _w;
	if (nullptr == im_){
		im_ = new T[w * h];
	}
	for (int i = 0; i < w * h; i++) {
		im_[i] = 0.0;
	}

}

template <typename T>
void Img<T>::MemFree()
{
	if (im_){
		delete[] im_;
	}
	im_ = nullptr;
}

template <typename T>
void Img<T>::CopyFromMat(const Mat& _in)
{
	h = _in.rows;
	w = _in.cols;
	if (nullptr ==im_) Zeros(h, w);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = _in.at<T>(i, j);
		}
	}
}

template <typename T>
void Img<T>::CopyToMat(Mat* _out, img_type type)
{
	if (_out->rows == h && _out->cols == w && (nullptr != im_)){
		for (int i = 0; i < h; i++){
			for (int j = 0; j < w; j++){
				switch (type){
				case UNSIGNED_CHAR:
					_out->at<unsigned char>(i, j) = static_cast<unsigned char>(im_[i*w + j]);
					break;
				case UNSIGNED_SHORT:
					_out->at<unsigned short>(i, j) = static_cast<unsigned short>(im_[i*w + j]);
					break;
				case INT:
					_out->at<int>(i, j) = static_cast<int>(im_[i*w + j]);
					break;
				case FLOAT:
					_out->at<float>(i, j) = static_cast<float>(im_[i*w + j]);
					break;
				case DOUBLE:
					_out->at<double>(i, j) = static_cast<double>(im_[i*w + j]);
					break;
				}
				
			}
		}
	}
}

template <typename T>
T& Img<T>::in(int _i, int _j)
{
	return im_[_i*w + _j];
}

template <typename T>
T Img<T>::out(int _i, int _j) const
{
	return im_[_i*w + _j];
}

template <typename T>
void Img<T>::CopyTo(Img<T>* _out)
{
	if (_out == nullptr){
		_out->Zeros(w, h);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			_out->in(i, j) = this->out(i, j);
		}
	}

}

template <typename T>
void Img<T>::Print()
{
	int startx = 540;
	int starty = 280;
	int max_print_h = starty + ((h < 60) ? h : 60);
	int max_print_w = startx + ((w < 30) ? w : 30);
	std::cout << "Img Size: [ " << h << ", " << w << " ]" << std::endl;
	for (int i = starty; i < max_print_h; i++){
		for (int j = startx; j < max_print_w; j++){
			std::cout << out(i, j) << " ";
		}
		std::cout << std::endl;
	}
}

template <typename T>
void Img<T>::ConvertTypeFrom(const Img<unsigned char>& _in)
{
	if (nullptr == im_){
		h = _in.h;
		w = _in.w;
		Zeros(h, w);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = static_cast<T>(_in.out(i, j));
		}
	}
}

template <typename T>
void Img<T>::ConvertTypeFrom(const Img<unsigned short>& _in)
{
	if (nullptr == im_){
		h = _in.h;
		w = _in.w;
		Zeros(h, w);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = static_cast<T>(_in.out(i, j));
		}
	}
}

template <typename T>
void Img<T>::ConvertTypeFrom(const Img<int>& _in)
{
	if (nullptr == im_){
		h = _in.h;
		w = _in.w;
		Zeros(h, w);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = static_cast<T>(_in.out(i, j));
		}
	}
}

template <typename T>
void Img<T>::ConvertTypeFrom(const Img<float>& _in)
{
	if (nullptr == im_){
		h = _in.h;
		w = _in.w;
		Zeros(h, w);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = static_cast<T>(_in.out(i, j));
		}
	}
}

template <typename T>
void Img<T>::ConvertTypeFrom(const Img<double>& _in)
{
	if (nullptr == im_){
		h = _in.h;
		w = _in.w;
		Zeros(h, w);
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			im_[i*w + j] = static_cast<T>(_in.out(i, j));
		}
	}
}

template <typename T>
void Img<T>::LoadImage(const std::string& filepath)
{
	Mat img = imread(filepath);
	//imwrite("C:\\Users\\yunje\\Documents\\GitHub\\L_Algorithm\\L_Algorithm\\butterfly.bilinear.bmp",img);
	CopyFromMat(img);
}

template <typename T>
void Img<T>::SaveImage(const std::string& filepath, img_type type)
{
	Mat out;
	switch (type){
	case UNSIGNED_CHAR:
		out = Mat::zeros(h, w, CV_8U);
		break;
	case UNSIGNED_SHORT:
		out = Mat::zeros(h, w, CV_16U);
		break;
	case INT:
		out = Mat::zeros(h, w, CV_32S);
		break;
	case FLOAT:
		out = Mat::zeros(h, w, CV_32F);
		break;
	case DOUBLE:
		out = Mat::zeros(h, w, CV_64F);
		break;
	}
	CopyToMat(&out, type);
	imwrite(filepath, out);
}

typedef Img<unsigned char> Imguc;
typedef Img<unsigned short> Imgu, Imgus;
typedef Img<int> Imgn;
typedef Img<double> Imgd;
typedef Img<float> Imgf;

typedef struct label_info_{
	int size;
	int value;
	int i, j;
} label_info;

template <typename T1, typename T2>
bool Binarize(Img<T1>& _in, Img<T2>* _bin_map, float thres)
{
	int h = _in.h;
	int w = _in.w;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			(*_bin_map).im(i, j) = (_in.im(i, j) > thres) ? 1 : 0;
		}
	}
	return true;
}

template <typename T1, typename T2>
bool Segmentation(Img<T1>& _in, Img<T2>* _out)
{

	Imgu bin_img(_in.h, _in.w);
	bin_img.Print();
	Binarize(_in, &bin_img, 10);
	bin_img.Print();
	//imwrite("binary_img.bmp", binary_map);
	bin_img.CopyTo(_out);
	return true;
}

bool Labelling(const Mat& in, Mat* out)
{
	Mat temp;
	in.copyTo(temp);
	int binx, biny, binw, binh;
	binx = biny = 8;

	binw = in.cols / binx;
	binh = in.rows / biny;
	Mat label_img = Mat::zeros(binh, binw, CV_8U);
	for (int i = 0; i < binh; i++){
		for (int j = 0; j < binw; j++){

		}
	}

	(*out) = temp;
	return true;
}

bool Selection(const Mat& in, Mat* out)
{
	Mat temp;
	in.copyTo(temp);

	(*out) = temp;
	return true;
}

void LogAdd(string str, bool bRet)
{
	if (bRet){
		std::cout << str << " Process done!" << std::endl;
	}
	else{
		std::cout << " [Error] " << str << " in Process" << std::endl;
	}
}