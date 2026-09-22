	.file	"exchange.c"
	.text
	.globl	exchange
	.type	exchange, @function
exchange:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rax
	movq	%rsi, (%rdi)
	ret
	.cfi_endproc
.LFE0:
	.size	exchange, .-exchange
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
