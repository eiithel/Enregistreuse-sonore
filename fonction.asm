
	.def _enableInterrupts
	.data

MASK_SELECTOR .set 0x00000004

	.text

_enableInterrupts
	.asmfunc

	MVK 1h, B1;global interrupt enable
	MVC CSR, B0
	OR B1, B0, B0
	MVC B0, CSR;

	MVK 13h, B1;NMI et EI4
	MVC IER, B0
	OR B1,B0,B0
	MVC B0,IER

	;MVK 04h, B2
	;MVC MUXL, B0
	;MV MUXL, B0
	;OR B2, MUXL, MUXL; Set selector


	;choisir selector
	MVKL MASK_SELECTOR, B1;
	MVKH MASK_SELECTOR, B1;

	;OR MUXL, B1, MUXL;
; MUXL ne pourra pas etre adresse comme ca en asm, faut trouver son @ et le co


	B B3 ; Branch pour retourner au code C, EVITER DE OVERWRITE B3
	NOP 5

	.endasmfunc


	.end
