; assembly part using x86-64
section .text
bits 64
default rel
global asm_1D_stencil

; RCX - ARRAY_SIZE
; RDX - Address of input vector x -> [rdx] is value of x 
; R8 - Address of output vector y -> [r8] is value of y

asm_1D_stencil:
	mov r10, rcx
	dec r10
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