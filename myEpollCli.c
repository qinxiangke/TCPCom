/*
Autor:Kevin
Date:2019-12-21
*/

#include <sys/types.h>

#include <sys/socket.h>

#include <stdio.h>

#include <string.h>

#include <arpa/inet.h>

//这里配置服务器的ip地址和端口号

#define SERVER_LISTEN_PORT 6000

#define SERVER_HOST "192.168.124.133"

 

#define  MAX_BUF_SIZE 1024

#define INVALID_SOCKET -1

 

//返回值的宏定义，在开发linux环境时，用0表示成功

#define RES_SUC 0

#define RES_FAIL -1

 

int main()

{

	//创建一个客户端socket

	int client_socket = INVALID_SOCKET;

	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(INVALID_SOCKET == client_socket)

	{

		printf("[main] create client socket fail!\n");

		return RES_FAIL;

	}

	//创建一个服务器端的地址结构体，用于存储socket要连接的服务器地址信息

	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);  // 指定服务程序的ip地址

    serv_addr.sin_port = htons(SERVER_LISTEN_PORT); //指定服务程序的端口

	//与服务器程序建立tcp连接

	if(connect(client_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == RES_FAIL)

	{

		printf("[main] connect to server %s:%d fail!\n", SERVER_HOST, SERVER_LISTEN_PORT);

		return RES_FAIL;

	}

	printf("[main] connect to server %s:%d suc!\n", SERVER_HOST, SERVER_LISTEN_PORT);

	//发送一条消息

	char write_buf[MAX_BUF_SIZE];

	memset(write_buf, '\0', MAX_BUF_SIZE);

	snprintf(write_buf, MAX_BUF_SIZE, "hello");

	while(1)
	{
	
		if(write(client_socket, write_buf, MAX_BUF_SIZE) <= 0)
		
			{
		
				printf("[main] write msg into socket:%d fail! content:[%s]!\n", client_socket, write_buf);
		
				return RES_FAIL;
		
			}
		
			printf("[main] send msg to server %s:%d suc!\n", SERVER_HOST, SERVER_LISTEN_PORT);
		
			//等待服务端的返回消息
		
			char read_buf[MAX_BUF_SIZE];
		
			memset(read_buf, '\0', MAX_BUF_SIZE);
		
			if (read(client_socket, read_buf, MAX_BUF_SIZE) > 0)
		
			{
		
				printf("[main] recv responsed msg from socket:%d suc! content: %s\n", client_socket, read_buf);
		
				//return RES_SUC;
		
			}
		
			else
		
			{
		
				printf("[main] recv responsed msg from socket:%d fail!\n", client_socket);
		
				//return RES_FAIL;
		
			}
	}
	

}

