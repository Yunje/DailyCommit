#include <xmmintrin.h>
//#define ARRAY_SIZE 30000
//__declspec(align(16)) float m_fArray[ARRAY_SIZE];
//float* m_fArray = (float*)_aligned_malloc(ARRAY_SIZE*sizeof(float), 16);
#include <math.h>
//#include <stdio.h>

void ComputeArraySSE(float* pArray1, float* pArray2, float* pResult, int nSize)
{
	int nLoop = nSize / 4;
	__m128 m1, m2, m3, m4;
	__m128* pSrc1 = (__m128*) pArray1;
	__m128* pSrc2 = (__m128*) pArray2;
	__m128* pDest = (__m128*) pResult;
	__m128 m0_5 = _mm_set1_ps(0.5f);

	for (int i = 0; i < nLoop; i++){
		m1 = _mm_mul_ps(*pSrc1, *pSrc1);
		m2 = _mm_mul_ps(*pSrc2, *pSrc2);
		m3 = _mm_add_ps(m1, m2);
		m4 = _mm_sqrt_ps(m3);
		*pDest = _mm_add_ps(m4, m0_5);

		pSrc1++;
		pSrc2++;
		pDest++;
	}
}

void ComputeArray(float* pArray1, float* pArray2, float* pResult, int nSize)
{
	for (int i = 0; i < nSize; i++){
		pResult[i] = sqrt(pArray1[i]*pArray1[i] + pArray2[i]*pArray2[i]) + 0.5f;
	}
}



//struct SMCPUInfo
//{
//	bool sse_;  // Streaming SIMD Extension
//	bool sse2_; // Streaming SIMD Extension2
//	bool mmx_;  // MMX
//
//	char vendor_[13]; // 생산자 이름
//};
//
//__asm {
//	CHAR*	vendorName = SMCPUInfo.vendor_;
//
//
//}