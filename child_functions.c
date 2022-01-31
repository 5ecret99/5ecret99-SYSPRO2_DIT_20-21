#include "child_functions.h"
void readdirectory(char* directory,int sizeOfBloom,hashtable* viruses,hashtable* records,hashtable* countries,hashtable* directories)
	{
	char citizenID[5];
	char firstName[30];
	char lastName[30];
	char country[30];
	char age[5];
	char virus_name[30];
	char yes_no[5];
	char date[15];

	char* words= strtok (directory," \0");
	int word_count;
	while(words!=NULL)
		{
		//opening the directory
		struct dirent *dirent;
		DIR *dir=opendir(words);
		if(dir)
			{
			while((dirent=readdir(dir))!=NULL)
				{
				//ftiaxnei to directory gia to ka8e arxeio
				if(strcmp(dirent->d_name,".")==0||strcmp(dirent->d_name,"..")==0){continue;}
				char filename[300]="";strcat(filename,words);strcat(filename,"/");strcat(filename,dirent->d_name);

				//anoigei to arxeio
				FILE* citizenRecordsFile=fopen(filename,"r");
				if (citizenRecordsFile==NULL){continue;}

				//isagei to sigkekrimeno directory ston hashtable
				char* directory=(char*) malloc((strlen(filename)+1)*sizeof(char));strcpy(directory,filename);
				unsigned long directory_hash=hash_i(directory,HVALUE);//HVALUE is a universal integer .i use for hashing located in hash.h
				bucket* dbucket=hashtable_insert(directories,directory_hash);
				if(dbucket==NULL){perror("error with hashtable bucket\n");}
				else if(dbucket->data!=NULL)//exei 3anaelextei ayto to directory-arxeio
					{
					//printf("checked again %s\n",(char*)dbucket->data);
					free(directory);fclose(citizenRecordsFile);continue;
					}
				else{dbucket->data=(void*)directory;}

				char* line=NULL;
				size_t len=0;
				int linecount=0;
				while(getline(&line,&len,citizenRecordsFile)!=EOF)
					{
					linecount++;
					strcpy(date,"");strcpy(virus_name,"");strcpy(country,"");
					sscanf(line,"%s %s %s %s %s %s %s %s",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);//breaking the line into words
					insertCitizenRecord(sizeOfBloom,viruses,records,countries,citizenID,firstName,lastName,country,age,virus_name,yes_no,date);
					}
				fclose(citizenRecordsFile);
				//printf("read file: %s with total lines %d\n",filename,linecount);
				}
			}
		words= strtok (NULL," \n\0");
		word_count++;
		}
	free(words);
	}

