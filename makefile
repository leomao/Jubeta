###############################################################
# Makefile for Jubeta
# Created by LeoMao
###############################################################

PROGRAM = Jubeta

LIBPATH = ./
OBJPATH = ./obj/
SRCPATH = ./src/
BINPATH = ./bin/

COMPILER= $(shell wx-config --cxx)
LINKER  = $(shell wx-config --cxx)

SHARED := 0

DEFINES = -g -O2
CXXFLAG =
WXCXXFLAG = `wx-config --static=yes --cxxflags`
WXSHAREDCXXFLAG = `wx-config --static=no --cxxflags`
ifeq ($(SHARED), 1)
FLAGS = $(DEFINES) $(WXSHAREDCXXFLAG) $(CXXFLAG)
else
FLAGS = $(DEFINES) $(WXCXXFLAG) $(CXXFLAG)
endif

OSLIB   = -lgstreamer-0.10 -lgstinterfaces-0.10
WXLIB   = `wx-config --static=yes --libs std,media`
WXSHAREDLIB   = `wx-config --static=no --libs std,media`
ifeq ($(SHARED), 1)
LIBS = $(OSLIB) $(WXSHAREDLIB)
else
LIBS = $(OSLIB) $(WXLIB)
endif

SOURCES = $(wildcard $(SRCPATH)*.cpp)

OBJS := $(addprefix $(OBJPATH),$(patsubst $(SRCPATH)%.cpp,%.o,$(SOURCES)))

EXES    = $(BINPATH)$(PROGRAM)

VER = $(shell cat version.txt)

BINEXT =

.PHONY : all clean veryclean rebuild

all: version $(EXES)

$(PROGRAM): $(BINPATH)$(PROGRAM)

$(BINPATH)$(PROGRAM): $(OBJS)
	$(LINKER) $(OBJS) \
	$(LIBS) -o $@_$(VER)
	strip $@_$(VER)$(BINEXT)

$(OBJPATH)%.o : $(SRCPATH)%.cpp $(SRCPATH)*.h
	$(COMPILER) $(FLAGS) -c $< -o $@

version:
	echo '#ifndef VERSIONNO_H' > src/version_no.h
	echo '#define VERSIONNO_H' >> src/version_no.h
	echo '#include <string>' >> src/version_no.h
	echo 'const string VERSIONNO = "$(VER)";' >> src/version_no.h
	echo '#endif' >> src/version_no.h

clean:
	rm -rf $(OBJPATH)*.o

veryclean: clean
	rm $(EXES)

rebuild: veryclean all

##############################################################
