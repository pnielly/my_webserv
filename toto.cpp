/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toto.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 10:13:08 by user42            #+#    #+#             */
/*   Updated: 2021/12/20 11:14:38 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define MAXLINE 80
#define PORT 80
#define MAX_LINE 4096
#define SA struct sockaddr

int	main(int ac, char **av)
{
	int sock, n;
	int sendbytes;
	struct sockaddr_in servaddr;
	char	sendline[MAXLINE];
	char	recvline[MAXLINE];

	if (ac != 2)
		exit(EXIT_FAILURE);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error");
		exit(EXIT_FAILURE);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	if (inet_pton(AF_INET,av[1], &servaddr.sin_addr) <= 0)
	{
		printf("inet_pton failed");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (SA *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("connect failed");
		exit(EXIT_FAILURE);
	}

	sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
	sendbytes = strlen(sendline);

	if (write(sock, sendline, sendbytes) != sendbytes)
	{
		printf("write failed");
		exit(EXIT_FAILURE);
	}

	memset(recvline, 0, MAXLINE);
	while ((n = read(sock,recvline, MAXLINE - 1)) > 0)
	{
		printf("%s", recvline);
	}

	if (n < 0)
	{
		printf("read failed");
		exit(EXIT_FAILURE);
	}
	exit(0);
}
