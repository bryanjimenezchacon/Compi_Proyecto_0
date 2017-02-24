.data
    A: .word 0 
    BB: .word 0 
    Temp1: .word 0 
    Temp2: .word 0 
.text
    main:
    lw $a0, BB
    li $a1, 314
    sub $v0, $a0, $a1
    sw $v0, Temp1
    lw $a0, Temp1
    lw $a1, A
    add $v0, $a0, $a1
    sw $v0, Temp2
    lw $a0, Temp2
    sw $a0, A
    la $v0, 10
    syscall
