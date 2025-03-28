.data

; Variables
C3b .word 0 ; Q15
driveResist .word 100 ; Q9.0 (0-500)

; Constants
; DO NOT CHANGE ORDER
part2Reflect .word 1599
part2ReflectRecip .word 20

.text

	.global _setDriveResist
_setDriveResist:
	; 10 bit adc so 1023 should correspond to 500 (approx)
	MOV T0, HI(AC0)
	MPYK #500, AC0
	SFTL AC0, #-10
	MOV AC0, *(driveResist)
	RET

	.global _ts808
_ts808:
part1:
	NEG T0 ; y1 = -x
part2:
	MOV T0, HI(AC0) ; AC0 = y1
	MPYK #3199, AC0 ; AC0 *= 2*r
	MOV dbl(*(part2Reflect)), pair(T2)
	MACMR *(C3b), T2, AC0 ; AC0 += r*C3b = a
	MACMR *(C3b), T2, AC0, AC1 ; AC1 = AC0 + R*C3b
	NEG AC1 ; AC1 = - AC1
	ADD *(C3b) << #15, AC1 ; AC1 += C3b (Q30)
	SFTS AC1, #-15 ; AC1 >>= 15 (get to Q15)
	MOV AC1, *(C3b) ; store C3b for next call
	MAC AC0, T3, AC1
	NEG AC1, T0 ; DUMMY REMOVE THIS SHIT PLS
	RET
; part2:
; 	MOV *(C3b), AC1
; 	ADD AC0<<#1, AC1 ; y1*2 + C3b
; 	MOV AC1, AC2 ; AC2 = a
; 	ADD *(C3b), AC1 ; AC1 += C3b
; 	MPY #-1599, AC1 ; AC1 *= -r (Q0.30)
; 	SFTL AC1, #-15 ; AC1 = AC1>>15
; 	ADD *(C3b), AC1 ; AC1 += C3b (AC1 = C3a, Q15)
; 	MOV AC1, *(C3b) ; save C3b for next call
; 	ADD AC1, AC2 ; AC2 = -y2 (Q3.15)
; 	NEG AC2, AC0 ; AC0 = y2 (Q3.15)
part3:
	RET