#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

#include<unistd.h>
#include<fcntl.h>
 
#define BUFLEN 255

int main(int argc, char **argv)
{
	struct sockaddr_in peeraddr;
	struct in_addr ia;
	int sockfd;
	char recmsg[BUFLEN + 1];
	unsigned int socklen, n;
	struct hostent *group;
	struct ip_mreq mreq;
	
	/* 创建 socket 用于UDP通讯 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	   // set socket reuse
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse)))
	{
		perror("Error setting socket option SO_REUSEADDR");
	}
      
	if (sockfd < 0) 
	{
		printf("socket creating err in udptalk\n");
		exit(1);
	}
	
	/* 设置要加入组播的地址 */
	bzero(&mreq, sizeof(struct ip_mreq));
	if ((group = gethostbyname("239.0.0.2")) == (struct hostent *) 0) 
	{
		perror("gethostbyname");
		exit(errno);
	}
	bcopy((void *) group->h_addr, (void *) &ia, group->h_length);
	/* 设置组地址 */
	bcopy(&ia, &mreq.imr_multiaddr.s_addr, sizeof(struct in_addr));
	
	/* 设置发送组播消息的源主机的地址信息 */
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	/* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */
	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) == -1) 
	{
		perror("ADD membership error");
		exit(-1);
	}
	
	socklen = sizeof(struct sockaddr_in);
	memset(&peeraddr, 0, socklen);
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(10000);
	if (inet_pton(AF_INET, "239.0.0.2", &peeraddr.sin_addr) <= 0) 
	{
		printf("Wrong dest IP address!\n");
		exit(0);
	}
	
	/* 绑定自己的端口和IP信息到socket上 */
	if (bind(sockfd, (struct sockaddr *) &peeraddr,	sizeof(struct sockaddr_in)) == -1) 
	{
		printf("Bind error\n");
		exit(0);
	}
	//int fd = open("rev.log", O_CREAT | O_RDWR, 0666);
	/* 循环接收网络上来的组播消息 */
	for (;;) 
	{
		bzero(recmsg, BUFLEN + 1);
		n = recvfrom(sockfd, recmsg, BUFLEN, 0,	(struct sockaddr *) &peeraddr, &socklen);
		if (n < 0) 
		{
			printf("recvfrom err in udptalk!\n");
			exit(4);
		} else 
		{
			/* 成功接收到数据报 */
			printf("\npeer rev len: %d, content:\n", n);
			
			for(int i = 0; i < n; i++)
			{
				printf("%c", recmsg[i]);
			}
		/*	
			if((recmsg[26] == 'c') && (recmsg[27] == 'a') && (recmsg[28] == 'm') && (recmsg[29] == 'e') && (recmsg[30] == 'r') && (recmsg[31] == 'a') )
			{
				printf("get camera msg. len:%d\n", n);
				write(fd, recmsg, n + 1);
			}*/
			//printf("\n\n");
		}
	}

}