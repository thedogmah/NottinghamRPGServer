#include <iostream>
#include <thread>
#include "SFML/Network.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"
#include "RPGserver.h"
int main()
{

	RPGserver server(3000, false);

	return 0;

}

