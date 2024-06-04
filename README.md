# CEPARCO SIMD Programming Project
Write the kernel in (1) C program; (2) an x86-64 assembly language; (3) x86 SIMD AVX2 assembly language using XMM register; (4) x86 SIMD AVX2 assembly language using YMM register. The kernel is to perform 1-D stencil of vector X place the result in vector Y. 

**Input**: Memory location n (integer) contains the length of the vector; Vectors X and Y are both 32-bit integer. 

**Process**: Y = X[i - 3] + X[i — 2] + X[i — 1] + X[i] + X[i + 1] + X[i + 2]  + X[i +3] 

**Output**: store result in vector Y. Also, display the result of first ten elements and last 10 elements of vector Y for all versions of kernels.

*Example: X-> 1.2.3.4.5.6.7.8: output: Y-> 28.35*

## Implementation
### C
```
void c_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y) {
    for (size_t i = 3; i < ARRAY_SIZE - 3; i++) {
        y[i - 3] = x[i - 3] + x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2] + x[i + 3];
    }
}
```
This implementation serves as the baseline for comparison. It gets the sum of the 7-element window centered at each element of the array x

### ASM x86-64
```
asm_1D_stencil:
	mov r10, rcx
	sub r10, 1
L1:
	xor rax, rax
	mov rcx, 7
	L2: 
		add rax, [rdx]
		add rdx, 4
		loop L2

	sub rdx, 24
	mov [r8], rax
	add r8, 4
	dec r10
	cmp r10, 0
	jnz L1

	xor rax, rax
	ret
```
This implementation has the same logic as the c function but at a lower level. The outer loop (L1) iterates through the array, while the inner loop (L2) sums the 7 elements.

### SIMD XMM register
```
xmm_1D_stencil:
	sub rcx, 0x0000_0000_0000_0003
	xor rsi, rsi
	xmmloop1:
		movdqu xmm0, [rdx+rsi*4]
		movdqu xmm1, [rdx+rsi*4+4]
		movdqu xmm2, [rdx+rsi*4+8]
		movdqu xmm3, [rdx+rsi*4+12]
		movdqu xmm4, [rdx+rsi*4+16]
		movdqu xmm5, [rdx+rsi*4+20]
		movdqu xmm6, [rdx+rsi*4+24]
		paddd xmm0, xmm1
		paddd xmm2, xmm3
		paddd xmm4, xmm5
		paddd xmm0, xmm6
		paddd xmm2, xmm4
		paddd xmm0, xmm2
		movdqu [R8+rsi*4], xmm0
		add rsi, 0x0000_0000_0000_0001
		sub rcx, 0x0000_0000_0000_0001
		cmp rcx, 0x0
		jg xmmloop1

	xor rax, rax
	ret
```
This implementation uses XMM registers to perform SIMD operations and utilizes the paddd instruction, which operates on 4 packed integers, allowing for parallel processing of 4 elements.

### SIMD YMM register
```
ymm_1D_stencil:
```
This version uses YMM registers (256-bit) for SIMD operations vpaddd instruction operates on 8 packed integers, doubling the parallelism compared to the XMM implementation.

## Table of Execution Time
### Debug Mode
|   | 2^20 | 2^26 | 2^30 |
| ------------- |-------------|------------- |------------- |
| C Implementation | right foo     | right foo     | right foo     |
| x86-64 Assembly | right bar     | right foo     | right foo     |
| SIMD XMM | right baz     | right foo     | right foo     |
| SIMD YMM | right baz     | right foo     | right foo     |

### Release Mode
|   | 2^20 | 2^26 | 2^30 |
| ------------- |-------------|------------- |------------- |
| C Implementation | right foo     | right foo     | right foo     |
| x86-64 Assembly | right bar     | right foo     | right foo     |
| SIMD XMM | right baz     | right foo     | right foo     |
| SIMD YMM | right baz     | right foo     | right foo     |

