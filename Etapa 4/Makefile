# Linux Makefile
# Socket class with SSL
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := -lssl -lcrypto
DEPS := Socket.h
TARGETS := Cliente.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o Socket.o
	$(info $$^ = $^)
	$(CXX) $^ Socket.o $(LDFLAGS) -o $^

Client.out: Client.o MethodsClient.o Socket.o
	$(CXX) Client.o MethodsClient.o Socket.o $(LDFLAGS) -o $@

IntermediateServer.out: IntermediateServer.o MethodsIntermediate.o Socket.o
	$(CXX) IntermediateServer.o MethodsIntermediate.o Socket.o $(LDFLAGS) -o $@

PiecesServer.out: PiecesServer.o MethodsPieces.o Socket.o
	$(CXX) PiecesServer.o MethodsPieces.o Socket.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out
