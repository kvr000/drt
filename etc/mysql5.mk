mysql5_SOCXXFLAGS=-I/usr/local/include
mysql5_SOLDXXFLAGS=-L/usr/local/lib -lmysqlclient

mysql5_EXECXXFLAGS=$(CXXFLAGS)
mysql5_EXELDXXFLAGS=$(CXXFLAGS) $(T_LDXXFLAGS)

include $(ETCDIR_)p/mysql5.mk
