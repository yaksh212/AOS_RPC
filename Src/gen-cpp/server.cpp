#include "ProxyServer.h"
#include "Cache.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/dir.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

// using namespace std ;

int c_policy;
int c_size;
Cache* s_cache;
int hit=0;
int total_req=0;



static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  std::string *_return = (std::string*)userp;
  _return->append((char*)contents, size*nmemb);

   return size * nmemb;
}
 

bool get_by_curl(const std::string& url, std::string& _return){
  CURL *curl_handle;
  CURLcode res;
  
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  
  if (curl_handle == NULL)
  {
    fprintf(stderr, "Failed to create CURL connection\n");
 
    exit(EXIT_FAILURE);
  }
  /* specify URL to get */ 
  res = curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
   /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return false;
  }
 
 
  /* send all data to this function  */ 
  res = curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
   /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_setopt failed: %s\n",
            curl_easy_strerror(res));
    return false;
  }
 
 
  /* we pass our 'chunk' struct to the callback function */ 
  res = curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &_return);
   /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_setopt() failed: %s\n",
            curl_easy_strerror(res));
    return false;
  }
 
 
  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */ 
  res = curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
   /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_setopt() failed: %s\n",
            curl_easy_strerror(res));
  }
 
 
  /* get it! */ 
  res = curl_easy_perform(curl_handle);
   /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return false;
  }
 

  // // follow redirection
  //   curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
  //    /* check for errors */ 
  // if(res != CURLE_OK) {
  //   fprintf(stderr, "curl_easy_perform() failed: %s\n",
  //           curl_easy_strerror(res));
  // }
 
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return false;
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();

  return true;
}



class ProxyServerHandler : virtual public ProxyServerIf {
  
 public:
  ProxyServerHandler() {
    // Your initialization goes here
    curl_global_init(CURL_GLOBAL_ALL);
    s_cache = new Cache(c_policy,c_size);


  }

   void fetchURL(std::string& _return, const std::string& url) {
     // Your implementation goes here
     total_req+=1;
     _return.clear();
     if(s_cache->contains(url))
     {
      hit+=1;
      _return = s_cache->getPage(url);
      printf("CHR=%d \n",(hit*100)/total_req);
      return;
     }
     else
     {
     	//printf("Not found in cache\n");
     	if(get_by_curl(url, _return))
     		s_cache->addPage(url,_return);


     }
   printf("CHR=%d \n",(hit*100)/total_req);   
  }
 

};

int main(int argc, char **argv) {
  int port = 9093;
  if(argc != 3){
  	printf("%s <policy> <size_in_kb>\n",argv[0]);
  	return -1;
  }

  c_policy = atoi(argv[1]);
  c_size = atoi(argv[2]);
  
 
  shared_ptr<ProxyServerHandler> handler(new ProxyServerHandler());
  shared_ptr<TProcessor> processor(new ProxyServerProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

