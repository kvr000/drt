dl_SOCXXFLAGS=
dl_SOLDXXFLAGS=-L$(HOME)/platf.`uname -s`-`uname -m`/lib -ldl

include $(DRTETCDIR_)p/sys.mk
