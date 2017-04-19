VERSION		EQU	1
REVISION	EQU	2

DATE	MACRO
		dc.b '12.3.2017'
		ENDM

VERS	MACRO
		dc.b 'ipp.device 1.2'
		ENDM

VSTRING	MACRO
		dc.b 'ipp.device 1.2 (12.3.2017)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: ipp.device 1.2 (12.3.2017)',0
		ENDM
