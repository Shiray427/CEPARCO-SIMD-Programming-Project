; assembly part using x86-64
section .text
bits 64
default rel
global ymm_1D_stencil

; RCX - ARRAY_SIZE
; RDX - Address of input vector x -> [rdx] is value of x 
; R8 - Address of output vector y -> [r8] is value of y

ymm_1D_stencil:
	push rbx
	push rsi
	push rdi

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
		vpaddd ymm10, ymm7, ymm6
		vpaddd ymm11, ymm8, ymm9
		vpaddd ymm12, ymm10, ymm11
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


	pop rdi
	pop rsi
	pop rbx

	xor rax, rax
	ret