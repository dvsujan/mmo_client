#include "../headers/Client.hpp"
#include "../headers/Player.hpp"
#include <iostream>
#include <unordered_map>
enum ServerPackets
{
	welcome = 1,
	spawnPlayer,
	playerPosition,
	playerRotation,
	serverFull
};

/// <summary>Sent from client to server.</summary>
enum ClientPackets
{
	welcomeReceived = 1,
	playerMovement
};

TCPClient* client = new TCPClient();
std::unordered_map<int, Player*> players;
int myId;
std::string myUsername;
bool isFocused = true;

void Client::ConnectToServer()
{
	InitializeClientData();
	client->connect();
}
void Client::InitializeClientData()
{
}

TCPClient::TCPClient()
{
	connected = false;
}

void TCPClient::connect()
{
	if (this->socket.connect(this->ip, this->port) != sf::Socket::Done)
	{
		std::cout << "Failed to connect to server" << std::endl;
		this->connected = false;
		exit(EXIT_FAILURE);
	}
	else
	{
		this->connected = true;
		std::cout << "Connected to server" << std::endl;
	}
}

void TCPClient::disconnect()
{
	this->socket.disconnect();
	this->connected = false;
}

void TCPClient::send(sf::Packet packet)
{
	if (this->connected)
	{
		this->socket.send(packet);
	}
}

void TCPClient::recieve()
{
	sf::Packet packet;
	if (this->connected)
	{
		if (this->socket.receive(packet) == sf::Socket::Done)
		{
			int packetType = 0;
			packet >> packetType;
			switch (packetType)
			{
				case ServerPackets::welcome: {
					ClientHandle::welcome(packet);
					break;
				}
				case ServerPackets::spawnPlayer: {
					ClientHandle::spwanPlayer(packet);
					break;
				}
				case ServerPackets::playerPosition: {
					ClientHandle::playerPosition(packet);
					break;
				}
				case ServerPackets::playerRotation: {
					ClientHandle::playerRotation(packet);
					break;
				}
				case ServerPackets::serverFull: {
					ClientHandle::serverFull(packet);
					break;
				}
				default: {
					std::cout << "Unknown packet type" << std::endl;
					break;
				}
			}
		}
	}
}

void ClientSend::sendTCPData(sf::Packet packet)
{
	client->send(packet);
}

void ClientSend::sendWelcome()
{
	sf::Packet packet;
	packet << static_cast<int>(ClientPackets::welcomeReceived);
	packet << myId;
	packet << myUsername;
	client->send(packet);
}

void ClientSend::sendPlayerMovement(bool* inputs)
{
	sf::Packet packet;
	packet << static_cast<int>(ClientPackets::playerMovement);
	//if true then send 1 else send 0
	packet << (int)inputs[0];
	packet << (int)inputs[1];
	packet << (int)inputs[2];
	packet << (int)inputs[3];
	packet << 0;
	// packet << players[myId]->getRotation();
	client->send(packet);
}

void ClientHandle::welcome(sf::Packet packet)
{
	std::cout << "welcome Received from server" << packet.getData() << std::endl;
	int myid;
	packet >> myid;
	// std::cout << "got Id as : " << myid << std::endl;
	std::string username;
	packet >> username;
	myId = myid;
	ClientSend::sendWelcome();
}

void ClientHandle::spwanPlayer(sf::Packet packet)
{
	// std::cout << "welcome Received from server" << packet.getData() << std::endl;
	int idx;
	packet >> idx;
	// std::cout << "spwaned player " << idx << std::endl;
	std::string username;
	packet >> username;
	int X, Y;
	packet >> X;
	packet >> Y;
	GameManager::SpwanPlayer(idx, username, sf::Vector2f(X, Y));
}

void ClientHandle::playerPosition(sf::Packet packet)
{
	// std::cout << "playerPosition " << packet.getData() << std::endl;
	int id;
	packet >> id;
	int x;
	packet >> x;
	int y;
	packet >> y;
	//set playerPosition which is in other thread
	GameManager::SetPlayerPosition(id, sf::Vector2f(x, y));
}

void ClientHandle::playerRotation(sf::Packet packet)
{
	// std::cout << "playerRotation " << packet.getData() << std::endl;
	int id;
	packet >> id;
	float rotation;
	packet >> rotation;
	if (players[id] != nullptr)
		players[id]->setRotation(rotation);
}

void ClientHandle::serverFull(sf::Packet packet)
{
	std::cout << "serverFull " << packet.getData() << std::endl;
	exit(0);
}

void GameManager::SpwanPlayer(int id, std::string username, sf::Vector2f position)
{
	if (id == myId)
	{
		players[id] = new Player(username, position);
		players[id]->setMe();
	}
	else
	{
		players[id] = new Player(username, position);
	}
	players[id]->setup();
}

void GameManager::update()
{
	while (true)
	{
		client->recieve();
		bool* inputs = new bool[4];
		if (isFocused)
		{
			inputs[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
			inputs[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
			inputs[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
			inputs[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		}
		else
		{
			inputs[0] = false;
			inputs[1] = false;
			inputs[2] = false;
			inputs[3] = false;
		}
		ClientSend::sendPlayerMovement(inputs);
		//sleep for 120 ticks
		// sf::sleep(sf::milliseconds(1000 / 120));
	}
}

void GameManager::SetPlayerPosition(int id, sf::Vector2f position)
{
	if (players[id] != nullptr)
	{
		players[id]->setPosition(position);
	}
}

// std::unordered_map<int, Player*> GameManager::getPlayers()
// {
// 	return players;
// }

void GameManager::setup()
{
	client->connect();
}