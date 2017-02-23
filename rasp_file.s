.data
.balign 4
    string: .asciz "Result: %d"
    A: .word 512 


.text
.global main
.extern printf

main:

@Soy un store
    LDR R0, =18
    LDR R8, =A
    STR R0,[R8]
@Soy un Write
    push {ip, lr}
    ldr R1,[R8]
    ldr R0,=string
    bl printf
    pop {ip,pc}
@Soy un Halt
    mov r7, #1 
    mov r0, #0
    svc #0
    bx lr