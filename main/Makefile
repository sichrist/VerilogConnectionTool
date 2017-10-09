include ../flow/Makefile.flow

.PHONY: ../bin/test ../bin/ict clean

CFLAGS=-Wall -pedantic -Werror -g -std=c++0x
PROJECT_CONST = -D PROJECTPATH='"$(PROJ_PATH)"'
IDIR = -I $(UTI_PATH) -I$ $(CON_PATH) -I $(MOD_PATH)

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
	$(CC) $(BOOST_DIR) $(IDIR) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(BOOST_LIB)

global: ../bin/test

../bin/ict: ict.cpp $(DEPENDENCIES)
	@echo $(DEPENDENCIES)
	$(CC) $(BOOST_DIR) $(IDIR) $(PROJECT_CONST)  $(CFLAGS) $(LDFLAGS) $^ -o $@ $(BOOST_LIB)

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
