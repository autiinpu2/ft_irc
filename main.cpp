#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int create_server(int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int connected_cli = 0;
	if (fd < 0)
		return 1;
	struct sockaddr_in sock;
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;
	struct in_addr addr;
	if (!inet_aton("10.11.6.1", &addr))
		return 2;
	sock.sin_addr = addr;
	if (bind(fd, (struct sockaddr *)&sock, sizeof(sockaddr_in)))
		return 3;
	if (listen(fd, 10) == -1)
		return 4;
	int client_fds[10];
	for (int i = 0; i < 10; ++i) {
		client_fds[i] = 0;
	}

	while (true) {
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		int max_fd = fd;
		
		for (int i = 0; i < 10; ++i) {
			int client_fd = client_fds[i];
			if (client_fd > 0) {
				FD_SET(client_fd, &rfds);
				if (client_fd > max_fd) {
					max_fd = client_fd;
				}
			}
		}

		if (select(max_fd + 1, &rfds, NULL, NULL, NULL) == -1)
			return 5;

		if (FD_ISSET(fd, &rfds)) {
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int new_client = accept(fd, (struct sockaddr *)&client_addr, &client_len);
			
			if (new_client >= 0) {
				bool added = false;
				for (int i = 0; i < 10; ++i) {
					if (client_fds[i] == 0) {
						client_fds[i] = new_client;
						std::cout << "\033[1;32mNew client connected! (FD: " << new_client << ")" << std::endl;
						connected_cli++;
						added = true;
						break;
					}
				}
				
				if (!added) {
					close(new_client);
				}
			}
		}
		for (int i = 0; i < 10; ++i) {
			int client_fd = client_fds[i];
			
			if (client_fd > 0 && FD_ISSET(client_fd, &rfds)) {
				char buffer[1024];
				memset(buffer, 0, 1024);
				int bytes_read = recv(client_fd, buffer, 1023, 0);
				
				if (bytes_read <= 0) {
					close(client_fd);
					client_fds[i] = 0;
					connected_cli--;
					std::cout << "\033[1;31mClient disconnected. (FD: " << client_fd << ") Remaining: " << \033[1;37m" << std::endl;
				} else {
					std::cout << "\033[1;33mReceived: " << buffer << "\033[1;37m" << std::endl;
					send(client_fd, buffer, bytes_read, 0);
				}
			}
		}
	}
	return 0;
}

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "\033[1;31Error: wrong number of agrs\033[1;37" << std::endl;
		return 1;
	}

	char *tmp;
	long port = strtol(av[1], &tmp, 10);
	if (tmp == av[1])
		return 1;
	std::string password = av[2];

	if (port < 0 || port > 65535)
		return 1;

	std::cout << "\033[1;33mPort is: " << port << "\nPassword is: " << password << "\033[1;37m" <<std::endl;
	if (int test = create_server(port)) {
		std::cout << "\033[1;31mError while running server\033[1;37m" << std::endl;
		std::cout << test << std::endl;
		return 1;
	}
}
