#ifndef _SLIST__
#define _SLIST__
#include <string.h>
#include "records.h"
typedef struct snode snode;

typedef struct snode
	{
	int key;//citizen id
	time_t* date;//pointer to a date of vaccination if there is one
	record* rec;//pointer to the record
	snode* above;
	snode* below;
	snode* prev;
	snode* next;
	}snode;

typedef struct
	{
	int height;//current height of skiplisy
	int size;//number of snodes excluding headers
	snode* header;//upper left header
	}skiplist;

int skiplist_init(skiplist* slst);//initializes skip list
int coinflip();//returns 1 for heads 0 for tails
snode* skiplist_search(skiplist* slst,int key);//returns max id snode thats less or equal than ours
char* skiplist_req(skiplist* slst,int key,char* tdate);//epistrefei to katallilo minima gia to travelRequest tu citizen
int skiplist_insert(skiplist* slst,int key,record* rec,time_t* date);//inser new person if he is  not vaccinated  date should be NULL returns 0 if it already exist else 1
int skiplist_delete(skiplist* slst,int key);//deletes snode with the key in all levels returns 1 if its succesful else 0
void skiplist_free(skiplist* slst);//frees skiplist memory
//exrtras
int skiplist_checking(skiplist* slst,int key);//returns 1 if it doesnt exist 0 if it does
char* skiplist_VaccinationStatus(skiplist* slst,int key,char* virus_name);//stelnei ta katallila minhmata gia to searchVaccinationStatus
#endif