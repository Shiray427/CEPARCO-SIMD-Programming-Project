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
	sub r10, 3
L1:
	xor rax, rax
	mov rcx, 7
	L2: 
		add rax, [rdx]
		add rdx, 32
		loop L2
	sub rdx, 192
	mov [r8], rax
	add r8, 32
	sub r10, 1
	cmp r10, 0
	jnz L1

	xor rax, rax
	ret