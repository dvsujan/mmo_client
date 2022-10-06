#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Player.hpp"
#include <SFML/Network.hpp>
#include <unordered_map>
class Client;
class ClientSend;
class ClientHandle;
class GameManager;
class TCPClient;

extern std::unordered_map<int, Player*> players;
extern std::string myUsername;
extern bool isFocused;
extern std::string gipaddr;
class TCPClient
{
public:
	TCPClient(std::string ipaddr);
	~TCPClient();
	void connect();
	void disconnect();
	void send(sf::Packet packet);
	void recieve();
	bool isConnected();

private:
	sf::TcpSocket socket;
	bool connected;
	std::string ip;
	int port = 26950;
};

class Client
{
public:
	static void ConnectToServer();
	static void InitializeClientData();
	static int myId;
};

class ClientSend
{
public:
	void sendTCPData(sf::Packet packet);
	static void sendWelcome();
	static void sendPlayerMovement(bool* inputs);
};

class ClientHandle
{
public:
	static void welcome(sf::Packet packet);
	static void spwanPlayer(sf::Packet packet);
	static void playerPosition(sf::Packet packet);
	static void playerRotation(sf::Packet packet);
	static void serverFull(sf::Packet packet);
	static void playerDisconnected(sf::Packet packet);
};

class GameManager
{
public:
	static void SpwanPlayer(int id, std::string username, sf::Vector2f position);
	static void update();
	static std::unordered_map<int, Player*> getPlayers();
	static void setup();
	static void SetPlayerPosition(int id, sf::Vector2f position);
};
#endif