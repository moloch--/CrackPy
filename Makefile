# Python version
PYTHON = /usr/include/python2.7

# Boost libs
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# Build
TARGET = CrackPy

# Compiler
# CC = g++
CC = g++-4.6

OBJS = Md5.o Md4.o Sha1.o Sha256.o HashFactory.o CrackingEngine.o CrackPy.o
OPTIMIZATION = -O3
#DEBUG = -g -rdynamic
CFLAGS = -Wall -ansi $(DEBUG) $(OPTIMIZATION) -c -fPIC
LFLAGS = -Wall -filt -ansi $(OPTIMIZATION)
LIBS = -L/lib -L/usr/lib -lssl -lcrypto -lboost_thread -lboost_python -lpython2.7

all: $(OBJS) $(TARGET).so

clean:
	rm -f *.o 
# rm $(TARGET).so

Md4.o: HashAlgorithm.h Md4.h Md4.cpp
	$(CC) $(CFLAGS) -I$(BOOST_INC) -c Md4.cpp $(LIBS)

Md5.o: HashAlgorithm.h Md5.h Md5.cpp
	$(CC) $(CFLAGS) -I$(BOOST_INC) -c Md5.cpp $(LIBS)	

Sha1.o: HashAlgorithm.h Sha1.h Sha1.cpp
	$(CC) $(CFLAGS) -I$(BOOST_INC) -c Sha1.cpp $(LIBS)

Sha256.o: HashAlgorithm.h Sha256.h Sha256.cpp
	$(CC) $(CFLAGS) -I$(BOOST_INC) -c Sha256.cpp $(LIBS)

HashFactory.o: HashFactory.h HashFactory.cpp HashAlgorithm.h 
	$(CC) $(CFLAGS) -I$(BOOST_INC) -c HashFactory.cpp

CrackingEngine.o: CrackingEngine.h CrackingEngine.cpp
	$(CC) $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackingEngine.cpp $(LIBS)

CrackPy.o: CrackPy.cpp
	$(CC) $(CFLAGS) -I$(PYTHON) -I$(BOOST_INC) -c CrackPy.cpp $(LIBS)

CrackPy.so: CrackPy.o
	$(CC) $(LFLAGS) $(OBJS) \
	-shared -o $(TARGET).so \
	-Wl,--no-undefined      \
	-L$(BOOST_LIB) $(LIBS)  \
	-L/usr/lib/python2.7/config \
