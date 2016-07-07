CC      = g++
CFLAGS  = -g -lboost_program_options --std=c++11 `root-config --cflags` -O2 -I./ 
LDFLAGS = `root-config --libs`  -lASImage -lTreePlayer

OBJ =  dumpEffPlot.o Sample.o CMS_lumi.o tdrstyle.o 

all: dumpEffPlot
dumpEffPlot: $(OBJ)
	$(CC) $(CFLAGS) -o dumpEffPlot $(OBJ) $(LDFLAGS)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -c $<
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

CMS_lumi.o: CMS_lumi.cpp  CMS_lumi.hpp
	$(CC) $(CFLAGS) -c $<

tdrstyle.o: tdrstyle.C  tdrstyle.hpp
	$(CC) $(CFLAGS) -c $<


clean:	
	rm *.o 