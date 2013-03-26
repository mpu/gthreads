#
# Green threads context switch.
#
#   %rdi contains the current context structure.
#   %rsi contains the context to switch to.
#

.globl _gtswtch, gtswtch
_gtswtch:
gtswtch:

        mov     %rsp, 0x00(%rdi)
        mov     %r15, 0x08(%rdi)
        mov     %r14, 0x10(%rdi)
        mov     %r13, 0x18(%rdi)
        mov     %r12, 0x20(%rdi)
        mov     %rbx, 0x28(%rdi)
        mov     %rbp, 0x30(%rdi)

        mov     0x00(%rsi), %rsp
        mov     0x08(%rsi), %r15
        mov     0x10(%rsi), %r14
        mov     0x18(%rsi), %r13
        mov     0x20(%rsi), %r12
        mov     0x28(%rsi), %rbx
        mov     0x30(%rsi), %rbp

        ret
