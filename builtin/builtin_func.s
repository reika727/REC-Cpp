.globl __builtin_put_ascii
__builtin_put_ascii:
    push   %rbp
    mov    %rsp, %rbp
    mov    %dil, -1(%rbp)
    mov    $1, %rax
    mov    $1, %rdi
    lea    -1(%rbp), %rsi
    mov    $1, %rdx
    syscall
    mov    %rbp, %rsp
    pop    %rbp
    ret

.globl __builtin_print_u64
__builtin_print_u64:
    push   %rbp
    mov    %rsp, %rbp
    mov    %rdi, %rax
    mov    $10, %rdi # decimal
    mov    $20, %rcx # floor(log10(2^64)+1)
BPU64_MAKE_STRING:
    xor    %rdx, %rdx
    div    %rdi
    add    $'0', %rdx
    mov    %dl, -21(%rbp, %rcx)
    loop   BPU64_MAKE_STRING
    lea    -20(%rbp), %rsi
    mov    $20, %rdx
BPU64_ADJUST:
    cmpb   $'0', (%rsi)
    jne    BPU64_WRITE
    inc    %rsi
    dec    %rdx
    cmp    %rsi, %rbp
    je     BPU64_ZERO
    jmp    BPU64_ADJUST
BPU64_ZERO:
    dec    %rsi
    inc    %rdx
BPU64_WRITE:
    mov    $1, %rax
    mov    $1, %rdi
    syscall
    mov    %rbp, %rsp
    pop    %rbp
    ret
