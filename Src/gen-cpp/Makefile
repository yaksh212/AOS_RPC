CXXFLAGS = -std=c++11
GEN_SRC := ProxyServer.cpp Cache.cpp server_constants.cpp server_types.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))

#CXX = g++ -std=c++11

CURL_LIBS = $(shell curl-config --libs)
CURL_CFLAGS = $(shell curl-config --cflags)

THRIFT_DIR := /usr/local/include/thrift
BOOST_DIR := /usr/local/include

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR)

.PHONY: all clean

all: server client

%.o: %.cpp
	$(CXX)  -std=c++11 $(CURL_CFLAGS) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H $(INC) -c $< -o $@

server: server.o $(GEN_OBJ)
	$(CXX) -g $^ -o $@ -L/usr/local/lib -lthrift $(CURL_LIBS)

client: client.o $(GEN_OBJ)
	$(CXX) -std=c++11 $^ -o $@ -L/usr/local/lib -lthrift 

clean:
	$(RM) *.o server client