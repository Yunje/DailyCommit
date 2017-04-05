//#ifndef _SEGMENTATION_H_
//#define _SEGMENTATION_H_
#include <queue>

struct pts{
	int i, j;
};

// 1 is segment, 0 is background
class FloodFill{
public:
	enum {OUT = 0, IN, SAME};
	void Init(int* binary_img, int width, int height);
	void Run();
	void Get(int* labeled_img);

	FloodFill();
	~FloodFill();
private:
	void MemAlloc();
	void MemZero();
	void MemCopy(int* from, int* in, int to_in);
	void MemFree();

	void efficient_floodfill(int i, int j, int label);

	int width_;
	int height_;
	int* binary_img_;
	int* labeled_img_;
};

void FloodFill::Init(int* binary_img, int width, int height)
{
	width_ = width + 2; // for boundary negate
	height_ = height + 2;
	MemAlloc();
	MemZero();
	MemCopy(binary_img, binary_img_, IN);
	MemCopy(binary_img_, labeled_img_, SAME);
}

void FloodFill::Run()
{
	int label = 1;
	for (int i = 0; i < height_; i++){
		for (int j = 0; j < width_; j++){
			if ((-1) == labeled_img_[i * width_ + j]){
				efficient_floodfill(i, j, label);
				label++;
			}
		}
	}
}


void FloodFill::Get(int* labeled_img)
{
	MemCopy(labeled_img_, labeled_img, OUT); // is_out;
}

FloodFill::FloodFill()
{
	width_ = 0;
	height_ = 0;
	binary_img_ = nullptr;
	labeled_img_ = nullptr;
}

FloodFill::~FloodFill()
{
	MemFree();
}

void FloodFill::MemAlloc()
{
	binary_img_ = new int[width_ * height_];
	labeled_img_ = new int[width_ * height_];
}

void FloodFill::MemZero()
{
	for (int i = 0; i < height_; i++){
		for (int j = 0; j < width_; j++){
			binary_img_[i*width_ + j] = 0;
			labeled_img_[i*width_ + j] = 0;
		}
	}
}

void FloodFill::MemCopy(int* from, int* to, int to_in)
{
	int out_w = width_ - 2;
	int out_h = height_ - 2;

	if (IN == to_in){
		for (int i = 0; i < out_h; i++){
			for (int j = 0; j < out_w; j++){
				if (from[i * out_w + j]){
					to[(i + 1) * width_ + (j + 1)] = -1;
				}
			}
		}
	}
	else if (OUT == to_in){ // in2out
		for (int i = 1; i < height_ - 1; i++){
			for (int j = 1; j < width_ - 1; j++){
				to[(i - 1) * out_w + (j - 1)] = from[i * width_ + j];
			}
		}
	}
	else{
		for (int i = 0; i < height_; i++){
			for (int j = 0; j < width_; j++){
				to[i * width_ + j] = from[i * width_ + j];
			}
		}
	}
}

void FloodFill::MemFree()
{
	if (binary_img_) delete[] binary_img_;
	if (labeled_img_) delete[] labeled_img_;
	binary_img_ = nullptr;
	labeled_img_ = nullptr;
}

void FloodFill::efficient_floodfill(int i, int j, int label)
{
	int left, right;
	std::queue<pts> Queue;
	Queue.push({ i, j });
	while (!Queue.empty()){
		pts point = Queue.front();
		Queue.pop();
		if ((-1) == labeled_img_[point.i * width_ + point.j]){
			left = right = point.j;
			while ((-1) == labeled_img_[point.i * width_ + left - 1]){
				left--;
			}
			while ((-1) == labeled_img_[point.i * width_ + right + 1]){
				right++;
			}
			for (int c = left; c <= right; c++){
				labeled_img_[point.i * width_ + c] = label;
				if (((-1) == labeled_img_[(point.i - 1) * width_ + c]) &&
					(c == left || (-1) != labeled_img_[(point.i - 1) * width_ + c - 1])){
					Queue.push({ point.i - 1, c });
				}
				if (((-1) == labeled_img_[(point.i + 1) * width_ + c]) &&
					(c == left || (-1) != labeled_img_[(point.i + 1) * width_ + c - 1])){
					Queue.push({ point.i + 1, c });
				}
			}
		}
	}
}


//#endif