
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

#include "pipes.h"
#include "directory.h"
#include "hashtable.h"
#include "bloomfilter.h"
#include "parent_functions.h"
#include "hash.h"
#include "signal.h"

int SIGNUM;//metavliti pu exei ton ari8mo twn running children process
void sighandler(int sig) //sighandler etsi wste na meiwnei ton pio panw ari8mo ka8e fora pu termatizete ena paidi
	{
	//printf("signal %d process %d \n", sig,1);
	SIGNUM--;
	return;
	}

int main(int argc, char const *argv[])
	{
	//checking command line arguments
	if(argc!=9){printf("input should be like this:\n./travelMonitor -m numMonitors -b bufferSize -s sizeOfBloom -i input_dir\n");return 1;}
	if (argv[1][1]!= 'm' || argv[3][1] != 'b'|| argv[5][1]!= 's'|| argv[7][1]!= 'i'){printf("wrong -symbol\n");return 1;}
	//initializing variables from command line arguments
	int numMonitors=atoi(argv[2]);
	int bufferSize=atoi(argv[4]);
	char bufferSizeSTR[30];strcpy(bufferSizeSTR,argv[4]);
	int sizeOfBloom=atoi(argv[6]);
	char sizeOfBloomSTR[30];strcpy(sizeOfBloomSTR,argv[6]);
	char input_dir[100];strcpy(input_dir,argv[8]);
	
 	//assignig directories for each monitor
 	char** namelist;//sto namelist[i] exw ola ta directories pu anate8ontai ston monitor i me round robin alphabetically
 	hashtable* cp;//hashtable pou leitourgei ews dictionary  exwntas (xwra,monitor)
 	int numCountries=dir(&namelist,&cp,input_dir,numMonitors);//sinartisi des directory.h,directory.c
 	if(numMonitors>numCountries){numMonitors=numCountries;}//an oi monitor einai perissoteroi aptis xwres tote tus miwnw ston ari8mo twn xwrwn kai den tha tus dhmiourgisw giati einai spatali porwn
 	SIGNUM=numMonitors;

	printf("numCountries %d \n",numCountries);
	printf("_______________________________________________________________________________\n");
	create_pipes(numMonitors);//dhmiourgw ta named pipes vlepe pipes.h,pipes.c
	int fd[2][numMonitors];

	pid_t parent = getpid(); // parent id
 	pid_t child[numMonitors];
	char argva[50];
	char argvb[50];

	for (int i = 0; i < numMonitors; ++i)
		{
		//ta namedpipes tu ka8e monitor
		sprintf(argva,"./Child2Parent%d",i);
		sprintf(argvb,"./Parent2Child%d",i);
		int pid=fork();
		if(pid==-1)//to fork apetyxe teliwnw to programma
			{
			perror("fork failed\n");
			return 1;
			}
		else if (pid==0)//ine paidi eleftherwnw tus porous kai kalw thn execl me orismata ta named pipes
			{
			//printf("son with pid: %d from parent with pid: %d \n",getpid(),getppid());
			remove_namelist(&namelist,numMonitors);
			hashtable_free(cp);
			execl("./Monitor","./Monitor",argva,argvb,(char*) 0);
			perror("error execl was called\n");
			return 1;
			}
		else
			{
			child[i]=pid;

			struct pollfd fdarray [1];
			int rc;
			//arxika stelnw to buffersize
			//kanw poll gia na perimenei
			fd[1][i]=open(argvb,O_WRONLY);if(fd[1][i]<0){perror ("couldnt open pipe in parent");return 1;}
			int flag=0;
			do{
			fdarray [0]. fd = fd[1][i];
			fdarray [0]. events = POLLOUT;
			rc = poll( fdarray , 1 , WTIME);
			if ( rc == 0) {perror("Poll timed - out\n");return 1;}
			else if ( ( rc ==1) && ( fdarray [0]. revents == POLLOUT ) )
    			{
				if ( fdarray [0]. fd == fd[1][i] )
					{
					if(write(fd[1][i],bufferSizeSTR, 30)<0){perror("couldnt write on pipe");flag=1;}
					else{flag=0;}
					}//stelnw to buffersize to exw kai sto monitor kai sto travelMonitor na stelnoun  kai na diavazoun char* me length 30}
				else{perror("fdarray [0]. fd != fd");flag =1;}
				}
			else{flag=1;}
			}while(flag);
			//stelnw ta directories tu ka8e monitor
			encode(fd[1][i],namelist[i],bufferSize);//sinartisi pu xrisimopoiw gia na steilw ena char* mesw named pipe des pipes.h,pipes.c

			//stelnw to sizeOfBloom
			encode(fd[1][i],sizeOfBloomSTR,bufferSize);

			}
		}
	//ftiaxnw ena pinaka apo hashtables opou sto bloomhasharr[i] vriskete ena hashtable me ola ta blooms gia to monitor i 
	hashtable** bloomhasharr=(hashtable**) malloc(numMonitors*sizeof(hashtable*));

	for (int i = 0; i < numMonitors; ++i)
		{
		sprintf(argva,"./Child2Parent%d",i);
		fd[0][i]=open(argva,O_RDONLY);if(fd[1][i]<0){perror ("couldnt open pipe in parent");return 1;}
		bloomhasharr[i]=NULL;
		receiveblooms(fd[0][i],i,bufferSize,sizeOfBloom,bloomhasharr);//sinartisi pu xrhsimopoiw gia na diavasw ta blooms des parent_functions.h/.c

		}
	signal(SIGCHLD,sighandler);//signal handler gia na pernei ta simata apo termatismena paidia (SIGCHLD)
	
	printf("_______________________________________________________________________________\n");
	printf("received bloomfilters from all the Monitors give one of the following commands:\n");
	printf("/travelRequest citizenID date countryFrom countryTo virusName\n");
	printf("/travelStats virusName date1 date2 [country]\n");
	printf("/addVaccinationRecords country\n");
	printf("/searchVaccinationStatus citizenID\n/exit\n");

	char a[7][50];
	int word_count=0;
	char* words;
	size_t len;
	char* line=NULL;
	while(1)
		{
		printf("_______________________________________________________________________________\n");
		printf("write command:\n");
		word_count=0;
		//spazw to command se lekseis
		getline(&line,&len,stdin);
		words= strtok (line," \n\0");
		while(words!=NULL)
			{
			if(word_count<8)//the biggest command has 8 words
				strcpy(a[word_count],words);
			else
				{break;}//den iparxei command me perissoteres apo 7 lekseis ara ine axriasto na tis diavasw
			words= strtok (NULL," \n\0");
			word_count++;
			}

		//elegxw an exei termatistei kapoios monitor prwtou ektelesw kapoia entolh
		if(SIGNUM<numMonitors)//recreate a monitor
			{
			printf("some childs were terminated\n");
			for (int i = 0; i < numMonitors; ++i)
				{
				int status;
    			int wait =waitpid(child[i], &status, WNOHANG);
    			//an to wait epistrepsei timi !=0 simainei oti o sigkekrimenow monitor termatisthke
    			if(wait!=0)
    				{
    				sprintf(argva,"./Child2Parent%d",i);
					sprintf(argvb,"./Parent2Child%d",i);
					int pid=fork();
					if(pid==-1)
						{
						perror("fork failed\n");
						break;
						}
					else if (pid==0)
						{
						
						free(bloomhasharr);
						hashtable_free(cp);
						remove_namelist(&namelist,numMonitors);
						execl("./Monitor","./Monitor",argva,argvb,(char*) 0);
						perror("error execl was called\n");
						return 1;		
						}
					else
						{
						child[i]=pid;
						int flag=0;
						do{
						struct pollfd fdarray [1];
						int rc;

						fdarray [0]. fd = fd[1][i];
						fdarray [0]. events = POLLOUT;
						rc = poll( fdarray , 1 , WTIME);
						if ( rc == 0) {perror("Poll timed - out\n");return 1;}
						else if ( ( rc ==1) && ( fdarray [0]. revents == POLLOUT ) )
    						{
							if ( fdarray [0]. fd == fd[1][i] ){write(fd[1][i],bufferSizeSTR, 30);flag=0;}
							else{perror("fdarray [0]. fd != fd");flag= 1;}
							}
						else
							{
							flag =1;
							}
						}while(flag);

						encode(fd[1][i],namelist[i],bufferSize);

						encode(fd[1][i],sizeOfBloomSTR,bufferSize);

						receiveblooms(fd[0][i],i,bufferSize,sizeOfBloom,bloomhasharr);
						SIGNUM++;
						}
    				}
				}

			if(SIGNUM!=numMonitors){printf("lost some SIGCHLD signals evrything is alright tho ;) \n");SIGNUM=numMonitors;}
			}


		if (strcmp(a[0],"/exit")==0)//exit command
			{
			int total_accepted=0;int total_rejected=0;
			if (word_count!=1){printf("given less or more arguments for %s command\n",a[0]);break;}
			
			for (int i = 0; i < numMonitors; ++i)
				{

				//prwta stelnw SIGUSR2 gia na 3eri to paidi oti perimenei entolh
				kill(child[i], SIGUSR2);

				//stelnw thn entolh
				encode(fd[1][i],"/exit",bufferSize);

				char* accepted=decode(fd[0][i],bufferSize);
				total_accepted+=atoi(accepted);free(accepted);

				char* rejected=decode(fd[0][i],bufferSize);
				total_rejected+=atoi(rejected);free(rejected);
				//pernw ta accepted kai rejected apo ka8e paidi kai ta poros8etw sto oliko mou

				kill(child[i],SIGKILL);//termatizw to paidi
				}
			//ftiaxnw to logfile tu patera des signal.h/.c
			logfile(getpid(),cp,total_accepted+total_rejected,total_accepted,total_rejected,1);

			//waiting for monitors to terminate 
			for (int i = 0; i < numMonitors; ++i)
				{
				int status;
    			waitpid(child[i], &status, 0);
				}

			break;
			}
		else if (strcmp(a[0],"/travelRequest")==0)//travelRequest command
			{
			if (word_count!=6){printf("given less or more arguments for %s command\n",a[0]);break;}
			char* countryFrom=a[3];
			char* virus_name=a[5];
			char* citizenID=a[1];
			char* date=a[2];

			//des  parent_functions.h/.c
			bucket* cpbucket=hashtable_check(cp,hash_i(countryFrom,HVALUE));//vriskw ton monitor gia thn xwra
			if(cpbucket!=NULL)//ean den iparxei kai einai NULL shmainei den exoume thn xwra auth
				{
				cm* cpdata=(cm*) cpbucket->data;

				if(cpdata!=NULL && strcmp(cpdata->country,countryFrom)==0)//ean teriazei h xwra
					{
					int monitor=cpdata->monitor;
					if(bloomhasharr[monitor]!=NULL)
						{
						bucket* bha_bucket =hashtable_check(bloomhasharr[monitor],hash_i(virus_name,HVALUE));//vriskw to katallilo bloom gia to sigkekrimeno virus tu sigkekrimenou monitor
						if (bha_bucket!=NULL)
							{
							virusparent* bha_data=(virusparent*) bha_bucket->data;
							if(bha_data!=NULL && strcmp(bha_data->virus_name,virus_name)==0)
								{
								if(bloom_check(bha_data->not_vaccinated_bloom,citizenID))
									printf("REQUEST REJECTED – YOU ARE NOT VACCINATED\n");
								else //ean to bloom_check ine arnitiko elegxw ston katallilo monitor
									{
									kill(child[monitor], SIGUSR2);
							
									encode(fd[1][monitor],"/travelRequest",bufferSize);

							
									encode(fd[1][monitor],virus_name,bufferSize);
	
									encode(fd[1][monitor],citizenID,bufferSize);
				
									encode(fd[1][monitor],date,bufferSize);
									printf("alright\n");

									char* message =decode(fd[0][monitor],bufferSize);
									printf("%s\n",message);if(message!=NULL){free(message);}
									}
								}
							}
						}
					}
				}
			}
		else if (strcmp(a[0],"/travelStats")==0)//travelStats command
			{
			if (word_count!=4 && word_count!=5){printf("given less or more arguments for %s command\n",a[0]);break;}

			char* date2=a[3];
			char* date1=a[2];
			char* virusName=a[1];
			if(word_count==5)//shmainei oti 8elei gia sigkekrimeni xwra
				{
				char* country=a[4];
				bucket* cpbucket=hashtable_check(cp,hash_i(country,HVALUE));
				if(cpbucket!=NULL)
					{
					cm* cpdata=(cm*) cpbucket->data;
					if(cpdata!=NULL && strcmp(cpdata->country,country)==0)
						{
						int monitor=cpdata->monitor;
						//stelnw to sima
						kill(child[monitor], SIGUSR2); sleep(1);
						//stelnw thn entolh
						encode(fd[1][monitor],"/travelStatsC",bufferSize);

						//stelnw tis aparetites plirofories		
						encode(fd[1][monitor],virusName,bufferSize);

						encode(fd[1][monitor],date1,bufferSize);

						encode(fd[1][monitor],date2,bufferSize);

						encode(fd[1][monitor],country,bufferSize);


						//pernw ton ari8mo twn accepted
						char* accepted=decode(fd[0][monitor],bufferSize);

						//pernw ton ari8mo twn rejected
						char* rejected=decode(fd[0][monitor],bufferSize);

						printf("TOTAL REQUESTS %d\n",atoi(accepted)+atoi(rejected));// to total ine to a8roisma autwn twn duo
						printf("ACCEPTED %s\n",accepted);free(accepted);
						printf("REJECTED %s\n",rejected);free(rejected);
						}
					}

				}
			else if(word_count==4)//8elei gia oles tis xwres
				{
				int total_accepted=0;
				int total_rejected=0;
				for (int i = 0; i < numMonitors; ++i)//se ka8e monitor
					{
					kill(child[i], SIGUSR2);
					encode(fd[1][i],"/travelStats",bufferSize);
									
					encode(fd[1][i],virusName,bufferSize);

					encode(fd[1][i],date1,bufferSize);

					encode(fd[1][i],date2,bufferSize);


					char* accepted=decode(fd[0][i],bufferSize);
					total_accepted+=atoi(accepted);free(accepted);//pros8etw ka8e fora sto oliko ton accepted

					char* rejected=decode(fd[0][i],bufferSize);
					total_rejected+=atoi(rejected);free(rejected);//pros8etw ka8e fora sto oliko ton rejected
					}
				printf("TOTAL REQUESTS %d\n",total_accepted+total_rejected);
				printf("ACCEPTED %d\n",total_accepted);
				printf("REJECTED %d\n",total_rejected);
				}
			}
		else if (strcmp(a[0],"/addVaccinationRecords")==0)//command addVaccinationRecords
			{
			if (word_count!=2){printf("given less or more arguments for %s command\n",a[0]);break;}
			char* country=a[1];
			bucket* cpbucket=hashtable_check(cp,hash_i(country,HVALUE));
			if(cpbucket!=NULL)
				{
				cm* cpdata=(cm*) cpbucket->data;
				if(cpdata!=NULL && strcmp(cpdata->country,country)==0)
					{
					int monitor=cpdata->monitor;
					kill(child[monitor], SIGUSR1);//signal SIGUSR1 giana 3erei to paidi na 3anapsa3ei sta directories tu

					receiveblooms(fd[0][monitor],monitor,bufferSize,sizeOfBloom,bloomhasharr);//vlepe parent_functions.h/.c
					}
				}
			}
		else if (strcmp(a[0],"/searchVaccinationStatus")==0)//command searchVaccinationStatus
			{
			if (word_count!=2){printf("given less or more arguments for %s command\n",a[0]);break;}
			char* citizenID=a[1];

			char test[30];sprintf(test,"%d",atoi(citizenID));

			if(strcmp(test,citizenID)!=0){printf("citizenID contains non numeric characters\n");break;}

			for (int i = 0; i < numMonitors; ++i)
					{
					sprintf(argvb,"./Parent2Child%d",i);
					sprintf(argva,"./Child2Parent%d",i);

					kill(child[i], SIGUSR2);

					encode(fd[1][i],"/searchVaccinationStatus",bufferSize);

					encode(fd[1][i],citizenID,bufferSize);

					char* buff=decode(fd[0][i],bufferSize);

					int flags=0;
					//otan den exei perissoteres plhrofories ena paidi gia to sigkekrimeno id stelnei "end_searchVaccinationStatus" mesw tou named pipe
					//ta paidia pu den exoun to id stelnoun e3arxhs to end_searchVaccinationStatus
					while(strcmp(buff,"end_searchVaccinationStatus")!=0)
						{
						flags=1;
						printf("%s\n",buff);
						free(buff);

						buff=decode(fd[0][i],bufferSize);
						}
					free(buff);
					if(flags){break;}//epeidh to ka8e id einai mono se ena monitor an stal8un plhrofories pavei to psaksimo sta ipoloipa paidia
					}

			}
		else
			{printf("command given isnt recognised try again\n");}//self explained


		if(line!=NULL){free(line);line=NULL;}//elefthrwnw thn grammi
		if(words!=NULL){free(words);words=NULL;}//elefthrwnw ta words
		}

	printf("freeing parent\n");

	for (int i = 0; i < numMonitors; ++i)
		{
		close(fd[1][i]);
		close(fd[0][i]);
		if(bloomhasharr[i]!=NULL)
			{
			hashtable_free(bloomhasharr[i]);
			}
		}
	remove_namelist(&namelist,numMonitors);
	free(bloomhasharr);
	remove_pipes(numMonitors);
	hashtable_free(cp);

	return 0;
	}