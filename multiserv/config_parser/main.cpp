/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnielly <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:53:56 by pnielly           #+#    #+#             */
/*   Updated: 2022/01/19 20:50:09 by pnielly          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../utils/utils.hpp"
#include "Parser.hpp"

std::string request_header = "GET /wiki/Wikipedia:Accueil_principal HTTP/2\r\nHost: fr.wikipedia.org\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:96.0) Gecko/20100101 Firefox/96.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate, br\r\nReferer: https://www.google.com/\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nCache-Control: max-age=0";

/**
 * main(): gets a config file in param and parses it
 **/
int		main(int ac, char **av) {

	Parser	parser;

	if (ac != 2)
		std::cout << RED << "Error: " << NC << "Need one and only one argument\n";
	else {
		try {
			parser.tokenizer(av);
			// TESTING
			print_test();
			requestParser(request_header, servers_g);
		}
		catch (std::exception &e) {
			std::cout << RED << "Error: " << NC << e.what() << std::endl;
		}
	}
}
