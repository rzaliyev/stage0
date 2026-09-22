	.file	"arith.c"
	.text
	.globl	arith
	.type	arith, @function
arith:
.LFB0:
	.cfi_startproc
	xorq	%rsi, %rdi
	leaq	(%rdx,%rdx,2), %rax
	salq	$4, %rax
	andl	$252645135, %edi
	subq	%rdi, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	arith, .-arith
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
