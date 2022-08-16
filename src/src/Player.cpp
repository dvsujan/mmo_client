#include "../headers/Player.hpp"
#include <iostream> //

Player::Player(std::string name, sf::Vector2f position)
{
	position = sf::Vector2f(400, 300);
	this->f = 1;
	// this->position = position;
	size = sf::Vector2f(20, 50);
	color = sf::Color::Red;
	rect = sf::RectangleShape();
	healthbar = sf::RectangleShape();
	rect.setOrigin(size.x / 2, size.y / 2);
	//
	speed = 4;
	this->name = name;
	this->health = 100;
	//

	pname.setFont(font);
	pname.setString(name);
	pname.setCharacterSize(15);
	pname.setFillColor(sf::Color::Black);
	pname.setPosition(position.x - 45, position.y - 5);
	//
	healthbar.setPosition(position.x, position.y);
	healthbar.setSize(sf::Vector2f(40, 10));
	healthbar.setFillColor(sf::Color::Green);
	//
	healthBoarder.setPosition(position.x, position.y);
	healthBoarder.setSize(sf::Vector2f(health / 2, 10));
	healthBoarder.setOutlineColor(sf::Color::Black);
	healthBoarder.setOutlineThickness(1);
	healthBoarder.setFillColor(sf::Color::Transparent);
	////////////////////////////////
	inputs = new bool[4];
	for (int i = 0; i < 4; i++)
	{
		inputs[i] = false;
	}
}

std::string Player::getName()
{
	return this->name;
}

void Player::setFont(sf::Font& font)
{

	this->font = font;
}
void Player::setPosition(sf::Vector2f positionn)
{
	// std::cout << "setting Player Position" << positionn.x << " " << positionn.y << std::endl;
	this->position = positionn;
}

float Player::getSpeed()
{
	return speed;
}

void Player::setup()
{
	if (this->me == true)
	{
		rect.setFillColor(sf::Color::Blue);
	}
	else
	{
		rect.setFillColor(sf::Color::Red);
	}
	rect.setSize(size);
	rect.setPosition(position);
}

void Player::update()
{
	if (this->name != "")
	{
		if (this->me == true)
		{
			this->color = sf::Color::Blue;
		}
		rect.setPosition(position);
		rect.setFillColor(color);
		pname.setPosition(position.x - 20, position.y - 50);
		healthbar.setPosition(position.x - 24, position.y + 30);
		healthbar.setSize(sf::Vector2f(health / 2, 10));
		healthBoarder.setPosition(position.x - 24, position.y + 30);
		healthBoarder.setSize(sf::Vector2f(100 / 2, 10));
		// std::cout << this->position.x << this->position.y << std::endl;
	}
}

void Player::render(sf::RenderWindow& window)
{

	window.draw(pname);
	window.draw(rect);
	window.draw(pname);
	window.draw(healthbar);
	window.draw(healthBoarder);
}

void Player::damage(int amount)
{
	health -= amount;
}
void Player::setRotation(float anglee)
{
	this->rotation = anglee;
}

bool* Player::getInputs()
{
	return inputs;
}

// float Player::getRotation()
// {
// 	return this->rotation;
// }
void Player::setMe()
{
	this->me = true;
}