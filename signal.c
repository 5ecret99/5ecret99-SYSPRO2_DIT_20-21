#include "signal.h"
void logfile(int process_id,hashtable* countries,int totalreq,int accepted,int rejected,int parent)
	{
	char filename[100];sprintf(filename,"./log_files/log_file.%d",process_id);

	FILE *fp;
	fp = fopen (filename, "w");
	int bucket_num=countries->bucket_num;
	int countries_count=countries->count;
	while(bucket_num && countries_count)//pigenei se mia mia xwra ston hashtable
		{
		bucket_num--;
		bucket* cbucket=countries->buckets[bucket_num];
		while(cbucket!=NULL)
			{
			countries_count--;
			if(cbucket->data!=NULL)
				{
				char* country=NULL;
				if(parent)
					{
					cm* cmdata=(cm*)cbucket->data;
					country=cmdata->country;//ston patera apo8ikevw kai tin xwra kai ton ari8mo tou monitor pu tis antistoixei
					}
				else
					{
					country=(char*)cbucket->data;//enw sto hashtable tu paidiou apo8ikevw mono tin xwra
					}
				fputs(country,fp);fputs("\n",fp);
				}
			cbucket=cbucket->next;
			}
		}

	char buff[100];
	sprintf(buff,"TOTAL TRAVEL REQUESTS	%d\n",totalreq);
	fputs(buff,fp);
	sprintf(buff,"ACCEPTED %d\n",accepted);
	fputs(buff,fp);
	sprintf(buff,"REJECTED %d\n",rejected);
	fputs(buff,fp);
	fclose(fp);
	}

/*
int main(int argc, char const *argv[])
	{
	totalreq=69;
	accepted=69;
	rejected=69;
	char directory[100];strcpy(directory,"./countries/cyprus ./counyy/GReece ./cdjnd/uk");
	logfile(69,directory);
	return 0;
	}*/