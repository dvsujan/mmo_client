#include "../headers/Gun.hpp"
#include <math.h>

Gun::Gun(sf::Vector2f position)
{
	this->size = sf::Vector2f(30, 10);
	this->position = position;
	this->body.setSize(this->size);
	this->body.setPosition(this->position);
	this->body.setFillColor(sf::Color::Black);
}

void Gun::setup()
{
}

void Gun::update()
{
}

void Gun::setPosition(sf::Vector2f pos)
{
	this->position = pos;
	this->body.setPosition(this->position);
}

void Gun::render(sf::RenderWindow& window)
{
	window.draw(this->body);
}

void Gun::follorCursor(sf::Vector2f mousePos)
{
	float angle = atan2(mousePos.y - this->position.y, mousePos.x - this->position.x) * 180 / 3.14159265;
	this->body.setRotation(angle);
}
// void
void Gun::rotate(float angle)
{
	body.rotate(angle);
}
void Gun::followDirection(sf::Vector2f prevPos, sf::Vector2f position)
{
	if (prevPos.x > position.x)
	{
		this->body.setRotation(180);
	}
	else if (prevPos.x < position.x)
	{
		this->body.setRotation(0);
	}
}
