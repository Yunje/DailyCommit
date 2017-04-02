// lap_define.h
// Linear Algebra Package
// Written by Yunje Cho. 4/1/2017

#ifndef LAP_DEFINE_H__
#define LAP_DEFINE_H__
#include <string.h>
#include <iostream>

template <typename T>
class Vect{
private:
	T* vect_;
	int size_;
	bool is_colvec_;
public:
	Vect();
	Vect(int size);
	~Vect();
	int Size() const;
	void MemAlloc(int size);
	void MemZeros();
	void MemZeros(int size);
	void MemFree();
	T& at(int ind);
	void Print() const;
	void Transpose();
};

template <typename T>
inline Vect<T>::Vect()
{
	vect_ = nullptr;
	size_ = 0;
	is_colvec_ = true;
}

template <typename T>
Vect<T>::Vect(int size)
{
		MemAlloc(size);
}

template <typename T>
Vect<T>::~Vect()
{
	MemFree();
}

template <typename T>
int Vect<T>::Size() const
{
	return size_;
}

template <typename T>
void Vect<T>::MemAlloc(int size)
{
	if (nullptr == vect_ && 0 < size){
		vect_ = new T[size];
		size_ = size;
	}
}

template <typename T>
void Vect<T>::MemZeros()
{
	if (nullptr != vect_){
		memset(vect_, 0, size_*sizeof(T));
	}
}

template <typename T>
void Vect<T>::MemZeros(int size)
{
	if (nullptr != vect_ && 0 < size){
		memset(vect_, 0, size_*sizeof(T));
	}
	else if (nullptr == vect && 0 < size){
		vect_ = new T[size];
		memset(vect_, 0, size_*sizeof(T));
	}
}

template <typename T>
void Vect<T>::MemFree()
{
	if (nullptr != vect_){
		delete[] vect_;
	}
}

template <typename T>
T& Vect<T>::at(int ind)
{
	if (0 <= ind && ind < size_ && nullptr != vect_){
		return vect_[ind];
	}
	else{
		return NULL;
	}
}

template <typename T>
void Vect<T>::Print() const
{
	int linewidth = 20;
	std::cout << "Vector Size: " << size_ << std::endl;
	
	if (is_colvec_){
		std::cout << "Vector Style: Column Vector" << std::endl;
	}
	else{
		std::cout << "Vector Style: Row Vector" << std::endl;
	}

	std::cout << "[Vector Contents]" << std::endl;
	for (int i = 0; i < size_; i++){
		if (0 == (i % linewidth) && 0 != i){
			std::cout << std::endl << vect_[i] << " ";
		}
		else{
			std::cout << vect_[i] << " ";
		}
	}
	std::cout << std::endl;
}

template <typename T>
void Vect<T>::Transpose()
{
	is_colvec_ = (!is_colvec_);
}


typedef Vect<float> Vectf;
typedef Vect<double> Vectd;
typedef Vect<int> Vectn;
typedef Vect<unsigned short> Vectus, Vectu;
typedef Vect<unsigned int> Vectun;
typedef Vect<unsigned char> Vectuc;

typedef enum _NormType 
{
	L0 = 0, L1 = 1, L2 = 2
} NormType;
#endif