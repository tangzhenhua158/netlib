include ../../Makefile.inc
export make_basedir = ../

CC = g++
CXXFLAG = -Wall -Wno-unused-variable -ggdb -DBOOST_LOG_DYN_LINK

ARRU = ar -ru

INCLUDE := $(INCLUDE) -I./ -I../ -I../../ -I../../netlib/ -I../../netlib/net/ -I../../netlib/common/
obj_dbcommon2 := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
LIB = ../../lib/net.a ../../lib/common.a -L/usr/local/lib/ -lboost_system -lboost_thread -lboost_log -lpthread -lboost_log_setup

OUTPUT = echo_server

.SUFFIXES: .o .cpp .c

#DBCXXFLAG += -Ddb2app_debuging

.cpp.o:
	$(CC) $(CXXFLAG) $(INCLUDE) -Wno-deprecated -c -o $@ $< 

.c.o:
	$(CC) $(CXXFLAGS) -c -o $@ $< 

all: $(OUTPUT)

$(OUTPUT): $(obj_dbcommon2)
	$(CC) -o $@ $(obj_dbcommon2) $(LIB)

install:
	cp $(OUTPUT) ../../bin/$(OUTPUT)

clean: 
	rm -f *.o
	rm -f *.ro
	rm -f *.a
	rm -f $(OUTPUT)
