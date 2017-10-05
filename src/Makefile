include ../flow/Makefile.flow

IDIR = ../header -I../lib/inc $(BOOST_DIR)
SOURCES = $(wildcard *.cpp)
HDR = $(wildcard ../header/*.h)
OBJECTS1 = $(SOURCES:.cpp=.o)
OBJECTS = ../obj/inpH.o ../obj/Instance.o  ../obj/Message.o ../obj/Modhandler.o ../obj/module.o ../obj/Controller.o
BINDIR = $(addprefix ../obj/,$(OBJECTS1))

src : $(BINDIR)
#	@bash -c "indent $(INDENT_OPTIONS) $(SOURCES) $(HDR)"
#	@rm *.cpp~ ../header/*.h~


Path_: ../obj/Path.o

../obj/module.o: module.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Instance.o: Instance.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Modhandler.o: Modhandler.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/inpH.o: inpH.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Message.o: Message.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Controller.o: Controller.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Path.o: Path.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/TestController.o: TestController.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

../obj/Connection.o: Connection.cpp
	$(CC) -c $(CFLAGS) -o $@ $< 

../obj/modifyUnit.o: modifyUnit.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

global: src

clean:
	rm -f $(OBJECTS)
clean_indent.bak:
	rm -f *.cpp~ ../header/*.cpp~
.PHONY: clean $(OBJECTS) src
