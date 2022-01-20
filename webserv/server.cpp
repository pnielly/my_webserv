/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 01:46:41 by user42            #+#    #+#             */
/*   Updated: 2021/12/21 15:20:03 by pnielly          ###   ########.fr       */
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

int ft_select(fd_set *working_set, fd_set *master_set, int max_sd, struct timeval timeout)
{
	int ret;

	// copy master fd_set over to the working fd_set
	memcpy(working_set, master_set, sizeof(*master_set));
	printf("Waiting on select()...\n");
	// call select()
	ret = select(max_sd + 1, working_set, NULL, NULL, &timeout);

	if (ret < 0)
	{
		perror("select() failed\n");
		return -1 ;
	}
	else if (!ret)
	{
		printf("select() timed out. End program.\n");
		return -1 ;
	}
	return ret;
}

int server_sock_accept(int server_fd, fd_set *master_fd)
{
	// adress and length are NULL because ???
	int new_sd = accept(server_fd, NULL, NULL);
	if (new_sd > 0)
	{
		printf("New incoming connection - %d\n", new_sd);
		FD_SET(new_sd, master_set);
	}
	return (new_sd);
}

// ft_find_readable_fd(): one or more fd are readable. Will determine which ones
ft_find_readable_fd(int ret, fd_set *working_set, fd_set *master_set, int server_fd, int max_sd)
{
	int new_sd; 

	for (int i = 0; i <= max_sd && ret > 0; i++)
	{
		//check if the fd is ready (if it belongs to working_set)
		if (FD_ISSET(i, &working_set))
		{
			rec--;
			// check if it is the server socket
			if (i == server_fd)
			{
				printf("server socket is readdable\n");
				// make server sock accept each incoming connection
				while ((new_sd = server_sock_accept(server_fd, master_set)) != -1)
				{
					if (new_sd > max_sd)
						max_sd = new_sd;
				}
			}
		}
	}
}

int	ft_listen(int server_fd, struct sockaddr_in address)
{
	int new_socket, ret, ready_fd;
	int addrlen = sizeof(address);
	struct timeval timeout;

	// wait for incoming connection
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}

	// init master fd_set
	fd_set	master_set, working_set;
	FD_ZERO(&master_set);
	int max_sd = server_fd;
	FD_SET(server_fd, &master_set);

	// init timeval to 3 min (before ending program if no activity)
	timeout.tv_sec = 3 * 60;
	timeout.tv_usec = 0;

	while(1)
	{

		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		if ((ret = ft_select(&working_set, &master_set, max_fd, timeout)) < 0)
			break ;
		
		ft_find_readable_fd(ret, &working_set, &master_set, server_fd, max_sd);
		
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
		char * message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		write(new_socket , message , strlen(message));
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
