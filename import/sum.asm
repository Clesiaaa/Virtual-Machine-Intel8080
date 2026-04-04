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

OUT 0x01
HLT