COMPRESS=bzip2 -9
COMPSUFF=.bz2

all:
	@echo Specify target first >&2
	@false

include drtetc/p/root.mk
drtetc/p/root.mk:
	touch drtetc/p/root.mk

makes:
	make -C src gmake

# sources list
slist:
	@echo INSTALL
	@echo Makefile
	@echo senv.sh
	@gen_rls drtetc | grep -v drtetc/p/
	@gen_rls def | grep -v def/p
	@gen_rls util | grep -v util/p_
	@echo include
	@echo bin
	@echo lib
	@DR_NODEP=true $(MAKE) -C src -s slist | sed 's:^:src/:'

# p files list
plist:
	@find drtetc/p ! -type d ! -size 0 -print
	@find util -name p_\* -print


# dependencies
dep:
	$(MAKE) -C src depends

# other (non project and non p) files list
olist:
	@$(MAKE) -s slist plist | sed 's/^/^/' | sed 's/$$/$$/' | sed 's/\./\\./g' >slist.tmp
	@find . -type f -print | sed 's/^\.\///' | grep -v -f slist.tmp

# clean binary targets
clean:
	$(MAKE) -C src wclean
	rm -rf include/*
	rm -rf bin/*
	rm -rf lib/*

# clean all other files (except .depends)
dclean: clean
	$(MAKE) -s slist plist | sed 's/^/^/' | sed 's/$$/$$/' | sed 's/\./\\./g' >slist.tmp
	find . -name .depends -exec $(SHELL) -c 'echo >{}' \;
	$(MAKE) -s olist | grep -v '/\.depends$$' | xargs rm

aclean: clean
	$(MAKE) -s slist plist | sed 's/^/^/' | sed 's/$$/$$/' | sed 's/\./\\./g' >slist.tmp
	$(MAKE) -s olist | xargs rm

# diff to other version
diff:
	@set -e; \
	[ -n "$$DR_DIFF" ] || read -p 'version to diff (or use environment DR_DIFF): ' DR_DIFF || exit; \
	bdir=`pwd`; bdir=`basename "$$bdir"`; \
	odir=$$bdir; r=$$DR_DIFF; \
	if echo $$r | grep '^-' >/dev/null; then \
		odir=`echo $$odir | sed 's/-[^.]*$$//'`; \
	else \
		echo $$r | grep '[.]$$' >/dev/null || r="$$r."; \
		r=`echo $$r | sed 's:[^.]*[.]:/:g'`; \
		odir=$$odir.$$r; \
		while [ "$$odir" != "$$r" ]; do \
			r="$$odir"; \
			odir=`echo $$odir | sed 's:[^\.]*\./::'`; \
		done; \
	fi; \
	odir="$$odir$$DR_DIFF"; \
	patchfile=patch$$odir`echo $$bdir | sed 's/^[^-]*-*/,/'`; \
	( cd .. && \
		( $(MAKE) -C $$odir -s slist | sed "s:^:$$odir/:"; $(MAKE) -C $$bdir -s slist | sed "s:^:$$bdir/:" ) | \
		filter_diff $$odir $$bdir | \
		$(COMPRESS) > $$patchfile$(COMPSUFF) \
	) && \
	ls -l ../$$patchfile$(COMPSUFF)

# tar
tarout:
	@bdir=`pwd`; bdir=`basename "$$bdir"`/; \
	$(MAKE) -s slist | \
	grep -v '^drtetc/.*/' | \
	sed "s:^:$$bdir:" | \
	( cd ..; exec tar fcv - --exclude $${bdir}drtetc/p/\* --exclude $${bdir}include/\* --exclude $${bdir}lib/\* --exclude $${bdir}bin/\* --exclude .svn --exclude \*.orig --exclude \*.rej -T - )

tar:
	@MAKEFLAGS=`echo $$MAKEFLAGS | sed 's/w//g'`; \
	bdir=`pwd`; bdir=`basename "$$bdir"`/; \
	tarfile=`echo $$bdir | sed s:/::`.tar; \
	$(MAKE) -s tarout | $(COMPRESS) > ../$$tarfile$(COMPSUFF) && \
	ls -l ../$$tarfile$(COMPSUFF)

# tar of p files
ptar:
	@$(MAKE) -s plist | tar fcv - -T - | $(COMPRESS) >../p.tar$(COMPSUFF)

# build project
build:
	$(MAKE) -C src