void insertCitizenRecord(int bloomSize,hashtable* viruses,hashtable* records,hashtable* countries,char* citizenID,char* firstName,char* lastName,char* country,char* age,char* virus_name,char* yes_no,char* date)
	{
	totalreq++;
	if(viruses==NULL||records==NULL||countries==NULL||citizenID==NULL||firstName==NULL||lastName==NULL||country==NULL||age==NULL||virus_name==NULL||yes_no==NULL){rejected++;printf("ERROR something is missing from this record cant insert");return;}
	int id=atoi(citizenID);
	int aa=atoi(age);
	int yn=-1;
	time_t dd=0;
	
	//checking if he is vaccinated
	if(strcmp(yes_no,"YES")==0)
		{
		yn=1;
		if(strcmp(date,"")==0)
			{
			rejected++;
			printf("date is yes but theres no date ERROR IN RECORD  %s %s %s %s %s %s %s %s \n",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);//theres isnt a  date next to a YES
			return;
			}
		dd=dateconverter(date);
		}
	else
		{
		yn=0;
		if(strcmp(date,"")!=0)
			{
			rejected++;
			printf("date is no but theres a date ERROR IN RECORD  %s %s %s %s %s %s %s %s \n",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);//theres a date next to a NO
			return;printf("error return command skipped\n");
			}
		}
	if (dd==-1)
		{
		rejected++;
		printf("date is wrong ERROR IN RECORD  %s %s %s %s %s %s %s %s \n",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);//date is wrong
		return;
		}
	unsigned long country_hash=hash_i(country,HVALUE);//HVALUE is a universal integer .i use for hashing located in hash.h
	char* country_data;
	bucket* cbucket=hashtable_insert(countries,country_hash);
	if(cbucket->data==NULL)
		{
		country_data=(char*) malloc(((int)strlen(country))*sizeof(char));//printf("%s with len %ld \n",country,strlen(country));
		strcpy(country_data,country);
		cbucket->data=country_data;
		}
	else
		{
		country_data=(char*) cbucket->data;
		if (strcmp(country_data,country)!=0)
			{
			rejected++;
			printf(" country ERROR IN RECORD  %s %s %s %s %s %s %s %s \n",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);
			return;printf("error return command skipped\n");
			}
		}

	record* rec;
	bucket* rbucket=hashtable_insert(records,id);
	if(rbucket->data==NULL)
		{
		rec=(record*)malloc(sizeof(record));
		rec->citizenID=id;
		rec->age=aa;
		rec->country=country_data;
		rec->firstName=(char*)malloc(strlen(firstName)*sizeof(char));strcpy(rec->firstName,firstName);
		rec->lastName=(char*)malloc(strlen(lastName)*sizeof(char));strcpy(rec->lastName,lastName);
		//printf("record :%d %s %s %s %d inserted succesful\n",id,rec->firstName,rec->lastName,rec->country,aa);
		rbucket->data=rec;
		}
	else
		{
		rec=(record*) rbucket->data;
		if(rec->age!=aa ||strcmp(rec->country,country_data)||strcmp(rec->firstName,firstName)!=0||strcmp(rec->lastName,lastName)!=0)
			{
			rejected++;
			printf("ERROR IN RECORD  %s %s %s %s %s %s %s %s \n",citizenID,firstName,lastName,country,age,virus_name,yes_no,date);
			return;printf("error return command skipped\n");
			}
		}

	bucket* vbucket=hashtable_insert(viruses,hash_i(virus_name,HVALUE));
	virus_child* vv_data;

	if(vbucket->data==NULL)
		{
		vv_data=(virus_child*) malloc(sizeof(virus_child));
		vv_data->virus_name=(char*)malloc(strlen(virus_name)*sizeof(char));strcpy(vv_data->virus_name,virus_name);
		vv_data->not_vaccinated_bloom=(bloom*) malloc(sizeof(bloom));bloom_init(vv_data->not_vaccinated_bloom,bloomSize);
		vv_data->vaccinated_persons=(skiplist*) malloc(sizeof(skiplist));skiplist_init(vv_data->vaccinated_persons);
		vv_data->not_vaccinated_persons=(skiplist*) malloc(sizeof(skiplist));skiplist_init(vv_data->not_vaccinated_persons);
		vbucket->data=vv_data;
		}
	else
		vv_data=(virus_child*) vbucket->data;

	if(yn==0)
		{
		if(skiplist_checking(vv_data->vaccinated_persons,id))
			{
			if(!skiplist_insert(vv_data->not_vaccinated_persons,id,rec,NULL)){rejected++;return;}
			}
		else
			{
			rejected++;return;
			}
		}
	else if(yn==1)
		{
		if(skiplist_checking(vv_data->not_vaccinated_persons,id))//if he is not in the not_vaccinated_persons skiplist 
			{
			time_t* day=(time_t*) malloc(sizeof(time_t));*day=dd;
			if(skiplist_insert(vv_data->vaccinated_persons,id,rec,day))
				{
				bloom_add(vv_data->not_vaccinated_bloom,citizenID);
				}
			else
				{
				free(day);
				rejected++;return;
				}
			}
		else
			{
			rejected++;return;
			}
		}
	accepted++;
	}
void sendblooms(int fda,hashtable* viruses,int bufferSize,int sizeOfBloom)
	{
	int virus_count=viruses->count;//printf("%d\n",virus_count );	
	char virus_countSTR[30];sprintf(virus_countSTR,"%d",virus_count);

	//stelnei ton ari8mo twn virus ston patera
	encode(fda,virus_countSTR,bufferSize);

	int bucket_num=viruses->bucket_num;

	while(bucket_num && virus_count)//ana virus
		{
		bucket_num--;
		bucket* vbucket=viruses->buckets[bucket_num];
		while(vbucket!=NULL)
			{
			if (vbucket->data!=NULL)
				{
				virus_child* vdata=(virus_child*) vbucket->data;
				//stelnei to onoma tu virus
				encode(fda,vdata->virus_name,bufferSize);
				
				//stelnei to bloomfilter des pipes.h/.c gia to encode_bloom
				encode_bloom(fda,vdata->not_vaccinated_bloom->buff,bufferSize,sizeOfBloom);
				}
			vbucket=vbucket->next;
			virus_count--;
			}
		}

	}
char* Child_travelRequest(hashtable* viruses,char* virusName,int citizenID,char* tdate)
	{
	char* rejected=malloc(sizeof("REQUEST REJECTED – YOU ARE NOT VACCINATED"));
	if(viruses==NULL|| virusName==NULL){return "REQUEST REJECTED – YOU ARE NOT VACCINATED";}

	bucket* vbucket=hashtable_check(viruses,hash_i(virusName,HVALUE));
	if(vbucket==NULL){return "REQUEST REJECTED – YOU ARE NOT VACCINATED";}
	if(vbucket->data==NULL){return "REQUEST REJECTED – YOU ARE NOT VACCINATED";}

	virus_child* vdata=(virus_child*) vbucket->data;
	skiplist* vaccinated_persons=vdata->vaccinated_persons;
	if (vaccinated_persons==NULL|| strcmp(virusName,(char*)vdata->virus_name)){return "REQUEST REJECTED – YOU ARE NOT VACCINATED";}
	return skiplist_req(vaccinated_persons,citizenID,tdate);//des sikiplis.h/.c
	}

