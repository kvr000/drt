libxml2_SOCXXFLAGS=-I/usr/include/libxml2
libxml2_SOLDXXFLAGS=-lxml2

libxml2_EXECXXFLAGS=$(libxml2_SOCXXFLAGS)
libxml2_EXELDXXFLAGS=$(libxml2_SOLDXXFLAGS)

include $(DRTETCDIR_)p/libxml2.mk
