export make_basedir = ../

CC = g++
CXXFLAG = -Wall -Wno-unused-variable -ggdb -DBOOST_LOG_DYN_LINK

ARRU = ar -ru

INCLUDE := -I./ -I../ -I../common
obj_dbcommon2 := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
http_obj := $(patsubst %.cpp,%.o,$(wildcard http/*.cpp))
obj_dbcommon2 += $(http_obj)

LIB = -L/usr/local/lib/ -lboost_system -lboost_thread -lboost_log -lpthread -lboost_log_setup


OUTPUT = net.a

.SUFFIXES: .o .cpp .c

#DBCXXFLAG += -Ddb2app_debuging

.cpp.o:
	$(CC) $(CXXFLAG) $(INCLUDE) -Wno-deprecated -c -o $@ $< 

.c.o:
	$(CC) $(CXXFLAGS) -c -o $@ $< 

all: $(OUTPUT)


$(OUTPUT): $(obj_dbcommon2)
	$(ARRU) $@ $(obj_dbcommon2) 

install:
	cp $(OUTPUT) ../../lib/$(OUTPUT)

clean: 
	rm -f *.o
	rm -f http/*.o
	rm -f *.ro
	rm -f *.a
