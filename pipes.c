#include "pipes.h"

void waiting()
	{
	int a=AVAL; while(a--);//des AVAL sto pipes.h
	}
void create_pipes(int numMonitors)
	{
	char PIPEA[50];
  	char PIPEB[50];

	for(int i = 0; i < numMonitors; i++)
		{
		//dio pipes ana monitor ena gia na stelnei o pateras sto pedi kai ena gia na stelnei to pedi ston patera
	    sprintf(PIPEA,"Parent2Child%d", i);
	    sprintf(PIPEB,"Child2Parent%d",i);
	    //elegxw an to mkfifo itan epityxes an oxi elegxw an to error itan epidh iparxei idi to sigkekrimeno pipe
		if((mkfifo(PIPEA, PERMS) < 0) && (errno != EEXIST)) {unlink(PIPEA);perror("can't create pipe");}
		//else{printf("named pipe: %s created\n",PIPEA);}

		if((mkfifo(PIPEB, PERMS) < 0) && (errno != EEXIST)) {unlink(PIPEB);perror("can't create pipe");}
		//else{printf("named pipe: %s created\n",PIPEB);}

		}
	}

void remove_pipes(int numMonitors)
	{
	char PIPEA[50];
  	char PIPEB[50];

	for(int i = 0; i < numMonitors; i++)
		{
		//kanw unlink ta dyo monitor tou ka8e paidiou
	    sprintf(PIPEA,"Parent2Child%d", i);
	    sprintf(PIPEB,"Child2Parent%d",i);

		if(remove(PIPEA)) {perror("can't remove pipe");}
		//else{printf("named pipe: %s removed\n",PIPEA);}

		if(remove(PIPEB)) {perror("can't remove pipe");}
		//else{printf("named pipe: %s removed\n",PIPEB);}
		}
	}
int encode(int fd,char* message,int bufferSize)
	{

	char msg[30]="";
	int message_len=strlen(message)+1;
	int msg_len=30;
	int flag=0;

	sprintf(msg,"%d",message_len);
	//arxika stelnw to mege80s tou minimatos
	//initialy sending the mesage length
	for (int i = 0; i < msg_len/bufferSize; ++i)
		{
		do{
		//domh poll gia na perimeni
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc == 1) && (fdarray [0].revents == POLLOUT))
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,msg+(i*bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag=1;}
    		}
    	else{ flag=1;}
    	}while(flag);
		}
	//oti emine pu den dierite akrivws me to buffersize to grafw twra
	if(msg_len%bufferSize)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc ==1) && ( fdarray [0]. revents == POLLOUT ) )
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,msg+(msg_len-msg_len%bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag=1;}
    		}
    	else{flag=1;}
    	}while(flag);
    	}
    //stelnw to char* minima
	//sending the actual message
	for (int i = 0; i < message_len/bufferSize; ++i)
		{

		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc == 1) && ( fdarray [0]. revents == POLLOUT ) )
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,message+(i*bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag=1;}
    		}
    	else{flag=1;}
    	}while(flag);

		}

	if(message_len%bufferSize)
		{
		do{	
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc ==1 ) && ( fdarray [0]. revents == POLLOUT ) )
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,message+(message_len-message_len%bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag=1;}
    		}
    	else{flag=1;}
    	}while(flag);
		}

	return 0;
	}

char* decode(int fd,int bufferSize)
	{
	if (fd==-1){perror("cant open pipe");}
	//printf("decode\n");
	char buffer[bufferSize];
	int message_len;
	char msg[30]="";
	int msg_len=30;
	int flag=0;

	//initialy receiving the message length
	for (int i = 0; i < msg_len/bufferSize; ++i)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc ==1) && ( fdarray [0]. revents == POLLIN ) )
    			{
   	 			if (fdarray [0]. fd == fd)
   	 				{
   	 				if(read(fd,buffer,bufferSize)<1){perror("couldnt read from pipe"); flag=1;}
   	 				else
						{
						memcpy(msg+(i*bufferSize),buffer,bufferSize);flag=0;
						}
   	 				}
   	 			else{perror("fdarray [0]. fd != fd\n");flag= 1;}
   	 			}
   	 	else{flag=1;}
   	 	}while(flag);
		}
	//oti emine pu den dierite akrivws me to buffersize to diavazw twra
	if(msg_len%bufferSize)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc ==1) && ( fdarray [0]. revents == POLLIN ) )
    			{
   	 			if (fdarray [0]. fd == fd)
   	 				{
   	 				if(read(fd,buffer, bufferSize)<1){perror("couldnt read from pipe");flag=1;}
   	 				else{memcpy(msg+(msg_len/bufferSize)*bufferSize,buffer,msg_len- (msg_len/bufferSize)*bufferSize);flag=0;}
   	 				}
   	 			else{perror("fdarray [0]. fd != fd\n");flag=1;}
   	 			}
   	 	else{flag=1;}
   	 	}while(flag);
		}

	message_len=atoi(msg);
	//now that we have the message length we create the message char array
	char *message=(char*) malloc(message_len*sizeof(char));

	//receiving the actual message
	for (int i = 0; i < message_len/bufferSize; ++i)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc ==1) && ( fdarray [0]. revents == POLLIN ) )
    			{
   	 			if (fdarray [0]. fd == fd)
   	 				{
   	 				if(read(fd,buffer,bufferSize)<1){perror("couldnt read from pipe");flag=1;}
					else{memcpy(message+(i*bufferSize),buffer,bufferSize);flag=0;}
   	 				}
   	 			else{perror("fdarray [0]. fd != fd\n");flag=1;}
   	 			}
   	 	else{flag=1;}
   	 	}while(flag);
		}

	if(message_len%bufferSize)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc ==1) )//&& ( fdarray [0]. revents == POLLIN ) )
    			{
   	 			if (fdarray [0]. fd == fd)
   	 				{
   	 				if(read(fd,buffer, bufferSize)<1){perror("couldnt read from pipe");flag=1;}
					else{memcpy(message+(message_len/bufferSize)*bufferSize,buffer,message_len- (message_len/bufferSize)*bufferSize);flag=0;}
   	 				}
   	 			else{perror("fdarray [0]. fd != fd\n");flag=1;}
   	 			}
   	 	else{flag=1;}
   	 	}while(flag);
		}

	return message;
	}



