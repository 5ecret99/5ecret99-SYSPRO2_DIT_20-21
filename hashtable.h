#ifndef _HASHTABLE__
#define _HASHTABLE__
#include "hash.h"
#include "bloomfilter.h"
#include "skiplist.h"

typedef struct 
	{
	char* virus_name;
	bloom* not_vaccinated_bloom;
	skiplist* vaccinated_persons;
	skiplist* not_vaccinated_persons;
	}virus_child;//struct used for each virus hashtable in Monitor.c(children) type 0

typedef struct 
	{
	char* virus_name;
	bloom* not_vaccinated_bloom;
	}virusparent;//struct used for virus hashtable in travelMonitor.c(parent process) type 1

typedef struct 
	{
	char country[50];
	int monitor;
	}cm;//struct used to identify the monitor assigned to each country;

typedef struct bucket bucket;

typedef struct bucket
	{
	unsigned long key;
	void* data;
	bucket* next;
	}bucket;

bucket* bucket_init(bucket*header,unsigned long key);//initializes bucket-node
bucket* bucket_check(bucket*header,unsigned long key);//returns the bucket-node if it finds it else NULL
bucket* bucket_insert(bucket*header,unsigned long key);//insers new bucket-node if doesnt already exist and returns its pointer
void bucket_free(bucket*header,int type);//frees this and all the bucket-nodes next to this one acording to their type

typedef struct
	{
	bucket** buckets;
	int bucket_num;//number of buckets
	int type;//type of hashtable 0  is for virus using the virus struct 1 for the records using the records struct any else is for every type **Warning** if that type has pointers in it ,it doesnt free em
	int count;//count of items
	}hashtable;

hashtable* hashtable_init(hashtable* hasht,int bucket_num,int type);//initializes hashtable
bucket* hashtable_check(hashtable* hasht,unsigned long key);//returns the bucket-node if it finds it else NULL
bucket* hashtable_insert(hashtable* hasht,unsigned long key);//insers new bucket-node if doesnt already exist and returns its pointer
void hashtable_free(hashtable* hasht);//frees the whole hashtable
//extras
//int vaccineStatus(hashtable* viruses,int citizenID);
#endif