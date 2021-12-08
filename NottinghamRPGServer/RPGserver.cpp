#include "RPGserver.h"

RPGserver::RPGserver(unsigned short port, bool rawMode = false) :
	listen_port (port) {
	logl("RPG Server Starting");
	
	this->rawMode = rawMode;
	if (rawMode) logl("Warning, Raw Mode Enabled");


};

RPGserver::ConnectClients(std::vector<sf::TcpSocket*>* client array)
{

	while (true) {
		sf::TcpSocket* new_client = new sf::TcpSocket();
	
	
	}

}