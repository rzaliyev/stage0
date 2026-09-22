	.file	"leaq.c"
	.text
	.globl	scale
	.type	scale, @function
scale:
.LFB0:
	.cfi_startproc
	addl	$10, %edi
	imull	%esi, %edi
	leal	(%rdx,%rdi), %eax
	ret
	.cfi_endproc
.LFE0:
	.size	scale, .-scale
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
