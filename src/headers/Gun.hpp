#ifndef GUN_HPP
#define GUN_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
class Gun
{
private:
	enum type
	{
		pistol,
		shotgun,
		rifle,
		sniper,
		rocket
	};
	sf::RectangleShape body;
	sf::Vector2f size;
	sf::Vector2f position;

public:
	Gun()
	{
	}
	Gun(sf::Vector2f position);
	void setup();
	void update();
	void setPosition(sf::Vector2f pos);
	void render(sf::RenderWindow& window);
	void rotate(float angle);
	void follorCursor(sf::Vector2f mousePos);
	void followDirection(sf::Vector2f prevPos, sf::Vector2f position);
	// void followDirection(sf::Vector2f direction);
};
#endif