int encode_bloom(int fd,unsigned char* message,int bufferSize,int sizeOfBloom)
	{
	int flag=0;

    //sta bloom den ine angki na stilw to megethos mias kai ine sta8ero (sizeOfBloom)
	//sending the actual message
	for (int i = 0; i < sizeOfBloom/bufferSize; ++i)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc == 1) && ( fdarray [0]. revents == POLLOUT ) )
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,message+(i*bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag=1;}
    		}
    	else{flag=1;}
    	}while(flag);
		}

	if(sizeOfBloom%bufferSize)
		{		

		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd;
		fdarray [0]. events = POLLOUT;
		rc = poll( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return 1;}
		else if ( ( rc ==1 ) && ( fdarray [0]. revents == POLLOUT ) )
    		{
    		if ( fdarray [0]. fd == fd )
    			{
    			if(write(fd,message+(sizeOfBloom- sizeOfBloom %bufferSize), bufferSize)<0){perror("couldnt write on pipe");flag=1;}
    			else{flag=0;}
    			}
    		else{perror("fdarray [0]. fd != fd\n");flag= 1;}
    		}
    	else{flag=1;}
    	}while(flag);
		}

	return 0;
	
	}
unsigned char* decode_bloom(int fd,int bufferSize,int sizeOfBloom)
	{
	int flag=0;
	unsigned char *message=(unsigned char*) calloc(sizeOfBloom,sizeof(unsigned char));
	unsigned char unbuffer[bufferSize];
	//receiving the actual message
	for (int i = 0; i < sizeOfBloom/bufferSize; ++i)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc == 1) && ( fdarray [0]. revents == POLLIN ) )
    		{
   	 		if (fdarray [0]. fd == fd)
   	 			{
   	 			if(read(fd,unbuffer,bufferSize)<1){perror("couldnt read from pipe bloom");flag=1;}
				else{memcpy(message+(i*bufferSize),unbuffer,bufferSize);flag=0;}
   	 			}
   	 		else{perror("fdarray [0]. fd != fd\n");flag=1;}
   	 		}
   	 	else{flag=1;}
   	 	}while(flag);
		}

	if(sizeOfBloom%bufferSize)
		{
		do{
		struct pollfd fdarray [1];
		int rc;
		fdarray [0]. fd = fd ;
		fdarray [0]. events = POLLIN ;
		rc = poll ( fdarray , 1 , WTIME);
		if ( rc == 0) {perror("Poll timed - out\n");return NULL;}
		else if ( ( rc == 1) && ( fdarray [0]. revents == POLLIN ) )
    		{
   	 		if (fdarray [0]. fd == fd)
   	 			{
   	 			if(read(fd,unbuffer, bufferSize)<1){perror("couldnt read from pipe bloom");flag=1;}
				else{memcpy(message+(sizeOfBloom/bufferSize)*bufferSize,unbuffer,sizeOfBloom- (sizeOfBloom/bufferSize)*bufferSize);flag=0;}
   	 			}
   	 		else{perror("fdarray [0]. fd != fd\n");flag=1;}
   	 		}
   	 	else{flag=1;}
   	 	}while(flag);
		}

	return message;
	
	}

/*
int main(int argc, char const *argv[])
	{
	create_pipes(2);
	if(fork()==0)
		{
		//printf("fork==0\n");
		int fd=open("Parent2Child0",O_WRONLY);
		encode(fd,"everything is alright?",2);
		close(fd);int a=99999; while(a--);

		//sleep(1);
		fd=open("Parent2Child0",O_WRONLY);
		encode(fd,"duuuuuude?",2);
		close(fd);

		fd=open("Child2Parent0",O_RDONLY);
		char* message=decode(fd,2);close(fd);
		printf("%s\n",message);
		free(message);
		}
	else
		{
		sleep(5);
		int fd=open("Parent2Child0",O_RDONLY);
		char* message=decode(fd,2);close(fd);
		printf("%s\n",message);
		free(message);

		fd=open("Parent2Child0",O_RDONLY);
		message=decode(fd,2);//close(fd);
		printf("%s\n",message);
		free(message);

		//sleep(1);
		fd=open("Child2Parent0",O_WRONLY);
		encode(fd,"yeah dude everything is awesomeeeeee!!!!!!!!!!!!!!!!!!!",2);
		close(fd);
		remove_pipes(2);
		}

	//printf("done process %d\n",getpid());
	return 0;
	}*/


