export make_basedir = ../

CC = g++
CXXFLAG = -Wall -Wno-unused-variable -ggdb -DBOOST_LOG_DYN_LINK

ARRU = ar -ru

INCLUDE := -I./
src_dbcommon2	:= Log.cpp Thread.cpp

obj_dbcommon2	:= $(src_dbcommon2:.cpp=.o)
obj_dbcommon2	:= $(obj_dbcommon2:.c=.o)

OUTPUT = common.a

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
	rm -f *.ro
	rm -f *.a
