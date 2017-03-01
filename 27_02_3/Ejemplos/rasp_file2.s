.data
.balign 4
    string: .asciz "Result: %d"
    A: .word 512 
    B: .word 512 


.text
.global main
.extern printf

main:

@Soy un store
    LDR R0, =18
    LDR R8, =A
    STR R0,[R8]

@Soy un CMPNZ
    LDR R2,=0
    CMP R2,#0
     LDR R3, =A
    LDR R1,[R3]
    LDR R2, =B
    STREQ R1,[R2]
    LDR R1, =24
    LDR R2, =B
    STRNE R1,[R2]

@Soy un Write
    push {ip, lr}
    LDR R8,=A
    LDR R1,[R8]
    ldr R0,=string
    bl printf
    pop {ip,pc}
@Soy un Halt
    mov r7, #1 
    mov r0, #0
    svc #0
    bx lr