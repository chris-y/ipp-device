VERSION		EQU	1
REVISION	EQU	1

DATE	MACRO
		dc.b '20.4.2013'
		ENDM

VERS	MACRO
		dc.b 'ipp.device 1.1'
		ENDM

VSTRING	MACRO
		dc.b 'ipp.device 1.1 (20.4.2013)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: ipp.device 1.1 (20.4.2013)',0
		ENDM
