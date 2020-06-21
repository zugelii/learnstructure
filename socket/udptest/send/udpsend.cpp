#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define BUFLEN 2550

int main(int argc, char **argv)
{
	struct sockaddr_in peeraddr, myaddr;
 
	int sockfd;
	char recmsg[BUFLEN + 1];
	unsigned int socklen;
 
	/* 创建 socket 用于UDP通讯 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket creating error\n");
		exit(1);
	}

	   // set socket reuse
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse)))
	{
		perror("Error setting socket option SO_REUSEADDR");
	}
      
	socklen = sizeof(struct sockaddr_in);
	
	unsigned int loop = 1;
	// 参数 loop 设置为 0 禁止回送，设置为 1 允许回送。
	if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)))
	{
		perror("Error setting socket option IP_MULTICAST_LOOP");
	}
	
 #if 0
	/* 设置对方的端口和IP信息 */
	memset(&peeraddr, 0, socklen);
	peeraddr.sin_family = AF_INET;
	/*
	if (argv[2])
		peeraddr.sin_port = htons(atoi(argv[2]));
	else
	*/
		peeraddr.sin_port = htons(10000);
		/* 注意这里设置的对方地址是指组播地址，而不是对方的实际IP地址 */
		/*
	if (argv[1]) {
		
		printf("argv[1]:%s\n", argv[1]);
		if (inet_pton(AF_INET, argv[1], &peeraddr.sin_addr) <= 0) {
			printf("wrong group address!\n");
			exit(0);
		}
	} else {
		printf("no group address!\n");
		exit(0);
	}
	*/
	peeraddr.sin_port = htons(10000);
	if (inet_pton(AF_INET, argv[1], &peeraddr.sin_addr) <= 0) 
	{
		printf("wrong group address!\n");
		exit(0);
	}	
 
	/* 设置自己的端口和IP信息 */
	memset(&myaddr, 0, socklen);
	myaddr.sin_family = AF_INET;
	/*
	if (argv[4])
		myaddr.sin_port = htons(atoi(argv[4]));
	else
	*/
		myaddr.sin_port = htons(14000);
 /*
	if (argv[3]) 
	{
		if (inet_pton(AF_INET, argv[3], &myaddr.sin_addr) <= 0) {
			printf("self ip address error!\n");
			exit(0);
		}
	} else
	*/
#endif

	/* 设置自己的端口和IP信息 */
	memset(&myaddr, 0, socklen);
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(10000);
	myaddr.sin_addr.s_addr = INADDR_ANY;
 
	/* 绑定自己的端口和IP信息到socket上 */
	if (bind(sockfd, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in)) == -1) 
	{
		printf("Bind error\n");
		exit(0);
	}
	
	/* 设置对方的端口和IP信息 */
	memset(&peeraddr, 0, socklen);
	peeraddr.sin_family = AF_INET;	
	peeraddr.sin_port = htons(10000);
	if (inet_pton(AF_INET, "239.0.0.2", &peeraddr.sin_addr) <= 0) 
	{
		printf("wrong group address!\n");
		exit(0);
	}	
	int n = 0;
	/* 循环接受用户输入的消息发送组播消息 */
	for (;;) 
	{
		/* 接受用户输入 */
		bzero(recmsg, BUFLEN + 1);
		if (fgets(recmsg, BUFLEN, stdin) == (char *) EOF)
		{
			exit(0);
		}			
		/* 发送消息 */
		if (sendto(sockfd, recmsg, strlen(recmsg), 0, (struct sockaddr *) &peeraddr, sizeof(struct sockaddr_in)) < 0) 
		{
			printf("sendto error!\n");
			exit(3);
		}
		printf("'%s' send ok\n", recmsg);
	}

}
