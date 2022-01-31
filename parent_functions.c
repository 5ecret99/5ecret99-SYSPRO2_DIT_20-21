#include "parent_functions.h"
void receiveblooms(int fd,int monitor,int bufferSize,int sizeOfBloom,hashtable** bloomhasharr)
	{
	
	//pernei twn ari8mo twn virus
	char*  virus_countSTR=decode(fd,bufferSize);

	int virus_count=atoi(virus_countSTR);free(virus_countSTR);

	//ean den yparxun epistrefei
	if(virus_count<1){return;}

	//an yparxei idh pinakas gia auto to paidi ton elefthrwnei
	if(bloomhasharr[monitor]!=NULL){ hashtable_free(bloomhasharr[monitor]);}

	bloomhasharr[monitor]=(hashtable*) malloc(sizeof(hashtable));
	hashtable_init(bloomhasharr[monitor],virus_count,1);

	while(virus_count)//gia ka8e virus
		{
		//printf("virus count %d monitor %d\n",virus_count,monitor);
		virus_count--;

		//lamvanei to onoma tu
		char*  virus_name=decode(fd,bufferSize);

		//ton  vazei ston hashtable
		bucket* vbucket=hashtable_insert(bloomhasharr[monitor],hash_i(virus_name,HVALUE));
		
		//pairnei to bloomfilter tou iou
		unsigned char*  buff=decode_bloom(fd,bufferSize,sizeOfBloom);

		//vazei tin swsti data sto bucket
		virusparent* vdata=(virusparent*) malloc(sizeof(virusparent));
		vdata->virus_name=virus_name;//printf("%s\n",virus_name);
		vdata->not_vaccinated_bloom=(bloom*) malloc(sizeof(bloom));
		//bloom_initb entolh gia initializing bloomfilter me etoimo bloom des bloomfilter.h/.c
		bloom_initb(vdata->not_vaccinated_bloom,buff,sizeOfBloom);
		vbucket->data=vdata;
		}
	}
/*
void travelRequest(char* countryFrom,char* virus_name,char* citizenID,char* date,hashtable* cp,hashtable** bloomhasharr,int bufferSize,pid_t* child,int** fd)
	{
	if(countryFrom==NULL||virus_name==NULL||citizenID==NULL||date==NULL||cp==NULL||bloomhasharr==NULL){perror("travelRequest something is NULL");return;}
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
							printf("alright\n");
							
							kill(child[monitor], SIGUSR2);
							
							encode(fd[1][monitor],"/travelRequest",bufferSize);

							
							encode(fd[1][monitor],virus_name,bufferSize);
	
							
							encode(fd[1][monitor],citizenID,bufferSize);
				
							encode(fd[1][monitor],date,bufferSize);
							printf("alright\n");

							char* message =decode(fd[0][monitor],bufferSize);close(fd);waiting();
							printf("%s\n",message);free(message);
							}
						}
					}

				}
			}
		}
	}*/