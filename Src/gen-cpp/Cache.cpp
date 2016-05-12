#include "Cache.h"
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <algorithm>
#include <list>
#include <vector>


Cache::Cache(int c_policy,int c_size){
	this->policy = c_policy;
	this->size = c_size * 1024;
	this->current_cache_size = 0;
}


void Cache::addPage(std::string url, std::string pageData){
	//get current size of the cache, if adding pageData to the cache exceeds the cache limit then evict pages
	if( pageData.length()*sizeof(char) > this->size || this->size == 0)
		return;
	int size = this->current_cache_size + pageData.length()*sizeof(char);
	while( size > this->size){
		if(this->policy == 0){
			evict();
		}
		else if(this->policy == 1){
			evictQueue();
		}
		else if(this->policy == 2){
			evictLRU();
		}
		size = this->current_cache_size + pageData.length()*sizeof(char);
	}
	
	//update the current cache size
	this->current_cache_size += pageData.length()*sizeof(char);
	
	//update the cache management data structures depending upon the policy
	if(this->policy == 0 || this->policy == 1)
		this->urlList.push_back(url);
	else
	{
		this->urlListLRU.push_front(url);
		// this->nodeRef.insert(std::pair<std::string,std::list<std::string>::iterator> (url,this->urlListLRU.begin()));
		this->nodeRef[url] = this->urlListLRU.begin();
	}
	
	//insert data into a hash table
	this->pageDataMap.insert (std::pair<std::string,std::string>(url,pageData) );
}


/*
 Function to check if cache contains page data
*/
bool Cache::contains(std::string url){

	if(this->pageDataMap.find(url) == this->pageDataMap.end()){
		return false;
	}
	return true;
}


/*
 Function to get page data from hash table
*/
std::string Cache::getPage(std::string url){
	
	if(this->policy == 2){
		if(this->nodeRef.find(url) != this->nodeRef.end()){
			this->urlListLRU.erase(this->nodeRef.find(url)->second);
			this->urlListLRU.push_front(url);
			this->nodeRef.insert(std::pair<std::string,std::list<std::string>::iterator>(url,this->urlListLRU.begin()));
		}
	}
	return this->pageDataMap.find(url)->second;
}


/*
 Function to implement random eviction policy
*/
void Cache::evict(){

	int size = this->urlList.size();
	srand (42);
	int rand_index = rand() % size;
	this->current_cache_size -= this->pageDataMap.find(this->urlList[rand_index])->second.length()*sizeof(char);
	this->pageDataMap.erase (this->urlList[rand_index]);
	this->urlList.erase(this->urlList.begin()+rand_index);
	
}


/*
 Function to implement FIFO eviction policy
*/
void Cache::evictQueue(){
		
	this->current_cache_size -= this->pageDataMap.find(this->urlList.front())->second.length()*sizeof(char);
	this->pageDataMap.erase (this->urlList.front());
	this->urlList.erase(this->urlList.begin());
	
}


/*
 Function to implement LRU replacement policy
*/
void Cache::evictLRU(){

	this->current_cache_size -= this->pageDataMap.find(this->urlListLRU.back())->second.length()*sizeof(char);
	this->pageDataMap.erase (this->urlListLRU.back());
	this->urlListLRU.pop_back();

}

