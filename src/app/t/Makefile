BUILD=_build
BUILD_=$(BUILD)/
GEN=$(BUILD_)_gen
GEN_=$(GEN)/
OBJDIR=$(BUILD_)$(DR_TARG)
OBJDIR_=$(OBJDIR)/
DIRS=app/t
DIRS_=app/t/
DIRS_R=app/t/
DIRM=src/$(DIRS)
DIRM_=src/$(DIRS_)
DIRM_R=src/$(DIRS_)
TOPDIR=../../..
TOPDIR_=../../../
TOPDIR_R=../../../
SRCDIR=../..
SRCDIR_=../../
SRCDIR_R=../../
ETCDIR=$(TOPDIR_)etc
ETCDIR_=$(ETCDIR)/
BINDIR=$(TOPDIR_)bin
BINDIR_=$(TOPDIR_)bin/
LIBDIR=$(TOPDIR_)lib
LIBDIR_=$(TOPDIR_)lib/
INCDIR=$(TOPDIR_)include
INCDIR_=$(TOPDIR_)include/

DR_BUILD_CXX_FLAGS= -DDR_HAS_THREAD_LOCAL -DDR_OS_UNIX

all: targ_all
default: targ_all
no: nothing

gmake:
	dr_genmake ../def/bsd.txt $(DIRS)

include $(ETCDIR_)main.mk

lgh:
	$(MAKE) -f Make_gh

include $(ETCDIR_)cxx.mk

ldeltags:
	rm -f tags
tags: ldeltags
	run_p gen_tags .
deltags: ldeltags


FLAGS_cxx=$(CXXINC) $(EXECXXFLAGS) -I. -I..
FLAGS_exec=$(EXELDXXFLAGS)

$(OBJDIR)/tlink_drapp$(OBJ_EXT): tlink_drapp.cxx
	$(CXX_O) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_tlink_drapp=1
$(OBJDIR)/tlink_drapp$(AS_EXT): tlink_drapp.cxx
	$(CXX_S) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_tlink_drapp=1
$(OBJDIR)/t0$(OBJ_EXT): t0.cxx
	$(CXX_O) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_t0=1
$(OBJDIR)/t0$(AS_EXT): t0.cxx
	$(CXX_S) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_t0=1
ldistinc:

distinc:
	$(MAKE) ldistinc

$(GEN):
	mkdir -p $(GEN)
$(OBJDIR):
	mkdir -p $(OBJDIR)
tlink_drapp: $(OBJDIR)/tlink_drapp$(OBJ_EXT)
	$(LDXX_EXEC) -o $@ -Wl,-rpath,$(LIBDIR) -L$(LIBDIR) $(OBJDIR)/tlink_drapp$(OBJ_EXT) -ldrtestenv -ldrapp -ldr $(FLAGS_exec)
t0: $(OBJDIR)/t0$(OBJ_EXT)
	$(LDXX_EXEC) -o $@ -Wl,-rpath,$(LIBDIR) -L$(LIBDIR) $(OBJDIR)/t0$(OBJ_EXT) -ldrtestenv -ldrapp -ldr $(FLAGS_exec)
test:
	$(MAKE) ltest

ltest:
	./t0

slist:
	$(MAKE) lslist

lslist:
	echo $(DIRS_)tlink_drapp.cxx
	echo $(DIRS_)t0.cxx
	echo $(DIRS_)mklist

outclean: loutclean
loutclean:
	rm -f tlink_drapp t0

clean: lclean
lclean: loutclean limmclean
limmclean:
	rm -f $(OBJDIR)/tlink_drapp$(OBJ_EXT) $(OBJDIR)/tlink_drapp$(OBJ_EXT).dep $(OBJDIR)/t0$(OBJ_EXT) $(OBJDIR)/t0$(OBJ_EXT).dep

wclean: lwclean
lwclean: lclean
	rm -rf $(BUILD)

lnothing:
nothing: lnothing

$(OBJDIR)/all_deps.mi:
	mkdir -p $(GEN)
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/dummy.dep
	echo "include $(OBJDIR)/*.dep" > $@
include $(OBJDIR)/all_deps.mi

gh: lgh
include Make_gh

targ_pre:
targ_init: targ_pre ldistinc $(GEN) $(OBJDIR)
targ_local: targ_init tlink_drapp t0
targ_post: targ_local
targ_all: targ_pre targ_init targ_local targ_post

