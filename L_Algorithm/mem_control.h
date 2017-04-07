#ifndef _MEM_CONTROL_H_
#define _MEM_CONTROL_H_

template<typename T>
void MemAlloc(T* ptr, int sz1)
{
	ptr = new T[sz1];
}

template<typename T>
void MemZero(T* ptr, int sz1)
{
	for (int i = 0; i < sz1; i++){
		ptr[i] = static_cast<T>(0);
	}
}

template<typename T>
void MemFree(T* ptr)
{
	delete []ptr;
}

template<typename T>
void MemAlloc(T** ptr, int sz1, int sz2)
{
	ptr = new T*[sz1];
	for (int i = 0; i < sz1; i++){
		ptr[i] = new T[sz2];
	}
}

template<typename T>
void MemZero(T** ptr, int sz1, int sz2)
{
	for (int i = 0; i < sz1; i++){
		for (int j = 0; j < sz2; j++){
			ptr[i][j] = static_cast<T>(0);
		}
	}
}

template<typename T>
void MemFree(T** ptr, int sz1)
{
	for (int i = 0; i < sz1; i++){
		delete[]ptr[i];
	}
	delete[] ptr;
}

template<typename T>
void MemAlloc(T*** ptr, int sz1, int sz2, int sz3)
{
	ptr = new T**[sz1];
	for (int i = 0; i < sz1; i++){
		ptr[i] = new T*[sz2];
		for (int j = 0; j < sz2; j++){
			ptr[i][j] = new T[sz3];
		}
	}
}

template<typename T>
void MemAlloc(T*** ptr, int sz1, int sz2, int sz3)
{
	for (int i = 0; i < sz1; i++){
		for (int j = 0; j < sz2; j++){
			for (int k = 0; k < sz3; k++){
				ptr[i][j][k] = static_cast<T>(0);
			}
		}
	}
}

template<typename T>
void MemFree(T*** ptr, int sz1, int sz2)
{
	for (int i = 0; i < sz1; i++){
		for (int j = 0; j < sz2; j++){
			delete[] ptr[i][j];
		}
		delete[] ptr[i];
	}
	delete[] ptr;
}

#endif