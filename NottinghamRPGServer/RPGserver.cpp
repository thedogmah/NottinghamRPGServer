#include "RPGserver.h"

RPGserver::RPGserver(unsigned short port, bool rawMode = false) :
	listen_port (port) {
	logl("RPG Server Starting");
	
	this->rawMode = rawMode;
	if (rawMode) logl("Warning, Raw Mode Enabled");

	if (listener.listen(listen_port) != sf::Socket::Done)
		std::cout << "Could not listen";
};

void RPGserver::ConnectClients(std::vector<sf::TcpSocket*> *client_array)
{
	logl("Connect Clients thread started\n");
	while (true) {
		sf::TcpSocket* new_client = new sf::TcpSocket();
		//new_client->receive(packet);
		if (listener.accept(*new_client) == sf::Socket::Done) {
			new_client->setBlocking(false);
			client_array->push_back(new_client);
			std::cout << "Connected clients is: " << client_array->size() << std::endl;
		}
		else {
			std::cout << "no connections received yet\n";
			delete (new_client);
		}
	}
	//std::this_thread::sleep_for((std::chrono::milliseconds)100);
}

void RPGserver::ReceivePacket(sf::TcpSocket* client, size_t iterator)
{

	sf::Packet packet;
	if (client->receive(packet) == sf::Socket::Disconnected)
	{
		//	std::cout << "client disconnected\nclient disconnected \n client disconnected \n";
	}
	else
	{
	
		if (packet.getDataSize() > 0)
		//processing received packet into string / object/ vector etc.
				{
				std::string received_message;		//process packet to string storage

				packet >> received_message;
				packet.clear();
				std::cout << client->getRemoteAddress() << " Says: " << received_message << "\n";
				packet << received_message << client->getRemoteAddress().toString(), client->getRemotePort(); // repackage with port and remote port.
		
				BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());

				}


	}
}

void RPGserver::BroadcastPacket(sf::Packet& packet, sf::IpAddress exclude_address, unsigned short port)
{
	for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
		sf::TcpSocket* client = client_array[iterator];
		if (client->send(packet) != sf::Socket::Done) { "Could not send packet on Broadcast function\n"; }
		else
			std::cout << "Packet sent from server to " << client->getRemoteAddress() << std::endl;
	
	}
}

void RPGserver::ManagePackets()

{

	while (true) {
	for (size_t iterator = 0; iterator < client_array.size() ; iterator++)
		{
		ReceivePacket(client_array[iterator], iterator);
	//	std::cout << "Currentlty in:Manage Packets Member Function\n";
		
	}
	
	std::this_thread::sleep_for((std::chrono::milliseconds)100);
	
	}

}
 void RPGserver::Run() {
	std::thread connection_thread(&RPGserver::ConnectClients, this, &client_array);
	
	ManagePackets();
}