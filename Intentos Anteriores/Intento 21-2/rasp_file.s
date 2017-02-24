.data
.balign 4
    string: .asciz "Result: %d"
    A: .word 512 
    BB: .word 512 
    Temp1: .word 512 
    C: .word 512 
    Temp2: .word 512 
    Temp3: .word 512 


.text
.global main
.extern printf

main:

@Soy un Read
    mov R7, #3 
    mov R0, #1 
    ldr R1, =BB
    mov R2, #12
    svc #0
@Soy un Read
    mov R7, #3 
    mov R0, #1 
    ldr R1, =C
    mov R2, #12
    svc #0
@Soy un add
    LDR R1, =BB
    LDR R2, =314
    add R0, R1, R2
  LDR R8,=Temp1
    STR R0,[R8]
@Soy un Sub
    LDR R1, =C
    LDR R2, =25
    sub R0, R1, R2
  LDR R8,=Temp2
    STR R0,[R8]
@Soy un Sub
    LDR R1, =Temp1
    LDR R2, =Temp2
    sub R0, R1, R2
  LDR R8,=Temp3
    STR R0,[R8]
@Soy un store
    LDR R0,=Temp3
    LDR R8, =A
    STR R0,[R8]
@Soy un Write
    push {ip, lr}
    ldr R3,=string
    bl printf
    pop {ip,pc}
@Soy un Halt
    mov r7, #1 
    mov r0, #0
    svc #0
    bx lr