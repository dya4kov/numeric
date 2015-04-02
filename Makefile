IDIR=./hdr
CC=g++
CFLAGS=-I$(IDIR)
ODIR=./obj
BINDIR=./bin
SDIR=./src
LIBS=-lm

_ODEDEPS=mathUtils.h \
	     numericTypes.h \
	     ODE/ODEdata.h \
	     ODE/ODEsolver.h \
	     ODE/ODEstepperBase.h \
	     ODE/steppers/ODEstepperPD5.h

_SFDEPS=mathUtils.h \
	    types.h \
	    FermiDirac.h
	           
ODEDEPS=$(patsubst %,$(IDIR)/%,$(_ODEDEPS))
SFDEPS=$(patsubst %,$(IDIR)/%,$(_SFDEPS))

_ODEOBJ=ODEtest.o
_SFOBJ=SFtest.o

ODEOBJ=$(patsubst %,$(ODIR)/%,$(_ODEOBJ))
SFOBJ=$(patsubst %,$(ODIR)/%,$(_SFOBJ))

all: ODEtest SFtest

makeobjdir:
	mkdir -p $(ODIR) 

makebindir:
	mkdir -p $(BINDIR) 

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) 
	$(CC) -c -o $@ $< -I$(IDIR)

ODEtest: makeobjdir makebindir $(ODEOBJ) 
	$(CC) -o $(BINDIR)/$@ $(ODEOBJ) $(LIBS)

ODEtest: DEPS=$(ODEDEPS)

SFtest: makeobjdir makebindir $(SFOBJ) 
	$(CC) -o $(BINDIR)/$@ $(SFOBJ) $(LIBS)

SFtest: DEPS=$(SFDEPS)

.PHONY: clean

clean:
	rm -rf $(ODIR) *~ $(IDIR)/*~ $(SDIR)/*~ $(BINDIR)
