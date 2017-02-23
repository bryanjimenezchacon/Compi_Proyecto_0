.data
.baling 4
    string: .asciz "%d"
    BB: .word 512 
    c: .word 512 
    A: .word 512 
    Temp1: .word 512 
    c: .word 512 
    Temp2: .word 512 


.text
.global main
.extern printf

main:

@Soy un store
    LDR R0, #4
    LDR R8, =BB
    STR R0,[R8]

@Soy un store
    LDR R0, #1
    LDR R8, =c
    STR R0,[R8]

@Soy un add
    LDR R1, =BB
    LDR R2, #314
    add R0, R1, R2
    LDR R8,=Temp1
    STR R0,[R8]
@Soy un Sub
    LDR R1,=Temp1
    LDR R2,=c
    sub R0, R1, R2
    LDR R8,=Temp2
    STR R0,[R8]
@Soy un store
    LDR R0,=Temp2
    LDR R8, =A
    STR R0,[R8]
@Soy un Write
    push{ip, lr}
    ldr R3,=string
    bl printf
    pop{ip,pc}
@Soy un Halt
    mov r7, #1 
    mov r0, #0
    svc #0
    bx lr