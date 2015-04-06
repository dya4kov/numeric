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
	     ODE/steppers/ODEstepperPD5.h \
	     ODE/steppers/ODEstepperPD853.h

_SFDEPS=mathUtils.h \
	    types.h \
	    FermiDirac.h

_LADEPS=
	           
ODEDEPS=$(patsubst %,$(IDIR)/%,$(_ODEDEPS))
SFDEPS=$(patsubst %,$(IDIR)/%,$(_SFDEPS))
LADEPS=$(patsubst %,$(IDIR)/%,$(_LADEPS))

_ODEOBJ=ODEtest.o
_SFOBJ=SFtest.o
_LAOBJ=LAtest.o

ODEOBJ=$(patsubst %,$(ODIR)/%,$(_ODEOBJ))
SFOBJ=$(patsubst %,$(ODIR)/%,$(_SFOBJ))
LAOBJ=$(patsubst %,$(ODIR)/%,$(_LAOBJ))

all: ODEtest SFtest LAtest

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

LAtest: makeobjdir makebindir $(LAOBJ) 
	$(CC) -o $(BINDIR)/$@ $(LAOBJ) $(LIBS)

LAtest: DEPS=$(LADEPS)

.PHONY: clean

clean:
	rm -rf $(ODIR) *~ $(IDIR)/*~ $(SDIR)/*~ $(BINDIR)
