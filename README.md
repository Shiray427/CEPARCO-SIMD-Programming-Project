# CEPARCO SIMD Programming Project
Write the kernel in (1) C program; (2) an x86-64 assembly language; (3) x86 SIMD AVX2 assembly language using XMM register; (4) x86 SIMD AVX2 assembly language using YMM register. The kernel is to perform 1-D stencil of vector X place the result in vector Y. 
Input: Memory location n (integer) contains the length of the vector; Vectors X and Y are both 32-bit integer. Process: Y = X[i - 3] + X[i — 2] + X[i — 1] + X[i] + X[i + 1] + X[i + 2]  + X[i +3] 
Example: X-> 1.2.3.4.5.6.7.8: output: Y-> 28.35 
Output: store result in vector Y. Also, display the result of first ten elements and last 10 elements of vector Y for all versions of kernels. 
