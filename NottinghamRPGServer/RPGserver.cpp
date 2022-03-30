#include "RPGserver.h"
#include "Player.h"

sf::Packet& operator << (sf::Packet& packet, sf::Vector2i& location)

{
	return packet << location.x << location.y;
}


sf::Packet& operator >> (sf::Packet& packet, sf::Vector2i& location)

{

	return packet >> location.x << location.y;
}


RPGserver::RPGserver(unsigned short port, bool rawMode = false) :
	listen_port (port) {
	logl("RPG Server Starting");
	std::cout << "Local IP is: " << sf::IpAddress::getLocalAddress() << "\n";
	std::cout << "Local IP is: " << sf::IpAddress::getPublicAddress() << "\n";
	this->rawMode = rawMode;
	if (rawMode) logl("Warning, Raw Mode Enabled");

	if (listener.listen(listen_port) != sf::Socket::Done)
		std::cout << "Could not listen";
	DayClock.restart().asSeconds();
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
			//push connection also into map alongside username?
			int i = 1;
			for (auto& client : IDmap)
			{
				std::cout << i << ": " << client.first << '\n';
				i++;
			}
		}
		else {
			std::cout << "no connections received yet\n";
			delete (new_client);
			

		}
	}
	std::this_thread::sleep_for((std::chrono::milliseconds)100);
}

void RPGserver::ReceivePacket(sf::TcpSocket* client, size_t iterator)
{

	sf::Packet packet;
	if (client->receive(packet) == sf::Socket::Disconnected)
	{
		
		for (auto it = IDmap.begin(); it != IDmap.end(); ++it)
			if (it->second == client)
			{
				std::cout << "\n" << it->first << " disconnected\n";
				IDmap.erase(it);
				break;
			//this needs to delete client from client array too
			}
		auto it2 = client_array.begin();
		for (auto it = IDmap.begin(); it2 != client_array.end(); ++it2)
			if (*it2 == client)
			{
				
				client_array.erase(it2);

				serverStats();
				break;
				//this needs to delete client from client array too
			}
		//	std::cout << "client disconnected\nclient disconnected \n client disconnected \n";
	}
	else
	{
	
		if (packet.getDataSize() > 0)
			//processing received packet into string / object/ vector etc.
		{
			std::string received_message;		//process packet to string storage
			unsigned char location;
			std::string username;
			int data = 0;
			std::string chat;
			sf::Vector2f position; //position packet
			//< data << player.direction << username << chat.playerInput.toAnsiString();

			packet >> data;

			if (data == 0)

			{
				int info{};
				packet >> info;
				std::cout << "\n" << info;
			}

			if (data == 5)
			{
				clientID(packet, client);
			}

			if (data == 2) {
				packet >> location >> username >> received_message;
				std::cout << "\nReceived: " << data << " " << " location: " << static_cast<int>(location) << ", " << username << " " << received_message;
				//DEBUG COMMENTS std::cout << "\n" << chat << "\n";
				packet.clear();
				packet << data << location << username << received_message << client->getRemoteAddress().toString() << client->getRemotePort(); // repackage with port and remote port.
				std::cout << "\nReceived after packing and sending: " << data << " " << " location: " << static_cast<int>(location) << ", " << username << " " << received_message;

				BroadcastPacket(packet, client->getRemoteAddress(), 2000);

			}


			if (data == 3) {
				//packet >> username >> position.x >> position.y;

				(positionSync(packet));
			}


			if (data == 4) {
				//packet >> username >> position.x >> position.y;

				(serverClientTrade(packet, client));
			}


			//header descriptor for Send Hug
			if (data == 21)
			{
			
				serverClientHug(packet, client);
			}
			//DEBUG COMMENTSstd::cout << client->getRemoteAddress() << " Says: " << received_message << "\n";

			else{
		}
				//BroadcastPacket(packet, client->getRemoteAddress(), 2000);

				}


	}
	packet.clear();
}

