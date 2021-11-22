  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.

	.text
main:
	li	$s0,0x30
loop:
	move	$a0,$s0		# copy from s0 to a0
	
	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window

	addi	$s0,$s0,3	## This line was changed. The constant determines how many steps to take in the ASCII talbe.
	
	li	$t0,0x5b
	ble	$s0,$t0,loop	## $t0 = 0x5b_16 = 01011011_2 = 91_10, so it stops looping when $s0 = 91.
	nop			# delay slot filler (just in case)

stop:	j	stop		# loop forever here
	nop			# delay slot filler (just in case)

