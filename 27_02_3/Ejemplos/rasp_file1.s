.data
.balign 4
    string: .asciz "Result: %d"
    C: .word 512 
    A: .word 512 
    Temp1: .word 512 


.text
.global main
.extern printf

main:

@Soy un store
    LDR R0, =8
    LDR R8, =C
    STR R0,[R8]

@Soy un add
    LDR R7, =C
    LDR R1,[R7]
    LDR R2, =2
    ADD R3, R1, R2
  LDR R8,=Temp1
    STR R3,[R8]
@Soy un CMPNZ
    LDR R1, =Temp1
    
    LDR R2, [R1]
    CMP R2,#0
     LDR R3, =C
    LDR R1,[R3]
    LDR R2, =A
    STREQ R1,[R2]
    LDR R1, =5
    LDR R2, =A
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