CXX=g++

CXX_O=$(CXX) -c
CXX_S=$(CXX) -S

LDXX=$(CXX)
LDXX_SO=$(CXX) -shared
LDXX_EXEC=$(CXX)

OBJ_EXT=.o
AS_EXT=.s

EXEC_EXT=
SHLIB_EXT=.so
SHLIB_PREF=lib

CXXFLAGS=-fmessage-length=0 $(T_CXXFLAGS) $(U_CXXFLAGS) -Wall -W -Wconversion -Wno-unused-parameter -Wdisabled-optimization -Woverloaded-virtual -Wshadow -Wunused -Wno-unused-parameter -fpermissive \
# -Wno-inline

CXXINC=-I$(BUILD) -I$(INCDIR) -I$(INCDIR)/arch-$(DR_ARCH) -I$(INCDIR)/os-$(DR_OSTYPE)

SOCXXFLAGS=$(CXXFLAGS) -fPIC
SOLDXXFLAGS=$(LDFLAGS) $(T_LDXXFLAGS) -lpthread

EXECXXFLAGS=$(CXXFLAGS) -fPIC
EXELDXXFLAGS=$(CXXFLAGS) $(T_LDXXFLAGS) -lpthread

include $(ETCDIR_)p/cxx.mk
