# Python version
PYTHON = /usr/include/python2.7

# Boost libs
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# Build
TARGET = CrackPy

OBJS = Md5.o CrackingEngine.o CrackPy.o
OPTIMIZATION = -O3
CFLAGS = -Wall -ansi $(INCLUDES) $(OPTIMIZATION) -c $(DEBUG) -fPIC -Wunused-result
LFLAGS = -Wall -ansi -filt $(INCLUDES) $(OPTIMIZATION)
LIBS = -L/lib -L/usr/lib -lssl -lboost_thread

all: $(OBJS) $(TARGET).so

clean:
	rm -f *.o $(TARGET).so

Md5.o: Md5.h Md5.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Md5.cpp -lssl

CrackingEngine.o: CrackingEngine.h CrackingEngine.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackingEngine.cpp -lboost_thread

CrackPy.o: CrackPy.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackPy.cpp

CrackPy.so: CrackPy.o
	g++ $(LFLAGS) $(OBJS) $(LIBS) -shared -Wl,--export-dynamic \
	-L$(BOOST_LIB) -lboost_python \
	-L/usr/lib/python2.7/config -lpython2.7 \
	-o $(TARGET).so