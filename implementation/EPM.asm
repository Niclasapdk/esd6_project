	.global _EPM

_EPM:
	BSET FRCT ; Fractional bit set which shifts once to the left
	AMAR *AR0+ ; AR0 points to X0
	MPYM uns(*AR0-), *AR1+, AC0 ; ACO = X0 * Y1 ; MPYM = Multiply and save
	MACM *AR0, uns(*AR1-), AC0 ; ACO += X1 * Y0 ; MACM = multiply &accumulate + save
	MACM *AR0, *AR1, AC0 >> #16, AC0 ; ACO = ACO > >16 + X1 * Y1
	MOV AC0, dbl(*AR2) ; Save W1 and W0
	BCLR FRCT ; Clear fractional bit so following multiplications execute correctly
	RET