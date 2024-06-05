; assembly part using x86-64
section .text
bits 64
default rel
global xmm_1D_stencil

; RCX - ARRAY_SIZE
; RDX - Address of input vector x -> [rdx] is value of x 
; R8 - Address of output vector y -> [r8] is value of y

xmm_1D_stencil:
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

	xor rax, rax
	ret