# CEPARCO SIMD Programming Project
#### AMBROSIO, ARCETA, MENDOZA, TAN
**Input**: Memory location n (integer) contains the length of the vector; Vectors X and Y are both **32-bit integer**. 

**Process**: Y[i] = X[i - 3] + X[i - 2] + X[i - 1] + X[i] + X[i + 1] + X[i + 2]  + X[i +3] 

**Output**: store result in vector Y. Also, display the result of first ten elements and last 10 elements of vector Y for all versions of kernels.

*Example: X-> 1.2.3.4.5.6.7.8: output: Y-> 28.35*

## Implementation
This project involves implementing the 1D stencil kernel in C, x86-64 assembly, and SIMD AVX2 with XMM and YMM registers. Outputs from the x86-64 assembly and SIMD AVX2 implementations are compared with a C computed result to confirm the correctness, and the last and first 10 elements of the output vectors are printed for verification.


### C
#### Code
```
void c_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y) {
    size_t i;
    for (i = 3; i < ARRAY_SIZE - 3; i++) {
        y[i - 3] = x[i - 3] + x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2] + x[i + 3];
    }
}
```
This implementation serves as the baseline for comparison. It gets the sum of the 7-element window centered at each element of the x vector. The y vector indexing is offset by 3 on both sides to account for the fact that the stencil outputs exactly 6 elements less than the input vector due to the window size. Thus this enables exact output size and no empty elements in the vector.

#### Screenshot
##### C Output at 2^20 (Debug)
![C_Output_20](Screenshots/C_Output_20.png)
##### C Output at 2^20 (Release)
![C_Output_20_rel](Screenshots/C_Output_20_rel.png)
##### C Output at 2^26 (Debug)
![C_Output_26](Screenshots/C_Output_26.png)
##### C Output at 2^26 (Release)
![C_Output_26_rel](Screenshots/C_Output_26_rel.png)
##### C Output at 2^30 (Debug)
![C_Output_30](Screenshots/C_Output_30.png)
##### C Output at 2^30 (Release)
![C_Output_30_rel](Screenshots/C_Output_30_rel.png)

### x86-64 ASM
#### Code
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
This implementation utilized a nested loop to achieve the stencil operation. the inner loop accumulated the value of the current value being calculated, and the 7 consecutive values are iterated through and added to an accumulator, after which the value is stored to the array. The pointers of the input and output vectors are adjusted accordingly between iterations to ensure that the correct values from the input vector are being accumulated and that the resulting sums are stored in the correct location in the output vector.

#### Screenshot
##### x86-64 ASM Output at 2^20 (Debug)
![ASM_Output_20](Screenshots/ASM_Output_20.png)
##### x86-64 ASM Output at 2^20 (Release)
![ASM_Output_20_rel](Screenshots/ASM_Output_20_rel.png)
##### x86-64 ASM Output at 2^26 (Debug)
![ASM_Output_26](Screenshots/ASM_Output_26.png)
##### x86-64 ASM Output at 2^26 (Release)
![ASM_Output_26_rel](Screenshots/ASM_Output_26_rel.png)
##### x86-64 ASM Output at 2^30 (Debug)
![ASM_Output_30](Screenshots/ASM_Output_30.png)
##### x86-64 ASM Output at 2^20 (Release)
![ASM_Output_30_rel](Screenshots/ASM_Output_30_rel.png)

### x86 SIMD AVX2 ASM using XMM
#### Code
```
xmm_1D_stencil:
	push rbx
	push rsi
	push rdi
	sub rsp,0x10
	movdqu [rsp],xmm6

	sub rcx, 0x0000_0000_0000_0004
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
		add rsi, 0x0000_0000_0000_0003
		sub rcx, 0x0000_0000_0000_0003
		cmp rcx, 0x0
		jg xmmloop1

	add ECX, 0x0000_0000_0000_0003
	mov R11, 7

	xmmloop2:
		xor RBX,RBX
		mov R10, R11
		xmmloop3:
			xor RAX,RAX
			mov RAX, [RDX+RSI*4]
			add RBX,RAX
			inc rsi
			dec R10
			cmp R10, 0x0
			jg xmmloop3
		sub RSI, R11
		mov [R8+RSI*4], RBX
		inc RSI
		loop xmmloop2


	movdqu xmm6,[rsp]
	add rsp,0x10
	pop rdi
	pop rsi
	pop rbx
	xor rax, rax
	ret
```
This implementation uses XMM registers to perform SIMD operations and utilizes the paddd instruction, which operates on 4 packed integers, allowing for parallel processing of 4 elements.

#### Screenshot
##### x86 SIMD AVX2 using XMM Output at 2^20 (Debug)
![XMM_Output_20](Screenshots/XMM_Output_20.png)
##### x86 SIMD AVX2 using XMM Output at 2^20 (Release)
![XMM_Output_20_rel](Screenshots/XMM_Output_20_rel.png)
##### x86 SIMD AVX2 using XMM Output at 2^26 (Debug)
![XMM_Output_26](Screenshots/XMM_Output_26.png)
##### x86 SIMD AVX2 using XMM Output at 2^26 (Release)
![XMM_Output_26_rel](Screenshots/XMM_Output_26_rel.png)
##### x86 SIMD AVX2 using XMM Output at 2^30 (Debug)
![XMM_Output_30](Screenshots/XMM_Output_30.png)
##### x86 SIMD AVX2 using XMM Output at 2^20 (Release)
![XMM_Output_30_rel](Screenshots/XMM_Output_30_rel.png)

