#define K 16
//#define M 100000
#include "bloomfilter.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

int bloom_init(bloom* bl,unsigned int size)
	{
	bl->ready=0;
	bl->bits=8*size;
	bl->bytes=size;
	bl->buff=(unsigned char*)calloc(size,sizeof(unsigned char));
	if(bl->buff==NULL)
		{
		printf("bloom not initialized\n");
		return 1;
		}
	bl->ready=1;
	return 0;
	}

int bloom_initb(bloom* bl,unsigned char* buff,unsigned int size)
	{
	bl->ready=0;
	bl->bits=8*size;
	bl->bytes=size;
	bl->buff=buff;
	if(bl->buff==NULL)
		{
		printf("bloom not initialized\n");
		return 1;
		}
	bl->ready=1;
	return 0;
	}

int bloom_check(bloom* bl,unsigned char* ch)
	{
	if(bl->ready!=1)
		{
		printf("bloom not initialized\n");
		return 1;
		}
	int i,hits=0;
	unsigned long hash;
	unsigned int mask;
	unsigned int byte;
	unsigned char tempc;
	for(i=0;i<K;i++)
		{
		hash=hash_i(ch,i)%bl->bits;
		byte=hash>>3;
		mask=1<<(hash%8);
		tempc=bl->buff[byte];
		if((tempc & mask)==mask)
			{
			hits++;
			}
		else
			return 1;

		}
	if(hits==K)
		{
		return 0;
		}
	return 1;
	}

int bloom_add(bloom* bl,unsigned char* ch)
	{
	if(bl->ready!=1)
		{
		printf("bloom not initialized\n");
		return 1;
		}
	int i,hits=0;
	unsigned long hash;
	unsigned int mask;
	unsigned int byte;
	unsigned char tempc;
	for(i=0;i<K;i++)
		{
		hash=hash_i(ch,i)%bl->bits;
		byte=hash>>3;
		mask=1<<(hash%8);
		tempc=bl->buff[byte];
		/*
		if((tempc & mask)==mask)
			hits++;
		else
			*/
			bl->buff[byte]=tempc | mask;
			

		}
	return 0;
	}

void bloom_free(bloom* bl)
	{
	if(bl->buff!=NULL)
		{
		free(bl->buff);
		}
	bl->ready=0;
	free(bl);
	}
/*
int main()
	{
	bloom* blo=(bloom*) malloc(sizeof(bloom));
	bloom_init(blo,100000);
	int check=bloom_check(blo,"12345");
	printf("check1: %d\n",check);
	int add=bloom_add(blo,"12345");
	check=bloom_check(blo,"12345");
	printf("add1: %d check2: %d\n",add,check);
	bloom_free(blo);
	}

*/
