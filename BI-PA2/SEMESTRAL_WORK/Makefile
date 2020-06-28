PROGRAM = urbanj51
CXX = g++
CFLAGS = -std=c++14 -Wall -pedantic -g
LDFLAGS = -lstdc++fs

SRCDIR = src/
SRC = $(shell find $(SRCDIR) -type f -name '*.cpp')

all: compile doc

compile: $(SRC: .cpp=.o)
	$(CXX) $(CFLAGS) $(SRC: .cpp=.o) -o $(PROGRAM) $(LDFLAGS) 

run:
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)

doc:
	doxygen

