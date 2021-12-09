#include <iostream>
#include <thread>
#include "SFML/Network.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "RPGserver.h"
#include <vector>
int main()
{

	RPGserver server(2000, false);
	server.Run();

	return 0;

}

