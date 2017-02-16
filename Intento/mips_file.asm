.data
    A: .word 0 
    B: .word 0 
    CC: .word 0 
    Temp1: .word 0 
    Temp2: .word 0 
    DD: .word 0 
    Temp3: .word 0 
.text
    main:
    li $v0, 5
    syscall
    sw $v0, A
    li $v0, 5
    syscall
    sw $v0, B
    li $v0, 5
    syscall
    sw $v0, CC
    li $v0, 5
    syscall
    sw $v0, DD
    lw $a0, B
    lw $a1, CC
    add $v0, $a0, $a1
    sw $v0, Temp1
    lw $a0, Temp1
    lw $a1, A
    add $v0, $a0, $a1
    sw $v0, Temp2
    lw $a0, Temp2
    lw $a1, DD
    add $v0, $a0, $a1
    sw $v0, Temp3
    lw $a0, Temp3
    sw $a0, A
    li $v0, 1
    lw $a0, A 
    syscall 
    la $v0, 10
    syscall
