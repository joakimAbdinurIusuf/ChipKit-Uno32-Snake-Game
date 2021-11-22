  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #

## .global makes labels accessible from other files. It is important in this task because
## there are probably other files using the labels above. 

.global hexasc
.global delay
.global time2string
.global enable_interrupt

.macro	PUSH reg 
	addi	$sp,$sp,-4
	sw	\reg,0($sp) 
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.data
 
.text

hexasc:
	andi $v0, $a0, 0xf	## 0xf is 0x0000...1111, and only 1 AND 1 = 1, so only the 4 LSB can be one
	ble $v0, 9, handleZeroToNine	## if $v0 <= 9 branch to the function handleZeroToNine
	nop
	addi $v0, $v0, 0x37	## add 0x37 (55) to convert the decimal number in $v0 to the corresponding ascii number 
				## Note: A = 10, so we counted back from 65 by 10 and got the number 0x37 (55)
	jr $ra
	nop
	
handleZeroToNine:
	addi $v0, $v0, 0x30	## add 0x30 (48) to convert the decimal number in $v0 to the corresponding ascii number 
	jr $ra
	nop
	
delay:

	PUSH $s0
	PUSH $s1
	addi	$s0,$0,0	## i=0
	addi	$s1,$0,6500	## antal loops		170 f�r 1ms i mars	ca 7000 f�r 1ms med ChipKit
	

	
	slt	$t0,$0,$a0	## while s0>0 (while ms>0)
	addi	$a0,$a0,-1	## ms--
	bne	$t0,$0,msloop
	
	nop
	
	POP $s1
	POP $s0
	
	jr	$ra
	nop
	
msloop:
	slt	$t0,$s0,$s1	## if i<4711 ...
	addi	$s0,$s0,1	## i = i + 1 
	bne	$t0,$0,msloop	## ... loop
	nop
	
	j	delay
	nop

time2string:
	PUSH $ra
	PUSH $s0
	PUSH $s1
	PUSH $s2
	
	move $s0, $a0
	
	andi $a0, $a1, 0xf000 
	srl $a0, $a0, 12 ##rd, rt, shamt (shift amount)
	jal hexasc
	nop
	sb $v0, 0($s0)
	
	andi $a0, $a1, 0xf00 
	srl $a0, $a0, 8 
	jal hexasc
	nop
	sb $v0, 1($s0)
	
	andi $a0, $a1, 0xf0 
	srl $a0, $a0, 4 ##rd, rt, shamt (shift amount)
	jal hexasc
	nop
	sb $v0, 3($s0)
	
	andi $a0, $a1, 0xf 
	jal hexasc
	nop
	sb $v0, 4($s0)
	
	li $s1, 0x3A
	sb $s1, 2($s0)
	
	li $s2, 0x00
	sb $s2, 5($s0) ## removing the null byte prints "more text lots of text"; null bytes indicate end of string
	
	POP $s2
	POP $s1
	POP $s0
	POP $ra
	
	jr $ra
	nop
	
delayTwo:
	addi $t0, $0, 0		## i = 0
	addi $t1, $0, 6500	## Number of loops
	
	addi $t2, $0, 1
	ble $t2, $a0, forLoop
	nop
	
	jr $ra
	nop
	
forLoop:
	addi $t0, $t0, 1
	ble $t0, $t1, forLoop
	nop
	
	addi $a0, $a0, -1
	j delayTwo
	nop

# 3g)	
enable_interrupt:
	ei 
	jr $ra
	nop

