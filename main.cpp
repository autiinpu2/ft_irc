#include <cstdlib>
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Error: wrong number of agrs" << std::endl;
		return 1;
	}

	int port = atoi(av[1]);
	std::string password = av[2];

	if (port < 0 || port > 65535)
		return 1;

	std::cout << av[1] << " " << av[2] << std::endl;
}
