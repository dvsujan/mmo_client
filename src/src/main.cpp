#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
// #include "../headers/Player.hpp"
#include "../headers/Client.hpp"
#include "../headers/Player.hpp"
#include <SFML/System/Thread.hpp>
#include <string>
#include <unordered_map>
#define ticks 60

int main()
{

	std::string username;
	std::cout << "Enter Username" << std::endl;
	std::cin >> username;
	myUsername = username;
	sf::RenderWindow window(sf::VideoMode(800, 600), "game Window");
	GameManager::setup();
	window.setFramerateLimit(120);
	//caliculate fps
	// sf::Clock clock;
	// sf::Time time = sf::Time::Zero;
	// sf::Time elapsed = sf::Time::Zero;
	// sf::Time fpsTime = sf::Time::Zero;
	//////////////////////////////////////
	//run GameManager update in seperate thread

	//load font
	sf::Font font;
	if (!font.loadFromFile("resources/sansation.ttf"))
	{
		std::cout << "error loading font" << std::endl;
	}

	sf::Thread thread(&GameManager::update);
	thread.launch();
	while (window.isOpen())
	{
		// std::cout<< "ttesting" <<std::endl;
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
		// for (auto& player : players)
		// {
		// }
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
		window.display();
		//run with delay
		sf::sleep(sf::milliseconds(1000 / ticks));
	}
	return EXIT_SUCCESS;
}