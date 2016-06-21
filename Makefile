CC = g++
CFLAGS = -Wall -c -g 

INCDIR   = interface
SRCDIR   = src
OBJDIR   = lib
BINDIR   = bin
TESTDIR  = test


SOURCES  := $(wildcard $(SRCDIR)/*.cc)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)

INCLUDES = -I. -I$(ROOTSYS)/include  -I./interface 

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -Wl,--no-as-needed $(ROOTGLIBS) -lHtml -lMathCore -lGenVector -lMinuit -lTMVA -lRooFit `xml2-config --cflags` `xml2-config --libs`


CPP =  $(wildcard test/*.cpp)
CPPOBJ = $(patsubst test/%,%,$(CPP))
BINOBJ	=$(patsubst %.cpp,bin/%,$(CPPOBJ))


all: $(BINOBJ) 

$(BINDIR)/% : test/%.cpp $(OBJECTS)  
	@echo "Compiling $@"
	@echo $(CC) $(INCLUDES) -o $@ $<  $(ROOTFLAG) $(EXTRALIBS) $(OBJECTS) 
	@$(CC) $(INCLUDES) -o $@ $<  $(ROOTFLAG) $(EXTRALIBS) $(OBJECTS) 

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc  $(INCDIR)/%.h  
	@echo "Compiling "$<" "
	@echo $(CC) $(CFLAGS) $(INCLUDES) -o $@  -c $< $(ROOTFLAG) $(EXTRALIBS) 
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@  -c $< $(ROOTFLAG) $(EXTRALIBS)	



clean:
	rm bin/*.o;rm lib/*.o
