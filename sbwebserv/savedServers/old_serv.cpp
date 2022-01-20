/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 01:46:41 by user42            #+#    #+#             */
/*   Updated: 2021/12/21 15:56:09 by pnielly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#define PORT 8080

int main(int ac, char const *av[])
{
	(void)ac; (void)av;
	int server_fd, new_socket, max_sd; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);


	// create socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	//allow socket descriptor to be reuseable
	int on = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		perror("In setsockopt");
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

	fd_set	master_set, working_set;

	FD_ZERO(&master_set);
	max_sd = server_fd;
	FD_SET(server_fd, &master_set);

	while(1)
	{
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");

		// get a copy of master fd_set (because select is destructive)
		memcpy(&working_set, &master_set, sizeof(master_set));

		printf("Waiting on select()...\n");
		//call select()
		if ((int rc = select(max_sd + 1, &working_set, NULL, NULL, NULL)) < 0)
		{
			perror("select() failed");
			break ;
		}

		int check = rc;
		int new_sd = 1;
		for (int i = 0; i <= max_sd && check > 0; ++i)
		{
			// check if the fd is ready (if it belongs to the set)
			if (FD_ISSET(i, &working_set))
			{
				check--;
				// this is the listening socket (server sock)
				if (i == server_fd)
				{
					printf("Listening socket is readable\n");
					while (new_sd != -1)
					{
						//accept each incoming connection
						new_sd = accept(server_fd, NULL, NULL);
						if (new_sd < 0)
						{
							perror("accept error");
							break ;
						}

						// add new connection to master read set
						printf("New incoming connection - %d\n", new_sd);
						FD_SET(new_sd, &master_set);
						if (new_sd > max_sd)
							max_sd = new_sd;
					}
				}
				// not the server socket
				else
				{
					printf("Descriptor %d is readable\n", i);
					while (1)
					{
						if ((recv(i, buffer, sizeof(buffer), 0)) <= 0)
						{
							perror("in recv\n");
							break ;
						}
						printf("Message received is: %s\n", buffer);
						if ((send(i, buffer, len, 0)) < 0)
						{
							perror("send failed\n");
							break ;
						}
					}
					close(i);
					FD_CLR(i, &master_set);
					if (i == max_sd)
					{
						while (FD_ISSET(max_sd, &master_set) == FALSE)
							max_sd -= 1;
					}
				}
			}
		}
/**
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
		char * message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		write(new_socket , message , strlen(message));
		printf("------------------Hello message sent-------------------\n");

		close(new_socket);
	}**/
	return 0;
}

// Note: read more about recv flags
