#ifndef _RECORDS__
#define _RECORDS__
#include <stdio.h>
#include <time.h>
//#include "bloomfilter.h"

//#include "skiplist.h"

typedef struct
	{
	unsigned int citizenID;
	char* firstName;
	char* lastName;
	char* country;
	unsigned int age;
	}record; //used for records hashtable type 2

time_t dateconverter(char* date);//converts "dd-mm-yyyy" to time_t or returns -1
int date_between(time_t a,time_t b ,time_t c);//returns 1 if b its between a and c else 0
char* timetconverter(time_t* date);//coverst time_t date to "dd-mm-yyyy"
time_t currdate();//returns current date in time_t
int sixmonths(char* datev,char* datef);//returns 1 if the date isnt older than six months else 0

#endif