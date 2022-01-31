#ifndef _PAFFUNCTIONS__
#define _PAFFUNCTIONS__

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

void receiveblooms(int fd,int monitor,int bufferSize,int sizeOfBloom,hashtable** bloomhasharr);//synartisi gia na diavasei ta blooms apto  monitor me ari8mo int monitor kai na to valei sto bloomhasharr[i]
//void travelRequest(char* countryFrom,char* virus_name,char* citizenID,char* date,hashtable* cp,hashtable** bloomhasharr,int bufferSize,pid_t* child,int** fd);//function gia thn entolh travelRequest

#endif