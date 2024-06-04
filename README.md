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


### SIMD XMM register
blah

### SIMD YMM register
blah

## Table of Execution Time
blah

