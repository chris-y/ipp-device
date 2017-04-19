#
# Makefile for ipp.device
#

CC		= ppc-amigaos-gcc
CPP		= ppc-amigaos-g++
CFLAGS	= -N -Wall -O2 -fomit-frame-pointer -c -fno-builtin -gstabs \
		  -mmultiple -Wunused -D__USE_INLINE__ -D__NOGLOBALIFACE__ -I..
LDFLAGS	= -nostartfiles
LDLIBS	= ../cups/libcups.a -lstdc++
OBJS	= ipp_dev.o dev_68k.o ../ipr/ipr.o ../ipr/iprutil.o ../ipr/AttrGroups.o

ipp.device:	$(OBJS)
		$(CPP) -N $(LDFLAGS) -o $@.debug $(OBJS) $(LDLIBS) -Wl,--cref,-M,-Map=$@.map
		ppc-amigaos-strip -R.comment -R.dstart -R.dend $@.debug -o ipp.device

ipp_dev.o: ipp_dev.c

dev_68k.o: dev_68k.c

clean:
		-rm $(OBJS) *.map *.device *.debug
