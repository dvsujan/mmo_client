#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Player
{
private:
	sf::Color color;
	int f = 0;
	sf::RectangleShape rect;
	sf::Vector2f size;
	float speed;
	std::string name = "";
	int health;
	sf::Font font;
	sf::Text pname;
	sf::RectangleShape healthbar;
	sf::RectangleShape healthBoarder;
	float rotation = 0;
	bool* inputs;

public:
	sf::Vector2f position;
	bool me = false;
	Player(std::string name, sf ::Vector2f position);
	void setSpeed(float speed)
	{
		this->speed = speed;
	}
	float getSpeed();
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition();
	sf::RectangleShape getShape();
	int getHealth();
	void damage(int amount);
	void setup();
	void update();
	void render(sf::RenderWindow& window);
	void setRotation(float angle);
	bool* getInputs();
	// float getRotation();
	void setFont(sf::Font& font);
	void setMe();
	std::string getName();
};

#endif