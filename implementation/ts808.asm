.data

C3b .word 0 ; Q15
driveResist .word 100 ; Q9.0 (0-500)

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
	NEG T0, AC0 ; y1 = -x
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