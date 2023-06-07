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

SSLServer.out: SSLServer.o Socket.o
	$(CXX) SSLServer.o Socket.o $(LDFLAGS) -o $@

SSLServer.out: NachosServer.o Socket.o
	$(CXX) NachosServer.o Socket.o $(LDFLAGS) -o $@

SSLClient.out: SSLClient.o HttpClient.o Socket.o
	$(CXX) SSLClient.o HttpClient.o Socket.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out
