// lap_v2v.h
// Linear Algebra Package
// Written by Yunje Cho. 4/1/2017
// This header defines linear algebra calculation between vector and vector.
#ifndef LAP_V2V_H__
#define LAP_V2V_H__
#include "lap_define.h"

template <typename T>
void Swap(Vect<T> in1, Vect<T> in2);

template <typename T>
void Copy(const Vect<T>& in, Vect<T>* out);

template <typename T>
void Axpy(const Vect<T>& a, const Vect<T>& x, Vect<T>* y);

template <typename T>
double Norm(const Vect<T>& in, NormType type = L2);



#endif