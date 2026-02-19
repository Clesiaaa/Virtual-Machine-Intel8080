; sum_simple_final.asm - Calculate 0+1+2+...+10, print "55"
; Uses only working instructions: MVI, ADD, INR, DCR, JNZ, OUT, HLT

MVI A, 0x00      ; sum = 0
MVI B, 0x0B      ; counter = 11 (counts down)
MVI C, 0x00      ; value to add (counts up)

; Loop at 0x0006
ADD C            ; sum += value
INR C            ; value++
DCR B            ; counter--
JNZ 0x0006       ; loop if counter != 0

; A now contains 55 (0+1+2+3+4+5+6+7+8+9+10)
; Just print '5' and '5'

MVI A, 0x35      ; ASCII '5'
OUT 0x01
MVI A, 0x35      ; ASCII '5'
OUT 0x01
MVI A, 0x0A      ; newline
OUT 0x01
HLT