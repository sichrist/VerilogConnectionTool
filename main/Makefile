include ../flow/Makefile.flow

TMP = $(wildcard ../obj/*.o)
DEPS = $(TMP:../obj/%=%)
VPATH = ../obj
CFLAGS=-Wall -pedantic -Werror -g -I../lib/inc -I../header -std=c++0x
.PHONY: ../bin/test ../bin/ict clean
PROJECT_CONST = -D PROJECTPATH='"$(NEW_PATH)"'


comp: build main
	#@bash -c "indent $(INDENT_OPTIONS) test.cpp"
#	@rm *.cpp~

main:  ../bin/ict

example: example.cpp $(DEPENDENCIES)
	$(CC) $(BOOST_DIR) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(BOOST_LIB)

build:
	@$(MAKE) $(PRNTFLAGS)  -C ../src
	@$(MAKE) $(PRNTFLAGS) -C ../lib

../bin/test: test.cpp $(DEPENDENCIES)
	$(CC) $(BOOST_DIR) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(BOOST_LIB)

global: ../bin/test

../bin/ict: ict.cpp $(DEPENDENCIES)
	$(CC) $(BOOST_DIR) $(PROJECT_CONST)  $(CFLAGS) $(LDFLAGS) $^ -o $@ $(BOOST_LIB)

# TestTargets

test: test.cpp ../obj/Cutter.o ../obj/Path.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

Cutter:
	@$(MAKE) $(PRNTFLAGS)  -C ../lib ../obj/Cutter.o

Path:
	@$(MAKE) $(PRNTFLAGS)  -C ../src ../obj/Path.o

clean:
	@$(MAKE) $(PRNTFLAGS) -C ../src clean
	@$(MAKE) $(PRNTFLAGS) -C ../lib clean
	rm -f ../bin/test ../bin/tmp.*

clean_indent.bak:
	rm -f *.cpp~ 
