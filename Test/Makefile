include ../flow/Makefile.flow

INCLUDE = -I$(UTI_PATH)

IOcon: IOcon_Test.cpp $(OBJ_PATH)IOcon.o $(OBJ_PATH)Cutter.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $^ -o $@
