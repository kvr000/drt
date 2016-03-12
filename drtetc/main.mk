include $(DRTETCDIR_)$(DR_TARG)_defs.mk

$(DRTETCDIR_)p/%.mk:
	mkdir -p `dirname $@`
	touch $@
