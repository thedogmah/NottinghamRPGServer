#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include "SFML/Network.hpp"
#include "Player.h"
#include <map>
#define MAX_RAW_DATA 256 

#define logl(x) std::cout << x << std::endl;
#define log(x) std::cout << x;

class RPGserver
{
	public:
	//server network resources
	sf::Packet packet;
	sf::TcpListener listener;
	std::vector<sf::TcpSocket*> client_array;
	std::map <std::string, std::string>ChatMap;

	std::map <std::string, sf::TcpSocket*> IDmap ;

	unsigned short listen_port;
	bool rawMode = false;
	std::vector<Player> livePlayers;
	//player data
	enum Direction : unsigned char {
		Stop = 0,
		Right = 1 << 0,
		Left = 1 << 1,
		Up = 1 << 2,
		Down = 1 << 3
	};
	//unsigned char
public:
	//constructors
	RPGserver(unsigned short, bool); //default constructor. Perhaps second constructor for cheat mode / beta client?
	int dayDivide=6;
	float hour = 0;
	sf::Clock DayClock;
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
	void Update();


	//server functions
	void timeSync();
	void positionSync(sf::Packet packet);
	void serverClientTrade(sf::Packet packet, sf::TcpSocket* client);
	void serverClientHug(sf::Packet paccket, sf::TcpSocket* client);
	void clientID(sf::Packet packet, sf::TcpSocket* client);
	void serverStats();//display serverstatistics //perhaps write a method within that also sends a packet of the stats to client

	//server handshakes
	void serverTradeRequest(sf::TcpSocket* target, std::string username);
};

sf::Packet& operator << (sf::Packet& packet, sf::Vector2i& location);
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2i& location);