### x86 SIMD AVX2 ASM using YMM
#### Code
```
ymm_1D_stencil:
	push rbx
	push rsi
	push rdi
	sub rsp,0x20
	vmovdqu [rsp],ymm6
	sub rsp,0x20
	vmovdqu [rsp],ymm7
	sub rsp,0x20
	vmovdqu [rsp],ymm8
	sub rsp,0x20
	vmovdqu [rsp],ymm9
	sub rsp,0x20
	vmovdqu [rsp],ymm10
	sub rsp,0x20
	vmovdqu [rsp],ymm11
	sub rsp,0x20
	vmovdqu [rsp],ymm12

	sub rcx, 0x0000_0000_0000_0008
	xor rsi, rsi
	ymmloop1:
		vmovdqu ymm0, [rdx+rsi*4]
		vmovdqu ymm1, [rdx+rsi*4+4]
		vmovdqu ymm2, [rdx+rsi*4+8]
		vmovdqu ymm3, [rdx+rsi*4+12]
		vmovdqu ymm4, [rdx+rsi*4+16]
		vmovdqu ymm5, [rdx+rsi*4+20]
		vmovdqu ymm6, [rdx+rsi*4+24]
		vpaddd ymm7, ymm0, ymm1
		vpaddd ymm8, ymm2, ymm3
		vpaddd ymm9, ymm4,ymm5
		vpaddd ymm13, ymm7, ymm6
		vpaddd ymm11, ymm8, ymm9
		vpaddd ymm12, ymm13, ymm11
		vmovdqu [R8+rsi*4], ymm12
		add rsi, 0x0000_0000_0000_0007
		sub rcx, 0x0000_0000_0000_0007
		cmp rcx, 0x0
		jg ymmloop1
	
	add ECX, 0x0000_0000_0000_0007
	mov R11, 7

	ymmloop2:
		xor RBX,RBX
		mov R10, R11
		ymmloop3:
			xor RAX,RAX
			mov RAX, [RDX+RSI*4]
			add RBX,RAX
			inc rsi
			dec R10
			cmp R10, 0x0
			jg ymmloop3
		sub RSI, R11
		mov [R8+RSI*4], RBX
		inc RSI
		loop ymmloop2


	vmovdqu ymm12,[rsp]
	add rsp,0x20
	vmovdqu ymm11,[rsp]
	add rsp,0x20
	vmovdqu ymm10,[rsp]
	add rsp,0x20
	vmovdqu ymm9,[rsp]
	add rsp,0x20
	vmovdqu ymm8,[rsp]
	add rsp,0x20
	vmovdqu ymm7,[rsp]
	add rsp,0x20
	vmovdqu ymm6,[rsp]
	add rsp,0x20
	pop rdi
	pop rsi
	pop rbx

	xor rax, rax
	ret
```
This version uses YMM registers (256-bit) for SIMD operations vpaddd instruction operates on 8 packed integers, doubling the parallelism compared to the XMM implementation.

#### Screenshot
##### x86 SIMD AVX2 using YMM Output at 2^20 (Debug)
![YMM_Output_20](Screenshots/YMM_Output_20.png)
##### x86 SIMD AVX2 using YMM Output at 2^20 (Release)
![YMM_Output_20_rel](Screenshots/YMM_Output_20_rel.png)
##### x86 SIMD AVX2 using YMM Output at 2^26 (Debug)
![YMM_Output_26](Screenshots/YMM_Output_26.png)
##### x86 SIMD AVX2 using YMM Output at 2^26 (Release)
![YMM_Output_26_rel](Screenshots/YMM_Output_26_rel.png)
##### x86 SIMD AVX2 using YMM Output at 2^30 (Debug)
![YMM_Output_30](Screenshots/YMM_Output_30.png)
##### x86 SIMD AVX2 using YMM Output at 2^20 (Release)
![YMM_Output_30_rel](Screenshots/YMM_Output_30_rel.png)

## Table of Execution Time
Execution times for each implementation are measured in both DEBUG and RELEASE mode. The kernel is ran 30 times to calculate the average runtime. Below is the table of average runtime of each implementation.

### Debug Mode
|   | 2^20 | 2^26 | 2^30 |
| ------------- |-------------|------------- |------------- |
| C | 2.299107 ms | 140.978020 ms | 2257.447447 ms |
| x86-64 ASM | 8.670397 ms | 541.459027 ms | 8749.174177 ms |
| x86 SIMD AVX2 ASM using XMM | 0.314783 ms | 29.459390 ms | 482.012747 ms |
| x86 SIMD AVX2 ASM using YMM | 0.250223 ms | 28.706993 ms | 472.057233 ms |

### Release Mode
|   | 2^20 | 2^26 | 2^30 |
| ------------- |-------------|------------- |------------- |
| C | 0.381130 ms | 30.060877 ms | 478.531403 ms |
| x86-64 ASM | 8.790763 ms | 545.266213 ms | 8722.438213 ms |
| x86 SIMD AVX2 ASM using XMM | 0.334360 ms | 30.458090 ms | 477.479007 ms |
| x86 SIMD AVX2 ASM using YMM | 0.270087 ms | 28.226107 ms | 465.343303 ms |

## Performance Analysis
no no 

