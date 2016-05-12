#!/usr/local/bin/thrift --gen cpp

namespace cpp Test

service Something {
  i32 ping()
  string fetch_page(1:string url);
}
