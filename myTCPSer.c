/*
Autor:Kevin
Create date:2019-12-21
*/

#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define SERVER_IP 	"192.168.124.133"	/*SERVER_PORT*/
#define SERVER_PORT 8888
#define BACK_LOG 10



int main(int argc, char *argv[])
{
    int serSocketFd = 0;
	int cliSocketFd = 0;
	struct sockaddr_in serAddr;
	struct sockaddr_in cliAddr;
	int iRet = 0;
	int lenSocketAddr = sizeof(struct sockaddr);
	int cliCounter = 0;
	char recvBuf[1024]="";
	char sendBuf[1024]="";
	int recvLen = 0;
	char *cmd = "ps -C ps au";
	char cmdOutBuf[1204]="";
	
	FILE *fp = NULL;

	fp = popen(cmd, "r");
	if (NULL == fp)
	{
		perror("popen fail!\n"); 
		exit(0);
	}

	if ((fgets(cmdOutBuf, strlen(cmdOutBuf), fp)) == NULL)
	{
		printf("cmdOutBuf:%s\n",cmdOutBuf);
	 	pclose(fp);
		//exit(0);
	}
	else
	{
		//int count = atoi(cmdOutBuf); 
		strcpy(sendBuf, cmdOutBuf);
		pclose(fp);  
	}

	if(daemon(1, 1) < 0)
	{
		perror("error daemon.../n");
		exit(1);
	}	
	
    serSocketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == serSocketFd)
  	{ 
	  perror("Socket create fail!\n"); 
	  exit(0);
  	}
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(SERVER_PORT);

	iRet = inet_pton(AF_INET, SERVER_IP, &serAddr.sin_addr.s_addr);
	if ( 1 > iRet)
	{ 
	  perror("inet_pton  fail!\n");
	  exit(0);
	}
	//serAddr.sin_addr.s_addr = SERVER_IP;
	memset(serAddr.sin_zero, 0, 8);
		
    iRet = bind(serSocketFd, (const struct sockaddr*)&serAddr,sizeof(struct sockaddr));
	if ( -1 == iRet)
	{ 
	  perror("Socket address binding fail!\n");
	  exit(0);
	}
	iRet = listen(serSocketFd,BACK_LOG);
	if ( -1 == iRet)
	{ 
	  perror("Listen initial  fail!\n");	
	  exit(0);
	}	

	while(1)
	{
		cliSocketFd = accept(serSocketFd, (struct sockaddr *)&cliAddr, &lenSocketAddr);
		if (-1 == cliSocketFd)
		{
			perror("accept fail!\n");	
			exit(0);
		}
		cliCounter++;
		printf("Get connection from client %d:%s\n", cliCounter, inet_ntoa(cliAddr.sin_addr));
		if(!fork())
		{
			while(1)
			{
				recvLen =  recv(cliSocketFd, recvBuf, 1000, 0);
				if (recvLen <= 0)
				{
					close(cliSocketFd);
					exit(0);
				}
				else
				{
					recvBuf[recvLen] = '\0';
                    printf("Get Msg from client %d:%s\n", cliCounter, recvBuf);
					iRet = send(cliSocketFd, sendBuf, strlen(sendBuf), 0);
					if (iRet == -1)
					{
						perror("send fail!\n");	
						exit(0);
					}				
				}
			}
		}

	}
	close(serSocketFd);
	exit(0);
}