void travelStats(hashtable* viruses,char* country,char* virusName,time_t a,time_t b,int fda,int bufferSize)
	{
	if(viruses==NULL||virusName==NULL){perror("viruses==NULL||virusName==NULL");return;}

	int country_flag=0;
	if(country==NULL){country_flag=1;}//elegxei an 8elei sigkekrimenh

	bucket* vbucket=hashtable_check(viruses,hash_i(virusName,HVALUE));
	if(vbucket==NULL || vbucket->data==NULL)
		{
		char buff[30];sprintf(buff,"%d",0);//stelnei ton ari8mo 0 ews accepted kai rejected ean den iparxun plhrofories gia ton io

		encode(fda,buff,bufferSize);

		encode(fda,buff,bufferSize);
		return;
		}

	int accepted_count=0;
	int rejected_count=0;

	virus_child* vdata=(virus_child*)vbucket->data;

	skiplist* positive_skiplist=vdata->vaccinated_persons;
	snode* positive_node=positive_skiplist->header;while(positive_node->below!=NULL){positive_node=positive_node->below;}
	skiplist* negative_skiplist=vdata->not_vaccinated_persons;
	snode* negative_node=negative_skiplist->header;while(negative_node->below!=NULL){negative_node=negative_node->below;}

	int max=positive_skiplist->size;if(negative_skiplist->size>max){max=negative_skiplist->size;}

	for (int i = 0; i < max; ++i)
		{
		if(positive_node->next!=NULL)//gia ka8e vaccinated
			{
			positive_node=positive_node->next;
			if(country_flag || strcmp(positive_node->rec->country,country)==0)//an ine  gia tin swsti xwra h den exei periorismo
				{
				if(date_between(a,*positive_node->date,b))//elegxei thn im/nia
					accepted_count++;
				else
					rejected_count++;
				}
			}
		
		if (negative_node->next!=NULL)//gia ka8e not vaccinated
			{
			negative_node=negative_node->next;
			if(country_flag || strcmp(negative_node->rec->country,country)==0)
				rejected_count++;
			}
		
		}
	//stelnei tus accepted
	char buff[30];sprintf(buff,"%d",accepted_count);
	encode(fda,buff,bufferSize);

	//stelnei tous rjected
	sprintf(buff,"%d",rejected_count);
	encode(fda,buff,bufferSize);
	}

void end_searchVaccinationStatus(int fda,int bufferSize)
	{

	encode(fda,"end_searchVaccinationStatus",bufferSize);

	return;
	}

void searchVaccinationStatus(hashtable* records,hashtable* viruses,int citizenID,int fda,int bufferSize)
	{
	if(records==NULL||viruses==NULL||citizenID>9999||citizenID<0){end_searchVaccinationStatus(fda,bufferSize);return;}
	bucket* rbucket=hashtable_check(records,citizenID);
	if(rbucket==NULL||rbucket->data==NULL){end_searchVaccinationStatus(fda,bufferSize);return;}
	record* rec=(record*)rbucket->data;
	if(rec->citizenID!=citizenID){end_searchVaccinationStatus(fda,bufferSize);return;}

	char buff[100];sprintf(buff,"%d ",rec->citizenID);
	strcat(buff,rec->firstName);strcat(buff," ");strcat(buff,rec->lastName);strcat(buff," ");
	strcat(buff,rec->country);

	encode(fda,buff,bufferSize);

	char age[8];sprintf(age,"AGE %d",rec->age);

	encode(fda,age,bufferSize);


	int virus_count=viruses->count;//printf("%d\n",virus_count );
	int bucket_num=viruses->bucket_num;

	while(bucket_num && virus_count)
		{
		bucket_num--;
		bucket* vbucket=viruses->buckets[bucket_num];
		while(vbucket!=NULL)
			{
			if (vbucket->data!=NULL)
				{
				virus_child* vdata=(virus_child*) vbucket->data;
				if (vdata->virus_name!=NULL && vdata->vaccinated_persons!=NULL && vdata->not_vaccinated_persons!=NULL)
					{
					char* str=skiplist_VaccinationStatus(vdata->vaccinated_persons,citizenID,vdata->virus_name);//des skiplist.h/.c
					if(str==NULL)
						str=skiplist_VaccinationStatus(vdata->not_vaccinated_persons,citizenID,vdata->virus_name);
					if (str!=NULL)
						{

						encode(fda,str,bufferSize);free(str);

						}
					}
				
				else{perror("hashtable  data  virus_child something is NULL error\n");}
				}
			vbucket=vbucket->next;
			virus_count--;
			}
		}

	end_searchVaccinationStatus(fda,bufferSize);
	}
