#include <opencv.hpp>
#include <iostream>
using namespace cv;

template <typename T>
class Img{
private:
	T* im_;
public:
	Img();
	Img(int _h, int _w);
	~Img();
	int w, h;
	void Zeros(int _h, int _w);
	void MemFree();
	void CopyFromMat(const Mat& _in);
	void CopyToMat(Mat* out);
	T& im(int i, int j);
	void CopyTo(Img<T>* _out);
	void Print();
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
	if (!im_){
		im_ = new T[w * h];
		for (int i = 0; i < w * h; i++) {
			im_[i] = 0.0;
		}
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
	if (_in.rows == h && _in.cols == w && (!im_)){
		for (int i = 0; i < h; i++){
			for (int j = 0; j < w; j++){
				im_[i*w + j] = _in.at<T>(i, j);
			}
		}
	}
}

template <typename T>
void Img<T>::CopyToMat(Mat* _out)
{
	if (_out->rows == h && _out->cols == w && (!im_)){
		for (int i = 0; i < h; i++){
			for (int j = 0; j < w; j++){
				_out->at<float>(i, j) = im_[i*w + j];
			}
		}
	}
}

template <typename T>
T& Img<T>::im(int _i, int _j)
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
			_out->im(i, j) = this->im(i, j);
		}
	}

}

template <typename T>
void Img<T>::Print()
{
	int max_print_h = (h < 60) ? h : 60;
	int max_print_w = (w < 30) ? w : 30;
	std::cout << "Img Size: [ " << h << ", " << w << " ]" << std::endl;
	for (int i = 0; i < max_print_h; i++){
		for (int j = 0; j < max_print_w; j++){
			std::cout << im(i, j) << " ";
		}
		std::cout << std::endl;
	}
}

typedef Img<double> Imgd;
typedef Img<float> Imgf;
typedef Img<unsigned short> Imgu;
typedef Img<int> Imgn;

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