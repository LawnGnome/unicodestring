CXX = `icu-config --cxx`
CXXFLAGS = `icu-config --cxxflags` -g -Wall -pedantic -DDEBUG
LDFLAGS = `icu-config --ldflags` -g -Wall

all:	ustring.o exceptions.o

clean:	
	rm -f *.o test/*.o test/basic

test:	test/basic

test/basic:	test/basic.o test/test.o test/runner.o ustring.o exceptions.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o:	%.cpp
	$(CXX) $(CXXFLAGS) -I. -o $@ -c $<
