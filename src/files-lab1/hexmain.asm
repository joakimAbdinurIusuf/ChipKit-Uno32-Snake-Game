  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17		# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

hexasc:
	andi $v0, $a0, 0xf	## 0xf is 0x0000...1111, and only 1 AND 1 = 1, so only the 4 LSB can be one
	ble $v0, 9, handleZeroToNine	## if $v0 <= 9 branch to the function handleZeroToNine
	addi $v0, $v0, 0x37	## add 0x37 (55) to convert the decimal number in $v0 to the corresponding ascii number 
				## Note: A = 10, so we counted back from 65 by 10 and got the number 0x37 (55)
	jr $ra
	
handleZeroToNine:
	addi $v0, $v0, 0x30	## add 0x30 (48) to convert the decimal number in $v0 to the corresponding ascii number 
	jr $ra
	
	
## Note on $ra: If we would have jal to the same function from main and a subroutine, $ra would have been overwritten.
## But it is not being overwritten right now so there is no need to save and restore $ra.

## Questions

## - 17 returns 1 because it is 00010001 in binary, but hexasc makes sure that only the 4 LSB can contain a 1, so
##   it would be transformed to 00000001 in binary, which is 1.

## - The input values $v0 and 9. It checks if $v0 is less than or equal to nine. If it is then the program branches
##   to another location.
