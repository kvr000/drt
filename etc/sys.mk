dl_SOCXXFLAGS=
dl_SOLDXXFLAGS=-L$(HOME)/platf.`uname -s`-`uname -m`/lib -ldl

include $(ETCDIR_)p/sys.mk
