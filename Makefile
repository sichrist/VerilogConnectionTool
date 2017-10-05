
include flow/Makefile.flow
OBJDIR := obj
SRC := src/
LIB := lib/src/
SRCFILES := $(wildcard $(SRC)*.cpp)
LIBFILES := $(wildcard $(LIB)*.cpp)
OBJS_SRC  :=  $(sort $(patsubst %.cpp,$(OBJDIR)/%.o,$(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRCFILES)))))
OBJS_LIB  :=  $(sort $(patsubst %.cpp,$(OBJDIR)/%.o,$(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(LIBFILES)))))
directory := src/ lib/src/
TOPTARGETS := lib src main clean


PATHSVAR :=


$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) $(PRNTFLAGS)  -C $@ global

build:
ifneq ($(wildcard ${CURDIR}/cfg/.*),)
		@rm -rf ${CURDIR}/cfg/paths.cfg
else
		@mkdir cfg/
endif
		@echo > ${CURDIR}/cfg/paths.cfg
		@echo "## path to the file which contains the hierarchy" > ${CURDIR}/cfg/paths.cfg
		@echo "[MODTXT]: ${CURDIR}/vpi/mod.txt" >> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "## Directory for backupfiles" >> ${CURDIR}/cfg/paths.cfg
		@echo "[FILE]: ${CURDIR}/.testfiles/" >> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "## input file" >> ${CURDIR}/cfg/paths.cfg
		@echo "[INPUT]:" >> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "## Files in this path throw a warning message" >> ${CURDIR}/cfg/paths.cfg
		@echo "[WARNING]" >> ${CURDIR}/cfg/paths.cfg
		@echo "{">> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "}">> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "## Paths/files which won't be modified" >> ${CURDIR}/cfg/paths.cfg
		@echo "[ILLEGAL]" >> ${CURDIR}/cfg/paths.cfg
		@echo "{">> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg
		@echo "}">> ${CURDIR}/cfg/paths.cfg
		@echo >> ${CURDIR}/cfg/paths.cfg

ifneq ($(wildcard ${CURDIR}/.testfiles),)

else
	@mkdir .testfiles
endif

ifneq ($(wildcard ${CURDIR}/obj),)

else
	@mkdir obj
endif

	@$(MAKE) $(PRNTFLAGS)	-C src/	src
	@$(MAKE) $(PRNTFLAGS)	-C lib/ lib
	@$(MAKE) $(PRNTFLAGS) 	-C main/ main

remove:
	@rm -rf ${CURDIR}/cfg
	@rm -rf ${CURDIR}/.testfiles
	@rm -rf ${CURDIR}/obj


.PHONY: $(TOPTARGETS) $(SUBDIRS)


