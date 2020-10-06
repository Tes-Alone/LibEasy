[bits 64]

	global asm_hex_double
	global asm_hex_float
	global asm_round0

[section .text]
asm_hex_double:
	vmovq rax, xmm0
	ret

asm_hex_float:
	vmovd eax, xmm0
	ret

asm_round0:
	roundsd xmm1, xmm0, 0b00000011 ;round mode
	movsd xmm0, xmm1
	ret