

include ../flow/Makefile.flow

SRC = $(wildcard src/*.cpp)
SRC_= $(SRC:src/%.cpp=%.o )
HDR = $(wildcard inc/*.h)
OBJ_PATH = $(addprefix ../obj/,$(SRC_) )
CFLAGS=-Wall -pedantic -Werror -g -std=c++0x -I../lib/inc $(BOOST_DIR)
OBJECTS = ../obj/parser.o ../obj/Filehandler.o ../obj/Exec.o ../obj/thread.o


lib: $(OBJ_PATH)

../obj/parser.o:src/parser.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Filehandler.o:src/Filehandler.cpp
	$(CC) -c $(CFLAGS) -o $@ $< 

../obj/Exec.o:src/Exec.cpp
	$(CC) -c $(CFLAGS) -o $@ $<	

../obj/csvFile.o:src/csvFile.cpp
	$(CC) -c $(CFLAGS) -o $@ $<	

../obj/IOcon.o:src/IOcon.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/verilogFile.o:src/verilogFile.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/thread.o:src/thread.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Cutter.o:src/Cutter.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

global: lib

indent:
	@bash -c "indent $(INDENT_OPTIONS) $(SRC) $(HDR)"

clean:
	rm -f $(OBJ_PATH)
	
clean_indent.bak:
	@rm src/*.cpp~ inc/*.h~

.PHONY: clean $(OBJ_PATH)
