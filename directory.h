#ifndef _DIRECTORY__
#define _DIRECTORY__

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

#include "hash.h"
#include "hashtable.h"

int dir(char*** namelist,hashtable** cp,char* input_dir,int numMonitors);//returns number of countries and creates  a namelist array of char*. namelist[i] contains string array for directories assigned to monitor i.
//Also creates cp hashtable witch acts like a dictionary between countries and monitors for parent process
void remove_namelist(char*** namelist,int numMonitors);//frees namilist array of char*
#endif