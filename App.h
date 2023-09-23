#pragma once
#include "SFML/Graphics.hpp"
#include <string>
using namespace std;

class App
{
public:
	sf::Font font;
	sf::Text appName;
	sf::RectangleShape appRect;
	sf::Texture appIconTex;
	sf::Vector2f pos;
	string appPath;

	App();

	App(std::string iconPath, std::string appName, sf::Vector2f pos, string appPath, sf::Font& font)
	{
		this->pos = pos;
		this->appIconTex.loadFromFile(iconPath);
		this->font.loadFromFile("resources/monospace.ttf");
		this->appIconTex = appIconTex;
		this->appName.setFont(font);
		this->appName.setString(appName);
		this->appName.setCharacterSize(20);
		this->appName.setFillColor(sf::Color::White);
		this->appName.setPosition(pos.x, pos.y + 250);
		this->appRect.setSize(sf::Vector2f(50,50));
		this->appRect.setOrigin(25, 25);
		this->appRect.setPosition(pos.x, pos.y);
		this->appRect.setTexture(&appIconTex);
		this->appPath = appPath;
	}	

	void draw(sf::RenderWindow& window)
	{
			window.draw(appRect);
			window.draw(appName);
	}

	void update(sf::Vector2f offset)
	{
		appRect.setPosition(this->pos.x, pos.y + offset.y);
		appName.setPosition(this->pos.x, pos.y + offset.y);
	}

	void clicked()
	{
		system(appPath.c_str());
	}
};

