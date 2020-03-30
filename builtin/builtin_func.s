.globl __builtin_print_u64
__builtin_print_u64:
    push   %rbp
    mov    %rsp, %rbp
    movb   $0xa, -1(%rbp)
    mov    %rdi, %rax
    mov    $10, %rdi
    mov    $20, %rcx
BPU64_MAKE_STRING:
    xor    %rdx, %rdx
    div    %rdi
    add    $0x30, %rdx
    mov    %dl, -22(%rbp, %rcx)
    loop   BPU64_MAKE_STRING
    lea    -21(%rbp), %rsi
    mov    $21, %rdx
BPU64_ADJUST:
    cmpb   $0x30, (%rsi)
    jne    BPU64_WRITE
    cmpb   $0xa, 1(%rsi)
    je     BPU64_WRITE
    inc    %rsi
    dec    %rdx
    jmp    BPU64_ADJUST
BPU64_WRITE:
    mov    $1, %rax
    mov    $1, %rdi
    syscall
    mov    %rbp, %rsp
    pop    %rbp
    ret
