#ifndef _BLOOM__
#define _BLOOM__

typedef struct
	{
	unsigned char* buff;
	int bits,bytes;
	int ready;//if its not initialized ready is 0
	}bloom;
int bloom_init(bloom* bl,unsigned int size);//initializes bloomfilter 

int bloom_initb(bloom* bl,unsigned char* buff,unsigned int size);//kanei initialize me iparxwn bloom filter

int bloom_check(bloom* bl,unsigned char* ch);//returns 0 if it checks else 1

int bloom_add(bloom* bl,unsigned char* ch);//adds it to the bloomfilter

void bloom_free(bloom* bl);//frees bloomfilter



#endif