BUILD=_build
BUILD_=$(BUILD)/
GEN=$(BUILD_)_gen
GEN_=$(GEN)/
OBJDIR=$(BUILD_)$(DR_TARG)
OBJDIR_=$(OBJDIR)/
DIRS=app
DIRS_=app/
DIRS_R=app/
DIRM=src/$(DIRS)
DIRM_=src/$(DIRS_)
DIRM_R=src/$(DIRS_)
TOPDIR=../..
TOPDIR_=../../
TOPDIR_R=../../
SRCDIR=..
SRCDIR_=../
SRCDIR_R=../
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
deltags:
	$(MAKE) ldeltags
	$(MAKE) -C t deltags



FLAGS_shlib=$(SOLDXXFLAGS)
FLAGS_cxx=$(CXXINC) $(SOCXXFLAGS)

$(OBJDIR)/ApplicationCache$(OBJ_EXT): appcache/ApplicationCache.cxx
	$(CXX_O) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_drapp=1
$(OBJDIR)/ApplicationCache$(AS_EXT): appcache/ApplicationCache.cxx
	$(CXX_S) $< -o $@ -MD -MF $@.dep $(DR_BUILD_CXX_FLAGS)  $(FLAGS_cxx) -DCOMPILING_drapp=1
$(OBJDIR)/ApplicationCache$(OBJ_EXT): $(GEN_)ApplicationCache.hxx $(GEN_)ApplicationCache-def.hxx
$(INCDIR_)dr/app:
	mkdir -p $(INCDIR_)dr/app
DIST_INC_di: $(INCDIR_)dr/app $(INCDIR_)dr/app/def.hxx $(INCDIR_)dr/app/ApplicationCache.hxx
$(INCDIR_)dr/app/def.hxx: base/def.hxx
	rm -f $(INCDIR_)dr/app/def.hxx; ln -sf ../../../$(DIRM_)base/def.hxx $(INCDIR_)dr/app/def.hxx
$(INCDIR_)dr/app/ApplicationCache.hxx: $(GEN)/ApplicationCache.hxx
	rm -f $(INCDIR_)dr/app/ApplicationCache.hxx; ln -sf ../../../$(DIRM_)$(GEN)/ApplicationCache.hxx $(INCDIR_)dr/app/ApplicationCache.hxx

ldistinc: DIST_INC_di

distinc:
	$(MAKE) ldistinc
	$(MAKE) -C t distinc

$(GEN):
	mkdir -p $(GEN)
$(OBJDIR):
	mkdir -p $(OBJDIR)
$(LIBDIR_)$(SHLIB_PREF)drapp$(SHLIB_EXT): $(OBJDIR)/ApplicationCache$(OBJ_EXT)
	cd $(TOPDIR) && $(LDXX_SO) -o lib/$(SHLIB_PREF)drapp$(SHLIB_EXT) -Llib $(DIRM_)$(OBJDIR)/ApplicationCache$(OBJ_EXT) -ldr $(FLAGS_shlib)
test:
	$(MAKE) ltest
	$(MAKE) -C t test

ltest:

slist:
	$(MAKE) lslist
	$(MAKE) -C t slist

lslist:
	echo $(DIRS_)base/mklist
	echo $(DIRS_)base/def.hxx
	echo $(DIRS_)except/mklist
	echo $(DIRS_)appcache/mklist
	echo $(DIRS_)appcache/ApplicationCache.cxx
	echo $(DIRS_)mklist
	echo $(DIRS_)Doxyfile

outclean:
	$(MAKE) loutclean
	$(MAKE) -C t outclean

loutclean:
	rm -f drapp

clean:
	$(MAKE) lclean
	$(MAKE) -C t clean

lclean: loutclean limmclean
limmclean:
	rm -f $(OBJDIR)/ApplicationCache$(OBJ_EXT) $(OBJDIR)/ApplicationCache$(OBJ_EXT).dep

wclean:
	$(MAKE) lwclean
	$(MAKE) -C t wclean

lwclean: lclean
	rm -rf $(BUILD)

sub_t: targ_local 
	$(MAKE) -C t


$(OBJDIR)/all_deps.mi:
	mkdir -p $(GEN)
	mkdir -p $(OBJDIR)
	touch $(OBJDIR)/dummy.dep
	echo "include $(OBJDIR)/*.dep" > $@
include $(OBJDIR)/all_deps.mi

gh:
	$(MAKE) lgh
	$(MAKE) -C t gh

include Make_gh

targ_pre:
targ_init: targ_pre ldistinc $(GEN) $(OBJDIR)
targ_local: targ_init $(LIBDIR_)$(SHLIB_PREF)drapp$(SHLIB_EXT)
targ_post: targ_local sub_t
targ_all: targ_pre targ_init targ_local targ_post

