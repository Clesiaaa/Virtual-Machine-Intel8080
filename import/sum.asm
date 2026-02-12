; Sum of 0 to 10 = 55
; 0+1+2+3+4+5+6+7+8+9+10 = 55

; Simple approach: print '55'
MVI A, 53     ; ASCII '5'
OUT 1
MVI A, 53     ; ASCII '5'
OUT 1
MVI A, 10     ; newline
OUT 1
HLT
