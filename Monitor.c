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
# include <poll.h>

#include "pipes.h"
#include "hashtable.h"
#include "child_functions.h"
#include "hash.h"
#include "signal.h"

int SIGNUM;

void sighandler(int sig) //sighandler pu apo8ikevei stin metavliti SIGNUM ton ari8mo tou shmatos
	{
	//printf("Caught signal %d, coming out... in process with pid %d\n", sig,getpid());
	SIGNUM=sig;
	return;
	}

int main(int argc, char const *argv[])
	{
	SIGNUM=0;

	char buff[30];	
	char pipeb[50];strcpy(pipeb,argv[2]);//pipe pu stelnei o pateras sto pedi
	char pipea[50];strcpy(pipea,argv[1]);//pipe pu stelnei to paidi ston patera

	//arxikopoiw ta aparetita hashtables
	hashtable* viruses=(hashtable*)malloc(sizeof(hashtable));viruses=hashtable_init(viruses,50,0);//me tus ious (2 skiplist kai 1 bloom opws thn prwth ergasia) des hashtable.h/.c gia tin domh
	hashtable* records=(hashtable*)malloc(sizeof(hashtable));records=hashtable_init(records,500,2);//me ta records h domh vriskete sto records.h/.c
	hashtable* countries=(hashtable*)malloc(sizeof(hashtable));countries=hashtable_init(countries,20,-1);//me tis countries den xriazete domh to void* data = char* country
	hashtable* directories=(hashtable*)malloc(sizeof(hashtable));directories=hashtable_init(directories,100,-1);//me ta directories den xriazete domh to void* data = char* directory
	
	//8a diavasw to buffersize apto katallilo pipe
	
	int fdb=open(pipeb,O_RDONLY);if(fdb<0){perror ("couldnt open pipe in a child");return 1;}
	//POLL ETSI WSTE NA PERIMENEI
	int flag=0;
	do{
	struct pollfd fdarray [1];
	int rc;
	fdarray [0]. fd = fdb;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc ==1) && ( fdarray [0]. revents == POLLIN ) )
    		{
   	 		if ( fdarray [0]. fd == fdb )
        		{
				if(read(fdb, buff, 30)<1){perror("error with reading buffersize");flag= 1;}
				else{flag=0;}
				}
			else{perror(" fdarray [0]. fd != fd\n");flag= 1;}
			}
		else{flag= 1;}
	}while(flag);
	int bufferSize=atoi(buff);

	//diavazw ta directories
	char* directory=decode(fdb,bufferSize);
	char* directory_copy=(char*) malloc((strlen(directory)+1)*sizeof(char));strcpy(directory_copy,directory);


	//diavazw to bloomsize
	char*  sizeOfBloomSTR=decode(fdb,bufferSize);
	int sizeOfBloom=atoi(sizeOfBloomSTR);free(sizeOfBloomSTR);

	printf("process %d created  with buffersize %d sizeOfBloom %d\n", getpid(),bufferSize,sizeOfBloom);

	//sinartisi pu diavazei ola ta arxeia se ka8e directory kai ta vazei stus analogous hashtables
	//des child_functions.h/.c
	readdirectory(directory_copy,sizeOfBloom,viruses,records,countries,directories);free(directory_copy);

	//sinartisi pu stelnei ola ta blooms ston patera
	//des child_functions.h/.c
	int fda=open(pipea,O_WRONLY);if(fda<0){perror ("couldnt open pipe in a child");return 1;}
	sendblooms(fda,viruses,bufferSize,sizeOfBloom);

	//assigning signals to sighandler
	signal(SIGUSR1,sighandler);
	signal(SIGUSR2,sighandler);
	//signal(SIGKILL,sighandler);//cant catch a SIGKILL
	signal(SIGINT,sighandler);
	signal(SIGQUIT,sighandler);

	for(;;)
		{
		
		sleep(2);//xrhsi sleep ephdh otan den to eixa ekane sinexei elegxous sta if me apotelesma na katanalwnwntai poroi xwris logo
		/*
		cant catch a SIGKILL
		if (SIGNUM==9)//SIGKILL
			{
			SIGNUM=0;
			break;
			}*/
		if (SIGNUM==30||SIGNUM==16 ||SIGNUM==10)//SIGUSR1 
			//pros8e8ikan arxeia sta directories
			{
			SIGNUM=0;

			directory_copy=(char*) malloc((strlen(directory)+1)*sizeof(char));strcpy(directory_copy,directory);
			//des child_functions.h/.c
			//3anadiavazw ta directories an vrw nea arxeia pros8etw tis plirofories tus
			readdirectory(directory_copy,sizeOfBloom,viruses,records,countries,directories);
			free(directory_copy);
			//stelnw ta blooms des child_functions.h/.c
			sendblooms(fda,viruses,bufferSize,sizeOfBloom);

			}
		else if(SIGNUM==2||SIGNUM==3)//SIGINT SIGQUIT
			{
			SIGNUM=0;
			//dhmiourgia logfile des signal.h/.c
			logfile(getpid(),countries,totalreq,accepted,rejected,0);
			break;
			}
		else if(SIGNUM==31||SIGNUM==12 ||SIGNUM==17)//SIGUSR2
			//shmainei oti 8a steilei o parent command mesw named pipe
			{

			//diavazei thn entolh
			char* command=decode(fdb,bufferSize);

			if (strcmp(command,"/exit")==0)//command exit
				{
				char buff[30];sprintf(buff,"%d",accepted);
				//stelnei ton ari8mo twn accepted
				encode(fda,buff,bufferSize);

				//stelnei ton ari8mo twn rejected
				sprintf(buff,"%d",rejected);
				encode(fda,buff,bufferSize);

				//apeleftherosi porwn
				
				free(directory);
				hashtable_free(directories);
				hashtable_free(countries);
				hashtable_free(records);
				hashtable_free(viruses);

				close(fda);close(fdb);

				//endless loop mexri na dw8ei sigkill apto patera
				while(1);

				}
			if (strcmp(command,"/travelRequest")==0)//command travelRequest
				{
				printf("travelRequest chikd\n" );

				char* virusName=decode(fdb,bufferSize);

				char* citizenID = decode(fdb,bufferSize);

				char* tdate=decode(fdb,bufferSize);

				char* message =Child_travelRequest(viruses,virusName,atoi(citizenID),tdate);//des child_functions.h/,c


				encode(fda,message,bufferSize);
				free(virusName);free(citizenID);free(tdate);
				}
			else if (strcmp(command,"/travelStats")==0||strcmp(command,"/travelStatsC")==0)//command travelStats
				{				

				char*virusName=decode(fdb,bufferSize);

				char*date1=decode(fdb,bufferSize);
				time_t dd1=dateconverter(date1);free(date1);

				char*date2=decode(fdb,bufferSize);
				time_t dd2=dateconverter(date2);free(date2);
				char* country=NULL;

				if(strcmp(command,"/travelStatsC")==0)//an ine gia sigkekrimeni xwra tin diavazei apto pipe
					{
					country=decode(fdb,bufferSize);
					}

				travelStats(viruses,country,virusName,dd1,dd2,fda,bufferSize);//des child_functions.h/,c
				free(virusName);if(country!=NULL){free(country);}
				}
			else if (strcmp(command,"/searchVaccinationStatus")==0)//command searchVaccinationStatus
				{


				char*citizenID=decode(fdb,bufferSize);

				searchVaccinationStatus(records,viruses,atoi(citizenID),fda,bufferSize);//des child_functions.h/,c
				free(citizenID);
				}

			if(command!=NULL){free(command);command=NULL;}
			}
		}
	//apelefthrwsi mnimis
	close(fda);close(fdb);
	free(directory);
	hashtable_free(directories);
	hashtable_free(countries);
	hashtable_free(records);
	hashtable_free(viruses);
	printf("child terminated by signal and freed succesfully\n");
	return 0;
	}