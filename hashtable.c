#include "hashtable.h"
#include "records.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////BUCKET FUNCTIONS////////////////////////////////////////////

bucket* bucket_init(bucket*header,unsigned long key)
	{
	if(header==NULL)
		return NULL;
	header->key=key;
	header->next=NULL;
	header->data=NULL;
	return header;
	}
bucket* bucket_check(bucket*header,unsigned long key)
	{
	bucket* temp=header;
	while(temp!=NULL)
		{
		if(temp->key==key)
			return temp;
		temp=temp->next;
		}
	return NULL;
	}
bucket* bucket_insert(bucket*header,unsigned long key)
	{
	if(header==NULL)
		return NULL;
	bucket* prev=NULL;
	bucket* curr=header;
	do
		{
		if(curr->key==key)
			return curr;
		prev=curr;
		curr=curr->next;
		}
	while(curr!=NULL);
	bucket* new=(bucket*) malloc(sizeof(bucket));
	new->key=key;
	new->next=NULL;
	new->data=NULL;
	prev->next=new;
	return new;
	}

void bucket_free(bucket*header,int type)
	{
	if(header!=NULL)
		{
		bucket_free(header->next,type);//frees the next one if it exist recursively
		if(header->data!=NULL)
			{
			if(type==0)//sees what type is it
				{
				virus_child* vv=(virus_child*) header->data;
				if(vv->virus_name!=NULL)
					free(vv->virus_name);
				if(vv->not_vaccinated_bloom!=NULL)
					bloom_free(vv->not_vaccinated_bloom);
				if(vv->vaccinated_persons!=NULL)
					skiplist_free(vv->vaccinated_persons);			
				if(vv->not_vaccinated_persons!=NULL)
					skiplist_free(vv->not_vaccinated_persons);
				}
			else if(type==1)
				{
				virusparent* vv=(virusparent*) header->data;
				if(vv->virus_name!=NULL)
					{
					free(vv->virus_name);
					}
				if(vv->not_vaccinated_bloom!=NULL)
					{
					bloom_free(vv->not_vaccinated_bloom);
					}
				}

			else if(type==2)
				{
				record* rec=(record*) header->data;
				if(rec->firstName!=NULL)
					free(rec->firstName);
				if(rec->lastName!=NULL)
					free(rec->lastName);
				/*if(rec->country!=NULL)
					free(rec->country);*/
				}
			free(header->data);
			}
		free(header);
		}
	}

/////////////////////////////////////////HASHTABLE FUNCTIONS////////////////////////////////////////////


hashtable* hashtable_init(hashtable* hasht,int bucket_num,int type)
	{
	if(hasht==NULL || bucket_num<1)
		return NULL;
	hasht->count=0;
	hasht->type=type;
	hasht->buckets=(bucket**) malloc(bucket_num*sizeof(bucket*));
	for (int i = 0; i < bucket_num; ++i)
		{
		hasht->buckets[i]=NULL;
		}
	hasht->bucket_num=bucket_num;
	return hasht;
	}
bucket* hashtable_check(hashtable* hasht,unsigned long key)
	{
	if(hasht==NULL)
		return NULL;
	int hashv=(int) (key % (hasht->bucket_num));
	bucket* header=hasht->buckets[hashv];
	if (header==NULL)
		return NULL;
	return (bucket_check(header,key));
	}
bucket* hashtable_insert(hashtable* hasht,unsigned long key)
	{
	if(hasht==NULL)
		return NULL;
	int hashv=(int) (key % (hasht->bucket_num));
	bucket* header=hasht->buckets[hashv];
	if (header==NULL)
		{
		header=(bucket*) malloc(sizeof(bucket));
		hasht->buckets[hashv]=bucket_init(header,key);
		hasht->count++;
		return header;
		}
	header=bucket_insert(header,key);
	if(header->data==NULL)
		hasht->count++;
	return header;
		
	}
void hashtable_free(hashtable* hasht)
	{
	for (int i = 0; i < hasht->bucket_num;i++)
		{
		bucket_free(hasht->buckets[i],hasht->type);
		}
	free(hasht->buckets);
	free(hasht);
	}

//extras

/*
int main()
	{
	hashtable* hasht=(hashtable*) malloc(sizeof(hashtable));
	virus* vv=(virus*)malloc(sizeof(virus));
	vv->virus_name=(char*)malloc(7*sizeof(char));
	strncpy(vv->virus_name,"corona",6);
	printf("before: %s\n",vv->virus_name );
	hashtable_init(hasht,6,1);
	bucket* bb;
	for (int i = 0; i < 12; ++i)
		{
		printf("i: %d\n",i);
		bb=hashtable_insert(hasht,i*13+i);
		if(i==1)
			bb->data=vv;
		printf("ok %d\n",i*13+i);
		}
	bb=hashtable_insert(hasht,14);
	if(bb==NULL)
		printf("bb NULL\n");
	vv=(virus*)bb->data;
	printf("after: %s\n",vv->virus_name );
	hashtable_free(hasht);
	}
	*/