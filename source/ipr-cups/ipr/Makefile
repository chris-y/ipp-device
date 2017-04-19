#
# "$Id:  $"
#
#   ipr Makefile
#
#   Copyright 2004 by Peter Michalek, all rights reserved.
#
#

include ../Makedefs
IPR_INSTALL_DIR=/usr/bin
#IPR_INSTALL_DIR=/tmp/itest
TARGET=$(IPR_INSTALL_DIR)/ipr

.cpp.o:
	echo Compiling $<...
	$(CXX) $(OPTIM) $(CXXFLAGS) -c $<
	#$(CXX) $(CXXFLAGS) -c $<
#
# Object files...
#
#LIBOBJS =       ../cups/attr.o ../cups/auth.o ../cups/dest.o ../cups/emit.o ../cups/encode.o ../cups/getputfile.o \
#                ../cups/http.o ../cups/http-addr.o ../cups/http-support.o ../cups/ipp.o ../cups/ipp-support.o \
#                ../cups/language.o ../cups/mark.o ../cups/md5.o ../cups/md5passwd.o ../cups/options.o ../cups/page.o \
#                ../cups/ppd.o ../cups/snprintf.o ../cups/string.o ../cups/tempfile.o ../cups/usersys.o ../cups/util.o
	
OBJS	= ipr.o iprutil.o AttrGroups.o 


#
# Targets in this directory...
#

TARGETS	=	ipr



#
# Make all targets...
#

all:	$(TARGETS)


#
# Remove object and target files...
#

clean:
	$(RM) $(OBJS) $(TARGETS) 


#
# Update dependencies (without system header dependencies...)
#

depend:
	makedepend -Y -I.. -fDependencies $(OBJS:.o=.cpp) >/dev/null 2>&1


#
# Install object and target files...
#

install:	all
	cp ipr $(TARGET) 
	cp ipp-template $(IPR_INSTALL_DIR) 
	ln -f -s $(TARGET) $(IPR_INSTALL_DIR)/ipq
	ln -f -s $(TARGET) $(IPR_INSTALL_DIR)/iprm
	ln -f -s $(TARGET) $(IPR_INSTALL_DIR)/ipc

#
# testhttp (dependency on static CUPS library is intentional)
#

ipr:	$(OBJS) ../cups/libcups.a
	echo Linking $@...
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) ../cups/libcups.a -lssl -lcrypto -lz -lregex -lstdc++
#	$(CXX) $(LDFLAGS) -o $@ $(OBJS) -lstdc++

#
# Dependencies...
#
include Dependencies

