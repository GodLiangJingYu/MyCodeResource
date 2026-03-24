	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
@feat.00 = 0
	.file	"example.c"
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
main:                                   # @main
.seh_proc main
# %bb.0:
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$464, %rsp                      # imm = 0x1D0
	.seh_stackalloc 464
	leaq	128(%rsp), %rbp
	.seh_setframe %rbp, 128
	.seh_endprologue
	callq	__main
	movl	$0, 332(%rbp)
	movl	$10, 328(%rbp)
	movl	$20, 324(%rbp)
	movl	$25, -84(%rbp)
	movl	328(%rbp), %ecx
	movl	324(%rbp), %edx
	callq	add
	movl	%eax, 320(%rbp)
	cmpl	$25, 320(%rbp)
	jle	.LBB0_2
# %bb.1:
	leaq	.L.str(%rip), %rcx
	callq	printMessage
	jmp	.LBB0_3
.LBB0_2:
	leaq	.L.str.1(%rip), %rcx
	callq	printMessage
.LBB0_3:
	movl	$0, -88(%rbp)
.LBB0_4:                                # =>This Inner Loop Header: Depth=1
	cmpl	$5, -88(%rbp)
	jge	.LBB0_7
# %bb.5:                                #   in Loop: Header=BB0_4 Depth=1
	movl	-88(%rbp), %ecx
	shll	%ecx
	movslq	-88(%rbp), %rax
	movl	%ecx, -80(%rbp,%rax,4)
# %bb.6:                                #   in Loop: Header=BB0_4 Depth=1
	movl	-88(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -88(%rbp)
	jmp	.LBB0_4
.LBB0_7:
	movl	328(%rbp), %edx
	movl	324(%rbp), %r8d
	movl	320(%rbp), %r9d
	leaq	.L.str.2(%rip), %rcx
	callq	printf
	movl	-84(%rbp), %edx
	movl	globalVar(%rip), %r8d
	leaq	.L.str.3(%rip), %rcx
	callq	printf
	xorl	%eax, %eax
	.seh_startepilogue
	addq	$464, %rsp                      # imm = 0x1D0
	popq	%rbp
	.seh_endepilogue
	retq
	.seh_endproc
                                        # -- End function
	.def	add;
	.scl	2;
	.type	32;
	.endef
	.globl	add                             # -- Begin function add
	.p2align	4
add:                                    # @add
.seh_proc add
# %bb.0:
	pushq	%rax
	.seh_stackalloc 8
	.seh_endprologue
	movl	%ecx, 4(%rsp)
	movl	%edx, (%rsp)
	movl	4(%rsp), %eax
	addl	(%rsp), %eax
	.seh_startepilogue
	popq	%rcx
	.seh_endepilogue
	retq
	.seh_endproc
                                        # -- End function
	.def	printMessage;
	.scl	2;
	.type	32;
	.endef
	.globl	printMessage                    # -- Begin function printMessage
	.p2align	4
printMessage:                           # @printMessage
.seh_proc printMessage
# %bb.0:
	subq	$40, %rsp
	.seh_stackalloc 40
	.seh_endprologue
	movq	%rcx, 32(%rsp)
	movq	32(%rsp), %rdx
	leaq	.L.str.4(%rip), %rcx
	callq	printf
	nop
	.seh_startepilogue
	addq	$40, %rsp
	.seh_endepilogue
	retq
	.seh_endproc
                                        # -- End function
	.data
	.globl	globalVar                       # @globalVar
	.p2align	2, 0x0
globalVar:
	.long	42                              # 0x2a

	.section	.rdata,"dr"
.L.str:                                 # @.str
	.asciz	"Result is greater than 25"

.L.str.1:                               # @.str.1
	.asciz	"Result is 25 or less"

.L.str.2:                               # @.str.2
	.asciz	"x = %d, y = %d, result = %d\n"

.L.str.3:                               # @.str.3
	.asciz	"squared = %d, globalVar = %d\n"

.L.str.4:                               # @.str.4
	.asciz	"%s\n"

	.section	.debug$S,"dr"
	.p2align	2, 0x0
	.long	4                               # Debug section magic
	.long	241
	.long	.Ltmp1-.Ltmp0                   # Subsection size
.Ltmp0:
	.short	.Ltmp3-.Ltmp2                   # Record length
.Ltmp2:
	.short	4353                            # Record kind: S_OBJNAME
	.long	0                               # Signature
	.byte	0                               # Object name
	.p2align	2, 0x0
.Ltmp3:
	.short	.Ltmp5-.Ltmp4                   # Record length
.Ltmp4:
	.short	4412                            # Record kind: S_COMPILE3
	.long	0                               # Flags and language
	.short	208                             # CPUType
	.short	21                              # Frontend version
	.short	1
	.short	5
	.short	0
	.short	21015                           # Backend version
	.short	0
	.short	0
	.short	0
	.asciz	"clang version 21.1.5"          # Null-terminated compiler version string
	.p2align	2, 0x0
.Ltmp5:
.Ltmp1:
	.p2align	2, 0x0
	.addrsig
	.addrsig_sym add
	.addrsig_sym printMessage
	.addrsig_sym printf
	.addrsig_sym globalVar
