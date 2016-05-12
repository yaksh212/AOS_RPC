Contributors: Yakshdeep Kaul, Satyajeet Gawas

How to run

1. Run make all​to compile the server and client </br>

2. Server executable ./server <'cache_type> <'cache_size></br>

<'cache_type> : 0 for Random eviction, 1 for FIFO replacement policy, 2 for LRU replacement policy,

<'cache_size> : 0 for no caching policy and size in KB</br>

3. Client executable: ./client <'mode> <'server­ip> <'workload / url to fetch>

<'mode> : 0 for fetching only one url , 1 for testing caching for different work loads

<'workload / url to fetch> : (when mode 1)0 for uniform random distribution of 100 urls generated from 20 unique urls, 1 for exponential distribution of 100 urls generated from 20 unique urls

(When mode 0) url to fetch : the url to fetch should have start with www

ex: “./client 0 localhost www.google.com”

ex: “./client 1 localhost 0”

ex: “./client 1 localhost 1”
