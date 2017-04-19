#
# "$Id: Makefile,v 1.49 2004/02/25 20:14:50 mike Exp $"
#
#   Top-level Makefile for the Common UNIX Printing System (CUPS).
#
#   Copyright 1998-2004 by Peter Michalek, all rights reserved.
#
#
include Makedefs

#
# Directories to make...
#

DIRS	=	cups ipr

#
# Make all targets...
#

all:
	for dir in $(DIRS); do\
		echo Making all in $$dir... ;\
		(cd $$dir ; $(MAKE) $(MFLAGS)) || exit 1;\
	done

#
# Remove object and target files...
#

clean:
	for dir in $(DIRS); do\
		echo Cleaning in $$dir... ;\
		(cd $$dir; $(MAKE) $(MFLAGS) clean) || exit 1;\
	done

#
# Make dependencies
#

depend:
	for dir in $(DIRS); do\
		echo Making dependencies in $$dir... ;\
		(cd $$dir; $(MAKE) $(MFLAGS) depend) || exit 1;\
	done


#
# Install object and target files...
#

install:	installhdrs
	for dir in $(DIRS); do\
		echo Installing in $$dir... ;\
		(cd $$dir; $(MAKE) $(MFLAGS) install) || exit 1;\
	done

#
# Install source and header files...
#

installsrc:
	gnutar --dereference --exclude=CVS -cf - . | gnutar -C $(SRCROOT) -xf -


#
# Make software distributions using EPM (http://www.easysw.com/epm)...
#

#
# End of "$Id: $".
#