void RPGserver::BroadcastPacket(sf::Packet& packet, sf::IpAddress exclude_address, unsigned short port)
{
	for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
		sf::TcpSocket* client = client_array[iterator];
		if (client->send(packet) != sf::Socket::Done) { "Could not send packet on Broadcast function\n"; }
		else {}
		//DEBUG COMMENTS	std::cout << "Packet sent from server to " << client->getRemoteAddress() << std::endl;
	
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
	Update();
	std::this_thread::sleep_for((std::chrono::milliseconds)10);
	
	}

}
 void RPGserver::Run() {
	std::thread connection_thread(&RPGserver::ConnectClients, this, &client_array);
	
	ManagePackets();
	
	//WorldSync();
}

 void RPGserver::Update()
 {
	 hour += DayClock.getElapsedTime().asSeconds();
	 DayClock.restart();
	 if (hour > 7)
	 {
		 std::cout << "\nHour: " << dayDivide <<'\n';
		 dayDivide++;
		 timeSync();
		 hour = 0;
	 }
	 if (dayDivide > 15)
	 {
		 dayDivide = 4;
		// sf::TcpSocket* newcl;
		
		
	 }

	 if (hour > 0.5)
	 {
	//	 positionSync();
	 }

 }

 void RPGserver::timeSync()
 {
	 int dd = dayDivide;
	 int header = 1;
	 sf::Packet Sync;
	 Sync << header << dd;
	 std::cout << dayDivide;

	 for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
		 sf::TcpSocket* client = client_array[iterator];
		 if (client->send(Sync) != sf::Socket::Done) { "Could not send packet on Broadcast function\n"; }

	 }

	
 }

 void RPGserver::positionSync(sf::Packet packet)
 {
	 sf::Vector2f position;
	 std::string username;
	 packet >> username >> position.x >> position.y;
	 int data = 3;
	 sf::Packet syncpacket;
	 syncpacket << data << username << position.x << position.y;
	 for (size_t iterator = 0; iterator < client_array.size(); iterator++) {
		 sf::TcpSocket* client = client_array[iterator];
		 if (client->send(syncpacket) != sf::Socket::Done) { "Could not send packet on Broadcast function\n"; }

	 }


 }



 void RPGserver::clientID(sf::Packet packet, sf::TcpSocket* client)
 {
	 std::cout << "\nclient id function\n";
	// client.
	 std::string username{};
	 packet >> username;
	 if (username.size()<1)
	 {
		 std::cout << "empty username string";
		// return;
	 }
	 std::cout << "\nPacket contains: '" << username <<"";
	 IDmap.insert(std::pair<std::string, sf::TcpSocket*>(username, client));

 }

 void RPGserver::serverStats()
 {

	 std::cout<< "\n\n\nServer Stats:\n\n" <<IDmap.size() << " " << "Players Active\n\n ";
	 int count{};
	 for (const auto &player: IDmap) {
		 count++;
		 std::cout << count << ": " << player.first <<'\n';
	 }
 }

 void RPGserver::serverTradeRequest(sf::TcpSocket* target, std::string username)
 {
	 sf::Packet tradeRequest;
	 tradeRequest.clear();
	// int data = 20; //20 for trade request from server
	 int data = 20; //20 for trade request from server
	 tradeRequest << data << username;
	 if (target->send(tradeRequest) != sf::Socket::Done)
	 {	 std::cout << "\nTrade request failed\n";
 
	 
	 }
	 else {
		 int ryan{};
		 tradeRequest >> ryan;
		 std::cout << "\n" << ryan << " < is data header contents\nTrade request sent.\n";
	 }
 }

 void RPGserver::serverClientTrade(sf::Packet packet, sf::TcpSocket* client)
 {

	 //Function receives trade request from initiating client and passed data to serverTradeRequest function

	 std::string username;
	 packet >> username;
	
	 //Loop searches for client match in IDmap and prints out name to generate debug info to console.
	 //But function is not necessary beyond this.

	 auto it = IDmap.begin();
	 for (; it != IDmap.end(); ++it)
		 if (it->second == client)
			 break;
	 

	 std::cout << "\nServer received trade request from " << it->first << " to " << username;

	 //
	 for (auto it2 = IDmap.begin(); it2 != IDmap.end(); ++it2)
		 if (it2->first == username)
		 {
			 sf::TcpSocket* client;
			 client = it2->second;
			 serverTradeRequest(it2->second, it->first);
			 std::cout << "\nSending request to: " << it->first;
			 break;
		 }
 }

 void RPGserver::serverClientHug(sf::Packet packet, sf::TcpSocket* client)
 {
 
	 std::string username;
	 packet >> username;


	 auto it = IDmap.begin();
	 for (; it != IDmap.end(); ++it)
		 if (it->second == client)
			 break;

	 for (auto it2 = IDmap.begin(); it2 != IDmap.end(); ++it2)
	 if (it2->first == username)
	 {
		 sf::TcpSocket* client;
		 client = it2->second;

		 sf::Packet sendHug;
		 
		 // int data = 20; //20 for trade request from server
		 int data = 21; //20 for trade request from server
		 sendHug << data << it->first;
		 if (client->send(sendHug) != sf::Socket::Done)
		 {
			 std::cout << "\nHug send failed\n";


		 }
		 else {
			 std::cout << "Hug received to server and sent to " << username;
		 }


		 break;
	 }


 }