#include "Something.h"  // As an example

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/time.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace Test;

int main(int argc, char **argv) {
  boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9091));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  std::string proxy_server_output;
  std::string buffer;
  buffer=argv[1];
  SomethingClient client(protocol);
  transport->open();
  client.ping();
  client.fetch_page(proxy_server_output, std::string(buffer));  
  transport->close();
  std::cout<<"proxy_server_output="<<proxy_server_output;
  // printf("%s\n",buffer);
  return 0;
}