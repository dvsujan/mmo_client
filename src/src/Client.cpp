#include "../headers/Client.hpp"
// #include "../headers/Player.hpp"
#include <iostream>
#include <unordered_map>

/// @brief Server Packets
/// @nothig //

enum ServerPackets
{
	welcome = 1,
	spawnPlayer,
	playerPosition,
	playerRotation,
	serverFull,
	takeDamage,
	killPlayer,
	playerScore,
	disconnect
};
// <summary>Sent from client to server.</summary>
enum ClientPackets
{
	welcomeReceived = 1,
	playerMovement,
	playerShoot
};

std::string gipaddr;
TCPClient* client = new TCPClient(gipaddr);
std::unordered_map<int, Player*> players;
int myId;
std::string myUsername;
bool isFocused = true;
sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(1000, 800), "game Window");
bool isAlive = true;
void Client::ConnectToServer()
{
	InitializeClientData();
	client->connect();
}
void Client::InitializeClientData()
{
}

TCPClient::TCPClient(std::string ipaddr)
{
	this->ip = ipaddr;
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
				case ServerPackets::takeDamage: {
					ClientHandle::PlayerDamage(packet);
					break;
				}
				case ServerPackets::disconnect: {
					ClientHandle::playerDisconnected(packet);
					break;
				}
				case ServerPackets::killPlayer: {
					ClientHandle::playerKill(packet);
					break;
				}
				case ServerPackets::playerScore: {
					ClientHandle::PlayerScore(packet);
					break;
				}
				default: {
					std::cout << "Unknown packet type" << std::endl;
					break;
				}
			}
		}
		else if (this->socket.receive(packet) == sf::Socket::Disconnected)
		{
			std::cout << "server connection disconnected" << std::endl;
			exit(1);
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

void ClientSend::sendPlayerShoot(sf::Vector2i mousePos)
{
	sf::Packet packet;
	packet << static_cast<int>(ClientPackets::playerShoot);
	// packet << myId;
	packet << mousePos.x;
	packet << mousePos.y;
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

void ClientHandle::PlayerScore(sf::Packet packet)
{
	int id;
	packet >> id;
	int score;
	packet >> score;
	players[id]->setScore(score);
}

void ClientHandle::PlayerDamage(sf::Packet packet)
{
	int id;
	packet >> id;
	int health;
	packet >> health;
	players[id]->setHealth(health);
}

void ClientHandle::playerKill(sf::Packet packet)
{
	int id;
	packet >> id;
	std::cout << id << " died" << std::endl;
	if (id == myId)
	{
		isAlive = false;
		players.erase(myId);
		client->disconnect();
		exit(0);
	}
	else
	{
		players.erase(id);
	}
}

void ClientHandle::playerDisconnected(sf::Packet packet)
{
	int clientId;
	packet >> clientId;
	players.erase(clientId);
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
	sf::Clock clock;
	sf::Time t;
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
		// std::cout << "noplayers: " << x << std::endl;
		if (isAlive)
		{
			ClientSend::sendPlayerMovement(inputs);
		}
		//sleep for 120 ticks
		// sf::sleep(sf::milliseconds(1000 / 120));
		// if mouse is pressed then send mouse coordinates to server
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isAlive)
		{
			t = clock.getElapsedTime();
			//if elapse time is greater than 0.1 then send mouse coordinates
			if (t.asSeconds() > 0.5)
			{
				clock.restart();
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				// std::cout << "mouse Presed:  " << mousePos.x << " " << mousePos.y << std::endl;
				ClientSend::sendPlayerShoot(mousePos);
			}
		}
	}
}

void GameManager::SetPlayerPosition(int id, sf::Vector2f position)
{
	if (players[id] != nullptr)
	{
		players[id]->setPosition(position);
	}
}

void GameManager::setup()
{
	client->connect();
}
