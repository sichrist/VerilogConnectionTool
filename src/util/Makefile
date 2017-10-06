include ../../flow/Makefile.flow
LIB_H 	:= inc/
IDIR 	:= -I$(LIB_H)
SRC 	:= $(wildcard src/*.cpp)
BNC_SRC	:= $(SRC:src/%.cpp=%)
.PHONY: $(PROJ_PATH)obj/Path.o

build: $(PROJ_PATH)obj/$(BNC_SRC).o

$(PROJ_PATH)obj/%.o : src/%.cpp
	@mkdir -p $(dir $@)
	@echo "============="
	@echo "Compiling $<"
	$(CC) -c $(CFLAGS) $(IDIR) $@ $<
