#ifndef _CHFFUNCTIONS__
#define _CHFFUNCTIONS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/errno.h>
#include <dirent.h>
#include <string.h>

#include "hash.h"
#include "bloomfilter.h"
#include "skiplist.h"
#include "records.h"
#include "hashtable.h"
#include "pipes.h"

//global variables
int totalreq;
int accepted;
int rejected;

void readdirectory(char* directory,int sizeOfBloom,hashtable* viruses,hashtable* records,hashtable* countries,hashtable* directories);//diavazei to directory ana arxeio pu den exei diavasei hdh

void insertCitizenRecord(int bloomSize,hashtable* viruses,hashtable* records,hashtable* countries,char* citizenID,char* firstName,char* lastName,char* country,char* age,char* virus_name,char* yes_no,char* date);//vazei ton citizen stous katallilous hashtable idia me prwti ergasia

char* Child_travelRequest(hashtable* viruses,char* virusName,int citizenID,char* tdate);//epistrefei to katallilo minima gia to travelRequest tu citizen

void sendblooms(int fda,hashtable* viruses,int bufferSize,int sizeOfBloom);//stelnei ola ta blooms ana virus ston patera

void travelStats(hashtable* viruses,char* country,char* virusName,time_t a,time_t b,int fda,int bufferSize);//function pu stelnei ta travelStats ston patera

void end_searchVaccinationStatus(int fda,int bufferSize);//stelnei end_searchVaccinationStatus meso pipe ston patera
void searchVaccinationStatus(hashtable* records,hashtable* viruses,int citizenID,int fda,int bufferSize);//psaxnei kai stelnei ola ta stoixeia gia ton citizen pu iparxun se ato to monitor




#endif