#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "SFML/Network.hpp"

#define MAX_RAW_DATA 256 

#define logl(x) std::cout << x << std::endl;
#define log(x) std::cout << x;

class RPGserver
{

	//server network resources
	sf::TcpListener listener;
	std::vector<sf::TcpSocket*> client_array;

	unsigned short listen_port;
	bool rawMode = false;

public:
	//constructors
	RPGserver(unsigned short, bool); //default constructor. Perhaps second constructor for cheat mode / beta client?

	//Manage clients
	void ConnectClients(std::vector<sf::TcpSocket*>*);
	void DisconnectClients(sf::TcpSocket*, size_t);
		//custom (additional) functions 
	//void MessageClient(std::string, int); //message and client id

	void ReceivePacket(sf::TcpSocket *, size_t);
	void ReceiveRawData(sf::TcpSocket*, size_t);

	void BroadcastPacket(sf::Packet&, sf::IpAddress, unsigned short);
	void BroadcastRawData(const char*, sf::IpAddress, unsigned short);

	void ManagePackets();
	void Run();




};
