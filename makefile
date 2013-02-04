###############################################################
# Makefile Model for wxWidgets application ver 1.0
# Created by Taii on 20060520
###############################################################

PROGRAM = Jubeta

LIBPATH = ./
OBJPATH = ./obj/
SRCPATH = ./src/
BINPATH = ./bin/

COMPILER= $(shell wx-config --cxx)
LINKER  = $(shell wx-config --cxx)

SHARED  := 1

DEFINES = -g -O2
CXXFLAG =
WXCXXFLAG = `wx-config --cxxflags`
WXSHAREDCXXFLAG = `wx-config --static=no --cxxflags`
ifeq ($(SHARED), 1)
FLAGS = $(DEFINES) $(WXSHAREDCXXFLAG) $(CXXFLAG)
else
FLAGS = $(DEFINES) $(WXCXXFLAG) $(CXXFLAG)
endif

OSLIB   = 
WXLIB   = `wx-config --libs std,media`
WXSHAREDLIB   = `wx-config --static=no --libs std,media`
ifeq ($(SHARED), 1)
LIBS = $(OSLIB) $(WXSHAREDLIB)
else
LIBS = $(OSLIB) $(WXLIB)
endif

SOURCES = $(wildcard $(SRCPATH)*.cpp)

OBJS := $(addprefix $(OBJPATH),$(patsubst $(SRCPATH)%.cpp,%.o,$(SOURCES)))

EXES    = $(BINPATH)$(PROGRAM)

VERNO = $(VER).$(VERREV)

VER = $(shell cat version.txt)

VERREV = $(shell git log --pretty=format:'%h' -n 1)

NEWVER ?= $(VER)

BINEXT =

.PHONY : all clean veryclean rebuild

all: version $(EXES)

$(PROGRAM): $(BINPATH)$(PROGRAM)

$(BINPATH)$(PROGRAM): $(OBJS)
	$(LINKER) $(OBJS) \
	$(LIBS) -o $@_$(VERNO)
	strip $@_$(VERNO)$(BINEXT)

$(OBJPATH)%.o : $(SRCPATH)%.cpp $(SRCPATH)*.h
	$(COMPILER) $(FLAGS) -c $< -o $@

verup:
ifneq ($(NEWVER),$(VER))
	echo $(NEWVER)>version.txt
	git commit -m 'Ver_$(NEWVER)'
	git tag $(NEWVER)
endif

version:
ifneq ($(VERREV),$(shell cat rev.txt))
	echo \#ifndef VERSIONNO_H > src/version_no.h
	echo \#define VERSIONNO_H >> src/version_no.h
	echo \#include \<string\> >> src/version_no.h
	echo const string VERSIONNO = \"$(VER)-$(VERREV)\"\; >> src/version_no.h
	echo \#endif >> src/version_no.h
	echo $(VERREV) > rev.txt
endif

clean:
	rm -rf $(OBJPATH)*.o

veryclean: clean
	rm $(EXES)

rebuild: veryclean all

##############################################################
