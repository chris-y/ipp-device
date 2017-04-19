VERSION = 1
REVISION = 2

.macro DATE
.ascii "12.3.2017"
.endm

.macro VERS
.ascii "ipp.device 1.2"
.endm

.macro VSTRING
.ascii "ipp.device 1.2 (12.3.2017)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: ipp.device 1.2 (12.3.2017)"
.byte 0
.endm
