#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "skiplist.h"
#include "records.h"

int skiplist_init(skiplist* slst)
	{
	srand ( time(NULL) );
	slst->size=0;
	snode *header=(snode*) malloc(sizeof(snode));
	slst->header=header;
	header->key=-1;
	header->above=NULL;
	header->below=NULL;
	header->prev=NULL;
	header->next=NULL;
	header->rec=NULL;
	header->date=NULL;
	int level=0;
	while(coinflip())
		{
		header->above=(snode*) malloc(sizeof(snode));
		header->above->key=-1;
		header->above->above=NULL;
		header->above->below=header;
		header->above->prev=NULL;
		header->above->next=NULL;
		header=header->above;
		header->rec=NULL;
		header->date=NULL;
		level++;
		}
	slst->header=header;
	slst->height=level;
	}

int coinflip()
	{
	return (rand()%2==0);
	}

snode* skiplist_search(skiplist* slst,int key)
	{
	snode* header=slst->header;
	while(header->next!=NULL && key>=header->next->key)
			header=header->next;
	while(header->below!=NULL)
		{
		header=header->below;
		while(header->next!=NULL && key>=header->next->key)
			header=header->next;
		}
	return header;
	}
char* skiplist_req(skiplist* slst,int key,char* tdate)
	{
	snode* temp=skiplist_search(slst,key);
	if(temp->key==key)
		{
		if(temp->date!=NULL)
			{
			char* vdate=timetconverter(temp->date);
			if(sixmonths(vdate,tdate))//elegxei an i im/nia den apexei perissotero apo 6 mhnes des records.h/.c
				{
				free(vdate);
				return "REQUEST ACCEPTED – HAPPY TRAVELS";
				}
			else
				{
				free(vdate);
				return "REQUEST REJECTED – YOU WILL NEED ANOTHER VACCINATION BEFORE TRAVEL DATE";
				}
			}
		}
	return "REQUEST REJECTED – YOU ARE NOT VACCINATED";

	}

int skiplist_insert(skiplist* slst,int key,record* rec,time_t* date)
	{
	snode* temp=skiplist_search(slst,key);
	if(temp->key==key)
		{
		/*
		if(temp->date==NULL)
			printf("ERROR: CITIZEN %d ALREADY NOT VACCINATED\n",temp->key);
		else
			{
			char* dd=timetconverter(temp->date);
			printf("ERROR: CITIZEN %d ALREADY VACCINATED ON %s\n",temp->key,dd);
			free(dd);
			}*/
		return 0;
		}
	snode* new=(snode*) malloc(sizeof(snode));
	new->key=key;
	new->rec=rec;
	new->date=date;
	new->prev=temp;
	new->below=NULL;
	new->above=NULL;
	snode* ssnext=temp->next;
	new->next=ssnext;
	if(ssnext!=NULL)
		ssnext->prev=new;
	temp->next=new;
	int i=0;
	int flag=0;
	while(coinflip())
		{
		i++;
		if (i>slst->height)//if it goes to a bigger height than maximum we need to create a new header
			{
			slst->height=i;
			slst->header->above=(snode*) malloc(sizeof(snode));
			slst->header->above->key=-1;
			slst->header->above->above=NULL;
			slst->header->above->below=slst->header;
			slst->header->above->prev=NULL;
			slst->header->above->next=NULL;
			slst->header=slst->header->above;
			slst->header->rec=NULL;
			slst->header->date=NULL;
			flag=1;
			}
		while(temp->above==NULL)
			temp=temp->prev;
		temp=temp->above;
		new->above=(snode*) malloc(sizeof(snode));
		new->above->below=new;
		new=new->above;
		new->key=key;
		new->prev=temp;
		new->above=NULL;
		new->rec=NULL;
		new->date=NULL;
		ssnext=temp->next;
		new->next=ssnext;
		if(ssnext!=NULL)
			ssnext->prev=new;
		temp->next=new;
		if (flag)
			break;
		}
	slst->size=slst->size+1;
	return 1;
	}

int skiplist_delete(skiplist* slst,int key)
	{
	snode* curr=skiplist_search(slst,key);
	if(curr->key!=key)
		return 0;
	snode* above=NULL;
	do
		{
		above=curr->above;
		if(curr->date!=NULL)
			free(curr->date);
		if(curr->next!=NULL)
			curr->next->prev=curr->prev;
		curr->prev->next=curr->next;
		free(curr);
		curr=above;
		}
	while(curr!=NULL);
	slst->size--;
	return 1;
	}
void skiplist_free(skiplist* slst)
	{
	snode* temp;
	snode* header=slst->header;
	snode* next;
	while(header!=NULL)
		{
		next=header->next;
		while(next!=NULL)
			{
			temp=next;
			next=next->next;
			if(temp->date!=NULL)
				free(temp->date);//we dont free the record because its gonna be freed in its own hashtable
			free(temp);
			}
		temp=header;
		header=header->below;
		if(temp->date!=NULL)
			free(temp->date);
		free(temp);
		}
	free(slst);
	}
//extras
int skiplist_checking(skiplist* slst,int key)
	{
	snode* temp=skiplist_search(slst,key);
	if(temp->key==key)
		{
		/*char* str=timetconverter(temp->date);
		printf("VACCINATED ON %s\n",str);
		free(str);*/
		return 0;
		}
	//printf("NOT VACCINATED\n");
	return 1;
	}
char* skiplist_VaccinationStatus(skiplist* slst,int key,char* virus_name)
	{
	snode* temp=skiplist_search(slst,key);
	if(temp->key==key)
		{
		if(temp->date!=NULL)
			{
			char* dd=timetconverter(temp->date);
			char* buff=(char*) malloc(100*sizeof(char));strcpy(buff,virus_name);
			strcat(buff," VACCINATED ON ");strcat(buff,dd);
			free(dd);
			return buff;
			}
		else if(temp->date==NULL)
			{
			char* buff=(char*) malloc(100*sizeof(char));strcpy(buff,virus_name);
			strcat(buff," NOT YET VACCINATED");
			return buff;
			}
		}
	return NULL;
	}
/*
int main()
	{
	skiplist* slst=(skiplist*) malloc(sizeof(skiplist));
	skiplist_init(slst);
	for (int i = 0; i < 13; ++i)
		{
		if(skiplist_insert(slst,i*13+i,NULL,NULL))
			printf("succesful insert: %d\n",i*13+i );
		}
	
	if(skiplist_delete(slst,112))
		printf("skiplist_delete succesful\n");
	snode* search=skiplist_search(slst,112);
	if(search->key==112)
		printf("skiplist_search succesful\n");
	skiplist_free(slst);
	return 0;
	}
*/