	.file	"shift.c"
	.text
	.globl	shift_left4_rightn
	.type	shift_left4_rightn, @function
shift_left4_rightn:
.LFB0:
	.cfi_startproc
	movq	%rdi, %rax
	salq	$4, %rax
	movl	%esi, %ecx
	sarq	%cl, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	shift_left4_rightn, .-shift_left4_rightn
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
