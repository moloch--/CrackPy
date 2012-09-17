# Python version
PYTHON = /usr/include/python2.7

# Boost libs
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# Build
TARGET = CrackPy

OBJS = Md5.o HashFactory.o CrackingEngine.o CrackPy.o
OPTIMIZATION = -O3
DEBUG = -g
CFLAGS = -Wall -ansi $(DEBUG) $(OPTIMIZATION) -c -fPIC
LFLAGS = -Wall -ansi -filt $(OPTIMIZATION)
LIBS = -L/lib -L/usr/lib -lssl -lboost_thread

all: $(OBJS) $(TARGET).so

clean:
	rm -f *.o $(TARGET).so

Md5.o: HashAlgorithm.h Md5.h Md5.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Md5.cpp -lssl

HashFactory.o: HashFactory.h HashFactory.cpp HashAlgorithm.h 
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c HashFactory.cpp

CrackingEngine.o: CrackingEngine.h CrackingEngine.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackingEngine.cpp -lboost_thread

CrackPy.o: CrackPy.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackPy.cpp

CrackPy.so: CrackPy.o
	g++ $(LFLAGS) $(OBJS) $(LIBS) -shared -Wl,--export-dynamic \
	-L$(BOOST_LIB) -lboost_python \
	-L/usr/lib/python2.7/config -lpython2.7 \
	-o $(TARGET).so