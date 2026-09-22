	.file	"arith3.c"
	.text
	.globl	arith3
	.type	arith3, @function
arith3:
.LFB0:
	.cfi_startproc
	orl	%esi, %edx
	shrw	$9, %dx
	sete	%al
	movzbl	%al, %eax
	subl	%esi, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	arith3, .-arith3
	.ident	"GCC: (GNU) 16.2.1 20260819 (Red Hat 16.2.1-2)"
	.section	.note.GNU-stack,"",@progbits
