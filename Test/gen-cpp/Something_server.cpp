#include "Something.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TTransportUtils.h>
#include <curl/curl.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using boost::shared_ptr;

using namespace::Test;

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;

static int writer(char *data, size_t size, size_t nmemb,
                  std::string *writerData)
{
  if (writerData == NULL)
    return 0;
 
  writerData->append(data, size*nmemb);
 
  return size * nmemb;
}


bool init(std::string url)
{
  CURL *conn;
  CURLcode code;
 
  conn = curl_easy_init();
 
  if (conn == NULL)
  {
    fprintf(stderr, "Failed to create CURL connection\n");
 
    exit(EXIT_FAILURE);
  }
 
  code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
  if (code != CURLE_OK)
  {
    fprintf(stderr, "Failed to set error buffer [%d]\n", code);
 
    return false;
  }
 
  code = curl_easy_setopt(conn, CURLOPT_URL, url.c_str());
  if (code != CURLE_OK)
  {
    fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
 
    return false;
  }
 
 
  code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
  if (code != CURLE_OK)
  {
    fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
 
    return false;
  }
 
  code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
  if (code != CURLE_OK)
  {
    fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
 
    return false;
  }
  code = curl_easy_perform(conn);
  if (code != CURLE_OK)
  {
    fprintf(stderr, "Failed to get '%s' [%s]\n", url.c_str(), errorBuffer);
 
    exit(EXIT_FAILURE);
  }  
  curl_easy_cleanup(conn);
  return true;
}




class SomethingHandler : virtual public SomethingIf {
 public:
  SomethingHandler() {
   // Your initialization goes here
  }

  int32_t ping() {
   // Your implementation goes here
    printf("ping\n");
  }

  void fetch_page(std::string& _return, const std::string& url) {
    // Your implementation goes here
    printf("fetch_page %s...\n",url.c_str());

    init(url);
    _return = buffer;
    buffer.clear();
  }



};

int main(int argc, char **argv) {
  int port = 9091;
  shared_ptr<SomethingHandler> handler(new SomethingHandler());
  shared_ptr<TProcessor> processor(new SomethingProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}