#include <vector>
#include <algorithm>

using namespace std;

class DynamicTable
{
public:
	void Init(int map_width, int map_height, int t_size);
	void Push(int label, int size, double value);
	void Run();
	
	void Sort();
	void MakeTable();
	void Algorithm();
	void BackTracking();

	void MemAlloc();
	void MemZero();
	void MemFree();

	DynamicTable();
	~DynamicTable();
private:
	struct LV{
		int label;
		int size;
		double value;
	};

	bool Compare(LV first, LV second);
	vector<LV> v;
	double **dt;
	int *backtrack;
	int table_width_;
	int table_height_;
};

void DynamicTable::Init(int map_width, int map_height, int t_size)
{
	v.reserve(map_width*map_height);
	table_width_ = t_size + 1;
}

void DynamicTable::Push(int label, int size, double value)
{
	LV lv { label, size, value };
	v.push_back(lv);
}

void DynamicTable::Run()
{
	Sort();
	MakeTable();
	BackTracking();
}

void DynamicTable::Sort()
{
	sort(v.begin(), v.end(), Compare);
}

void DynamicTable::MakeTable()
{
	MemAlloc();
	MemZero();
	Algorithm();
}

void DynamicTable::BackTracking()
{

}

void DynamicTable::Algorithm()
{
	for (int i = 1; i < table_height_; i++){
		for (int j = 1; j < table_width_; j++){
			if (v[i].size > j){
				dt[i][j] = dt[i - 1][j];
			}
			else{
				const double newval = dt[i - 1][j - v[i].size] + v[i].value;
				const double oldval = dt[i - 1][j];
				if (newval > oldval){
					dt[i][j] = newval;
					backtrack[j] = j - v[i].size;
				}
				else{
					dt[i][j] = oldval;
				}
			}
		}
	}
}

void DynamicTable::MemAlloc()
{
	table_height_ = v.size() + 1;
	dt = new double*[table_height_];
	for (int i = 0; i < table_height_; i++)
	{
		dt[i] = new double[table_width_];
	}
	backtrack = new int[table_width_];
}

void DynamicTable::MemZero()
{
	for (int i = 0; i < table_height_; i++)
	{
		for (int j = 0; j < table_width_; j++)
		{
			dt[i][j] = 0.0;
		}
	}
	for (int j = 0; j < table_width_; j++){
		backtrack[j] = 0;
	}
}

void DynamicTable::MemFree()
{
	if (nullptr != dt){
		for (int i = 0; i < table_height_; i++)
		{
			if (nullptr != dt[i]){
				delete[] dt[i];
			}
		}
		delete[] dt;
	}
	if (nullptr != backtrack)
	{
		delete[] backtrack;
	}
}

DynamicTable::DynamicTable(){};
DynamicTable::~DynamicTable()
{
	MemFree();
};

bool DynamicTable::Compare(LV first, LV second)
{
	return first.label < second.label;
}