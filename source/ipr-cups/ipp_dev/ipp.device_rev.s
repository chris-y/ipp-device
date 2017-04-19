VERSION = 1
REVISION = 1

.macro DATE
.ascii "20.4.2013"
.endm

.macro VERS
.ascii "ipp.device 1.1"
.endm

.macro VSTRING
.ascii "ipp.device 1.1 (20.4.2013)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: ipp.device 1.1 (20.4.2013)"
.byte 0
.endm
