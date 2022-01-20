/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 01:46:41 by user42            #+#    #+#             */
/*   Updated: 2021/12/21 16:00:35 by pnielly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#define PORT 8080

int setup_socket(int *server_fd, int *on, struct sockaddr_in address)
{
	// create socket
	if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		return -1;
	}

	//allow socket descriptor to be reuseable
	if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		perror("In setsockopt");
		return -1;
	}
	// name the socket (assign transport address)
	if (bind(*server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("In bind");
		return -1;
	}
	return 0;

}

int	ft_listen(int server_fd, struct sockaddr_in address)
{
	int new_socket;
	int addrlen = sizeof(address);

	// wait for incoming connection
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	
	fd_set	master_set, working_set;

	FD_ZERO(&master_set);
	int max_sd = server_fd;
	FD_SET(server_fd, &master_set);
	
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
		long valread;
		valread = recv( new_socket , buffer, 30000, 0);
		printf("%s\n",buffer );

		// write into new_socket
		char * message = "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: 33117\n\n";
		write(new_socket , message , strlen(message));
		
		//write html file content
		char *line;
		FILE *file;
		size_t	n = 0;

		file = fopen("files/penguin.png", "r");
		while (getline(&line, &n, file) > 0)
			write(new_socket, line, strlen(line));


		printf("------------------Hello message sent-------------------\n");

		close(new_socket);
	}
}

int main(int ac, char const *av[])
{
	(void)ac; (void)av;
	int server_fd = 0, on = 0;
	struct sockaddr_in address;

	// set sockaddr_in
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	memset(address.sin_zero, '\0', sizeof address.sin_zero);

	// create and name socket
	if (setup_socket(&server_fd, &on, address) < 0)
		exit(EXIT_FAILURE);

	//listen
	if (ft_listen(server_fd, address) < 0)
		exit(EXIT_FAILURE);

	return 0;
}

// Note: read more about recv flags
