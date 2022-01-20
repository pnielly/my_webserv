/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 01:52:16 by user42            #+#    #+#             */
/*   Updated: 2021/12/20 17:14:51 by pnielly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#define PORT 8080

int main(int ac, char const *av[])
{
	(void)ac; (void)av;
	int sock = 0; long valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	// create sock
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	//set sockaddr_in
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 address from text to binary form
	if((serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1")) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	// try to connect client to server sock
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	// once connected: send string to server
	send(sock , "hello from client" , strlen("hello from client") , 0 );
	printf("Hello message sent\n");
	// once connected: receive string from server
	valread = recv( sock , buffer, 1024, 0);
	printf("%s\n",buffer );
	return 0;
}
