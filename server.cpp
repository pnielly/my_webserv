/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 01:46:41 by user42            #+#    #+#             */
/*   Updated: 2021/12/20 02:09:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#define PORT 8080

int main(int ac, char const *av[])
{
	(void)ac; (void)av;
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);


	// create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// set sockaddr_in
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	// name the socket (assign transport address)
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}

	// wait for incoming connection
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");

		// grab 1st connection from queue (set up in listen) and create new socket for it
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		// read from new_socket
		char buffer[30000] = {0};
		valread = recv( new_socket , buffer, 30000, 0);
		printf("%s\n",buffer );

		// write into new_socket
		write(new_socket , "hello from server" , strlen("hello from server"));
		printf("------------------Hello message sent-------------------\n");

		close(new_socket);
	}
	return 0;
}

// Note: read more about recv flags
