#include <stdio.h>
#include <time.h>
#include "records.h"
#include <stdlib.h>
#include <string.h>
time_t dateconverter(char* date)
	{
	char* i=date;
	int day=0;
	while(*i!='-' && *i!='/')
		{
		day=day*10+*i-'0';
		i++;
		if (day>31||day<0){return -1;}
		}
	if (day==0){return -1;}
	i++;;
	int month=0;
	while(*i!='-' && *i!='/')
		{
		month=month*10+*i-'0';
		i++;
		if (month>12||month<0){return -1;}
		}
	if (month==0){return -1;}
	i++;
	int year=0;
	while(*i!='\0'&& *i!='\0')
		{
		year=year*10+*i-'0';
		i++;
		if(year>9999||year<0){return -1;}
		}
	if (year==0){return -1;}
	struct tm tmdate = {0};
	tmdate.tm_mday =day;
	tmdate.tm_mon = month-1;
	tmdate.tm_year = year-1900;
	tmdate.tm_isdst = -1;
	time_t t=mktime( &tmdate);
	return t;
	}
time_t dateconverterplus(char* date)//ftiaxnei tin imerominia +6 mhnes
	{
	char* i=date;
	int day=0;
	while(*i!='-')
		{
		day=day*10+*i-'0';
		i++;
		if (day>31||day<0){return -1;}
		}
	if (day==0){return -1;}
	i++;;
	int month=0;
	while(*i!='-')
		{
		month=month*10+*i-'0';
		i++;
		if (month>12||month<0){return -1;}
		}
	if (month==0){return -1;}
	month=month+6;
	int yearp=month/12;
	month=month%12;
	i++;
	int year=0;
	while(*i!='\0'&& *i!='\0')
		{
		year=year*10+*i-'0';
		i++;
		if(year>9999||year<0){return -1;}
		}
	if (year==0){return -1;}
	year+=yearp;
	struct tm tmdate = {0};
	tmdate.tm_mday =day;
	tmdate.tm_mon = month-1;
	tmdate.tm_year = year-1900;
	tmdate.tm_isdst = -1;
	time_t t=mktime( &tmdate);
	return t;
	}
int date_between(time_t a,time_t b ,time_t c)
	{
	if(b>=a && b <= c)
		return 1;
	return 0;
	}
char* timetconverter(time_t* date)
	{
	char* str =(char*) malloc(21*sizeof(char));
	strftime(str, 20, "%d-%m-%Y", localtime(date));
	return str;
	}
time_t currdate()
	{
	time_t curr=time(NULL);
	return curr;
	}
int sixmonths(char* datev,char* datef)
	{
	time_t dvplus=dateconverterplus(datev);
	time_t dv=dateconverter(datev);
	time_t df=dateconverter(datef);
	if(dv==-1||df==-1){perror("date =-1\n");return 0;}
	if (dv>df){return 0;}//8a kanei vacciante meta tin ptisi opota oxi
	if (df<=dvplus){return 1;}//ekane vaccinate sto diasthma 6 mhnwn prin thn pthsh
	return 0;
	}
/*
int main()
	{
		
	char* date="27-11-2021";
	int day= 10*((int) date[0]-'0') +(int) date[1]-'0';
	printf("%d\n",day);

	int month= 10*((int) date[3]-'0') +(int) date[4]-'0';
	printf("%d\n",month);

	int year=1000*((int) date[6]-'0')+ 100*((int) date[7]-'0')+10*((int) date[8]-'0') +(int) date[9]-'0';
	printf("%d\n",year);
	
	date="27-11-2021";
	time_t a=dateconverter(date);
	char* str=timetconverter(&a);
	printf("%s %s\n",date,str);
	date="28-11-2021";
	time_t b=dateconverter(date);

	date="29-12-2021";
	time_t c=dateconverter(date);
	printf("%ld %ld %ld\n",a,b,c);
	if (date_between(a,b,c))
		{
		printf("date_between succesful\n");
		}
	else
		{
		printf("date_between unsuccesful\n");
		}
	if(sixmonths("15-01-2021","14-01-2021")){printf("date less than sixmonths\n");}
	return 0;
	}
*/
