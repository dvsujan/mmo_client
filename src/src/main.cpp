#include "../headers/Client.hpp"
#include "../headers/Player.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#define ticks 60

int main()
{
	std::string username;
	std::cout << "Enter Username" << std::endl;
	std::cin >> username;
	myUsername = username;
	////////////////////////////////////
	std::string ip;
	std::cout << "Enter Ipaddress" << std::endl;
	std::cin >> ip;
	gipaddr = ip;
	//////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(800, 600), "game Window");
	GameManager::setup();
	//////////////////////////////////////
	//caliculate fps
	sf::Clock clock;
	clock.restart();
	sf::Time time;
	window.setFramerateLimit(60);
	//run GameManager update in seperate thread
	//load font
	sf::Font font;

	if (!font.loadFromFile("resources/sansation.ttf"))
	{
		std::cout << "error loading font" << std::endl;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(15);
	text.setPosition(0, 0);
	text.setFillColor(sf::Color::Green);
	/// @nothig //
	sf::Thread thread(&GameManager::update);
	thread.launch();
	while (window.isOpen())
	{
		//convert time into fps
		time = clock.getElapsedTime();
		int fps = 1 / time.asSeconds();
		// std::cout << "FPS: " << fps << std::endl;
		text.setString("FPS: " + std::to_string(fps));
		// std::cout << "Fps" << time << std::endl;
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				thread.terminate();
			}
			if (event.type == sf::Event::GainedFocus)
			{

				isFocused = true;
			}
			if (event.type == sf::Event::LostFocus)
			{
				isFocused = false;
			}
		}
		// GameManager::update();
		//update stuff
		for (auto& player : players)
		{
			if (player.second != nullptr)
			{
				player.second->setFont(font);
				player.second->update();
			}
		}
		///////////////////////////////////////////////////////
		window.clear(sf::Color::White);
		//render stuf
		for (auto& player : players)
		{
			if (player.second != nullptr)
			{
				player.second->render(window);
			}
		}
		window.draw(text);
		window.display();
	}

	return EXIT_SUCCESS;
}
