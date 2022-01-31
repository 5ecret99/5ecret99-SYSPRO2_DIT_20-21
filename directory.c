#include "directory.h"

int dir(char*** namelist,hashtable** cp,char* input_dir,int numMonitors)
	{

	char** nslst=(char**) malloc(numMonitors*sizeof(char*));
	struct dirent **dnamelist;

	int numCountries=scandir(input_dir, &dnamelist, 0, alphasort)-2;
	int monitor=0;
	if(numMonitors>numCountries){numMonitors=numCountries;}//if the monitors are more than the countries then we assume they are the same number
	hashtable* tcp=(hashtable*) malloc(sizeof(hashtable));//hashtable used for the parent process to know witch country is in each monitor
	hashtable_init(tcp,numCountries,-1);

	for (int i = 0; i < numMonitors; ++i)
		{
		nslst[i]=(char*)malloc((numCountries/numMonitors +1)*70*sizeof(char));//malloc  the (numCountries/numMonitors +1)*50 gives rough estimate for the size needed
		monitor=i;if(i>=numCountries){strcpy(nslst[i],"");}
		while(monitor<numCountries)
			{
			if (monitor!=i){strcat(nslst[i]," ");strcat(nslst[i],input_dir);}//xwrizw ta directories meta3i touw me " "(space) kai arxika vazw to input_dir
			else{strcpy(nslst[i],input_dir);}
			
			strcat(nslst[i],"/");
			strcat(nslst[i],dnamelist[monitor+2]->d_name);//vazw /(xwra) ara to teliko apotelesma tha ine /input_dir/country1 ./input_dir/country2 .....

			//vazw ston hashtable mu to (country,monitor) me hashtimi hash_i(cmdata->country,HVALUE)
			cm* cmdata=(cm*) malloc(sizeof(cm));
			strcpy(cmdata->country,dnamelist[monitor+2]->d_name);
			cmdata->monitor=i;

			bucket* tcp_bucket=hashtable_insert(tcp,hash_i(cmdata->country,HVALUE));//HVALUE einai akeraios pou xrisimopoiw gia to hashing kai vriskete sto hash.h
			tcp_bucket->data=cmdata;
			
			monitor+=numMonitors;
			}
		printf("monitor%d:\n%s\n\n",i,nslst[i]);//typwnw to monitor kai tis xwres tou
		}
	*namelist=nslst;
	*cp=tcp;
	for (int i = 0; i < numCountries+2; i++) 
			{
            //printf("removing dnamelist[%d] : %s\n",i,dnamelist[i]->d_name);
            free(dnamelist[i]);
            }
    free(dnamelist);
    return numCountries;//epistrefw tonn ari8mo xwrwn
	}
void remove_namelist(char*** namelist,int numMonitors)
	{
	//printf("deleting namelist\n");
	char** nslst=*namelist;
	for (int i = 0; i < numMonitors; ++i)
		{
		//printf("freeing namelist[%d] :%s\n",i,nslst[i]);
		free(nslst[i]);//gia ka8e monitor eleftherwnw to char* directory tu
		}
	free(nslst);
	}
/*
int main()
	{
	char** namelist;
	hashtable* cp;
	int numMonitors=4;
	printf("countries count:%d\n",dir(&namelist,&cp,"./countries",numMonitors ));
	
	for (int i = 0; i < numMonitors; ++i)
		{
		printf("directories for monitor %d %s\n",i,namelist[i]);
		}
	bucket* bb=hashtable_check(cp,hash_i("cyprus",HVALUE));
	cm* data=(cm*) bb->data;
	printf("country :%s in monitor: %d \n",data->country,data->monitor);
	remove_namelist(&namelist,numMonitors);
	hashtable_free(cp);
	}
*/