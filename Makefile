# Python version
PYTHON = /usr/include/python2.7

# Boost libs
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# Build
TARGET = CrackPy

OBJS = Md5.o Md4.o Sha1.o Sha256.o HashFactory.o CrackingEngine.o CrackPy.o
OPTIMIZATION = -O3
#DEBUG = -g -rdynamic
CFLAGS = -Wall -ansi $(DEBUG) $(OPTIMIZATION) -c -fPIC
LFLAGS = -Wall -ansi -filt $(OPTIMIZATION)
LIBS = -L/lib -L/usr/lib -lssl -lboost_thread

all: $(OBJS) $(TARGET).so

clean:
	rm -f *.o $(TARGET).so

Md5.o: HashAlgorithm.h Md5.h Md5.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Md5.cpp -lssl

Md4.o: HashAlgorithm.h Md4.h Md4.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Md4.cpp -lssl
	
Sha1.o: HashAlgorithm.h Sha1.h Sha1.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Sha1.cpp -lssl

	
Sha256.o: HashAlgorithm.h Sha256.h Sha256.cpp
	g++ $(CFLAGS) -I$(BOOST_INC) -c Sha256.cpp -lssl

HashFactory.o: HashFactory.h HashFactory.cpp HashAlgorithm.h 
	g++ $(CFLAGS) -I$(BOOST_INC) -c HashFactory.cpp

CrackingEngine.o: CrackingEngine.h CrackingEngine.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackingEngine.cpp -lboost_thread

CrackPy.o: CrackPy.cpp
	g++ $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackPy.cpp

CrackPy.so: CrackPy.o
	g++ $(LFLAGS) $(OBJS) $(LIBS) -shared -Wl,--export-dynamic \
	-L$(BOOST_LIB) -lboost_python \
	-L/usr/lib/python2.7/config -lpython2.7 \
	-o $(TARGET).so