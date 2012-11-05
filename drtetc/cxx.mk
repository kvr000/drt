CC=gcc
CXX=g++

CXX_O=$(CXX) -c
CXX_S=$(CXX) -S

CC_O=$(CC) -c
CC_S=$(CC) -S

LDXX=$(CXX)
LDXX_SO=$(CXX) -shared
LDXX_EXEC=$(CXX)

OBJ_EXT=.o
AS_EXT=.s

EXEC_EXT=
SHLIB_EXT=.so
SHLIB_PREF=lib

# unfortunately, -Wconversion option became quite useless in gcc 4.3 and newer
# use older gcc for detecting this kind of warnings
CXXFLAGS=-fmessage-length=0 $(T_CXXFLAGS) $(U_CXXFLAGS) -Wall -W -Wconversion -Wno-unused-parameter -Wdisabled-optimization -Woverloaded-virtual -Wshadow -Wunused -Wno-unused-parameter -fpermissive -Wno-conversion \
# -Wno-inline

CXXINC=-I$(BUILD) -I$(INCDIR) -I$(INCDIR)/arch-$(DR_ARCH) -I$(INCDIR)/os-$(DR_OSTYPE)

SOCXXFLAGS=$(CXXFLAGS) -fPIC
SOLDXXFLAGS=$(LDFLAGS) $(T_LDXXFLAGS) -lpthread

EXECCXXFLAGS=$(CXXFLAGS) -fPIC
EXECLDXXFLAGS=$(CXXFLAGS) $(T_LDXXFLAGS)

include $(DRTETCDIR_)p/cxx.mk
