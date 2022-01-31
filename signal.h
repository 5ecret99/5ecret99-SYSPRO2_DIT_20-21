#ifndef _SIGNAL__
#define _SIGNAL__

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
#include <poll.h>

#include "hashtable.h"


void logfile(int process_id,hashtable* countries,int totalreq,int accepted,int rejected,int parent);//ftiaxni to logfile toso gia ton parent toso kai gia ta paidia

#endif

