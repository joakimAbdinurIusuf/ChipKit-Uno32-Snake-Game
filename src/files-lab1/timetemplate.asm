  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li 	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

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

	PUSH ($s0)
	PUSH ($s1)
	addi	$s0,$0,0	## i=0
	addi	$s1,$0,30	## antal loops		170 f�r 1ms i mars	ca 7000 f�r 1ms med ChipKit
	

	
	slt	$t0,$0,$a0	## while s0>0 (while ms>0)
	addi	$a0,$a0,-1	## ms--
	bne	$t0,$0,msloop
	
	nop
	
	POP ($s1)
	POP ($s0)
	
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
	PUSH ($ra)
	PUSH ($s0)
	PUSH ($s1)
	PUSH ($s2)
	
	move $s0, $a0
	
	andi $t0, $a1, 0xf0
	srl $t0, $t0, 4
	
	andi $t1, $a1, 0xf
	
	add $t2, $t0, $t1
	beq $t2, $0, ding
	
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
	
	POP ($s2)
	POP ($s1)
	POP ($s0)
	POP ($ra)
	
	jr $ra
	nop
	
ding:
	li $t3, 0x44
	sb $t3, 0($s0)
	
	li $t4, 0x49
	sb $t4, 1($s0)
	
	li $t5, 0x4e
	sb $t5, 2($s0)
	
	li $t6, 0x47
	sb $t6, 3($s0)
	
	li $t7, 0x00
	sb $t7, 4($s0)
	
	li 	$a0,10
	li	$v0,11
	syscall
	nop
	
	j main
	
	## Questions:
	
	## Saved and restored: $ra and $s0-$s2. 
	## $ra is saved because a new address is stored in it when we jump and link to hexasc.
	## So if we don't save and restore it we don't return to main. $s0-$s2 are saved because
	## we assume hexasc might change their values, so we save and restore them. And we might
	## call a function that changes their values at a future date so it is good practice to
	## save and restore them S-registers are so called preserved registers. 
	
	## Used but not saved. $a0-$a1 and $v0. Because these are nonpreserved registers and
	## are allowed to be changed. 
	
	## Handle 5: 108-111 for me. The digit saved in location 3. 

delaytwo:
	addi $t0, $0, 0		## i = 0
	addi $t1, $0, 120	## Number of loops
	
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
	j delaytwo
	nop
	
time2stringTwo:
	PUSH ($ra)
	PUSH ($s0)
	PUSH ($s1)
	PUSH ($s2)
	
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
	
	POP ($s2)
	POP ($s1)
	POP ($s0)
	POP ($ra)
	
	jr $ra
	nop
	
