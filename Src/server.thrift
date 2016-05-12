#!/usr/local/bin/thrift --gen cpp

service MyProxyServer{
  binary fetchURL(1:string url)
}
