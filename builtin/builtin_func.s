pow10:
    push %rbp
    mov  %rsp, %rbp
    mov  $1, %rax
    cmp  $0, %rdi
    je   POW10_RET
    mov  %rdi, %rcx
    mov  $10, %rdi
POW10_LOOP:
    mul  %rdi
    loop POW10_LOOP
POW10_RET:
    mov  %rbp, %rsp
    pop  %rbp
    ret

get_decimal_low_8:
    push %rbp
    mov  %rsp, %rbp
    sub  $16, %rsp  # original arg,  result
    movq $0, -16(%rbp)
    mov  %rdi, -8(%rbp)
    mov  $8, %rcx
PDL8_LOOP:
    shrq $8, -16(%rbp)
    lea  -1(%rcx), %rdi
    push %rcx
    call pow10
    pop  %rcx
    mov  %rax, %rdi
    mov  -8(%rbp), %rax
    mov  $0, %rdx
    div  %rdi
    mov  $10, %rdi
    mov  $0, %rdx
    div  %rdi
    add  $0x30, %rdx
    shl  $56, %rdx
    add  %rdx, -16(%rbp)
    loop PDL8_LOOP
    mov  -16(%rbp), %rax
    mov  %rbp, %rsp
    pop  %rbp
    ret

.globl __builtin_print_u64
__builtin_print_u64:
    push %rbp
    mov  %rsp, %rbp
    sub  $32, %rsp # data stock
    movq $0x0a, -8(%rbp)
    mov  $3, %rcx
    cmp  $0, %rdi
    jnz  BP64_GET_DATA
    movb $0x30, -9(%rbp)
    lea  -9(%rbp), %rsi
    mov  $2, %rdx
    jmp  BP64_WRITE
BP64_GET_DATA:
    push %rdi
    push %rcx
    call get_decimal_low_8
    pop  %rcx
    pop  %rdi
    mov  %rax, -40(%rbp, %rcx, 8)
    mov  %rdi, %rax
    mov  $0, %rdx
    mov  $100000000, %rsi
    div  %rsi
    mov  %rax, %rdi
    loop BP64_GET_DATA
    lea  -28(%rbp), %rsi
    mov  $21, %rdx
BP64_ADJUST:
    cmpb $0x30,(%rsi)
    jnz  BP64_WRITE
    inc  %rsi
    dec  %rdx
    jmp  BP64_ADJUST
BP64_WRITE:
    mov  $1, %rax
    mov  $1, %rdi
    syscall
    mov  $0, %rax
    mov  %rbp, %rsp
    pop  %rbp
    ret
