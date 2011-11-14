CXX = `icu-config --cxx`
CXXFLAGS = `icu-config --cxxflags --cxxflags-dynamic` -g -Wall -pedantic -DDEBUG
LDFLAGS = `icu-config --ldflags --ldflags-icuio` -g -Wall

all:	ustring.o

clean:	
	rm -f *.o test/*.o test/basic

test:	test/basic

test/basic:	test/basic.o test/test.o test/runner.o ustring.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -I. -o $@ -c $<
