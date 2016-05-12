#ifndef CACHE_HPP
#define CACHE_HPP

#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <list>
#include <vector>




class Cache {
	public:
	 Cache(int, int);
	 bool contains(std::string);
	 std::string getPage(std::string);
	 void addPage(std::string, std::string);
	 void evict();
	 void evictQueue();
	 void evictLRU();
	private:
	 int policy;
	 int size;
	 int current_cache_size;
	 std::map<std::string, std::string> pageDataMap;
	 std::map<std::string, std::list<std::string>::iterator > nodeRef;
	 std::vector<std::string> urlList;
	 std::list<std::string> urlListLRU;
};

#endif