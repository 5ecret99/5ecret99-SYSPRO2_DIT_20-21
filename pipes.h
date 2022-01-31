#ifndef _NPIPES__
#define _NPIPES__

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
#define PERMS 0777//used for mkfifo permissions
#define WTIME 99999//o megistos xronos p perimenoun ta polls
#define AVAL 999999//otan kanw  close kai open ena fd amesos kolaei opotan anti na xrhsimopoiw  sleep(1) ka8e fora pu ine arketa xronovoro kanw while(AVAL--) me tin sinartisi waiting()

void create_pipes(int numMonitors);//dhmiourgei 2 named pipes ana monitor
void remove_pipes(int numMonitors);//diagrafei ta pipes pu ftia3ame

int encode(int fd,char* message,int bufferSize);//stelnei minima meso namedpipe
char* decode(int fd,int bufferSize);//diavazi minima meso named pipe

int encode_bloom(int fd,unsigned char* message,int bufferSize,int sizeOfBloom);//stelnei ena bloom meso named pipe
unsigned char* decode_bloom(int fd,int bufferSize,int sizeOfBloom);//diavazi ena bloom meso named pipe
void waiting();//des AVAL


#endif