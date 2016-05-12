GEN_SRC := Something.cpp your_thrift_file_constants.cpp your_thrift_file_types.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))
CURL_LIB = -lcurl
THRIFT_DIR := /usr/local/include/thrift
BOOST_DIR := /usr/local/include

INC := -I$(THRIFT_DIR) -I$(BOOST_DIR)

.PHONY: all clean

all: Something_server Something_client

%.o: %.cpp
	$(CXX) -Wall -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H $(INC) -c $< -o $@

Something_server: Something_server.o $(GEN_OBJ)
	$(CXX) $^ -o $@ -L/usr/local/lib -lthrift $(CURL_LIB)

Something_client: Something_client.o $(GEN_OBJ)
	$(CXX) $^ -o $@ -L/usr/local/lib -lthrift 

clean:
	$(RM) *.o Something_server Something_client