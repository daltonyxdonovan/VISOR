#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <iostream>
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

		this->appName.setFont(font);
		this->appName.setString(appName);
		this->appName.setCharacterSize(15);
		this->appName.setFillColor(sf::Color::White);

		// Center the appIcon on 'pos'
		this->appRect.setSize(sf::Vector2f(100, 100));
		this->appRect.setOrigin(this->appRect.getSize() / 2.0f);
		this->appRect.setPosition(pos.x, pos.y);

		// Center the appName on 'pos' and adjust its position on the y-axis
		sf::FloatRect textRect = this->appName.getLocalBounds();
		this->appName.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		this->appName.setPosition(pos.x, pos.y + 90);

		this->appRect.setTexture(&appIconTex);
		this->appPath = appPath;
	}

	void draw(sf::RenderWindow& window)
	{
		//if appRect's texture is not the same as the appIconTex, set it to appIconTex
		if (appRect.getTexture() != &appIconTex)
		{
			appRect.setTexture(&appIconTex);
		}
		window.draw(appRect);
		window.draw(appName);
	}

	void update(sf::Vector2f offset, sf::RenderWindow& window, bool isDrawerEvenOpen, bool canOpenApp)
	{
		appRect.setPosition(this->pos.x, pos.y + offset.y);
		appName.setPosition(this->pos.x, pos.y + offset.y + 60);

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (appRect.getGlobalBounds().contains(mousePos.x, mousePos.y))
		{
			appRect.setFillColor(sf::Color(230,230,230));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (isDrawerEvenOpen && canOpenApp)
					clicked();
			}
			else
				appRect.setFillColor(sf::Color::White);
		}
	}

	void clicked()
	{
		// Enclose the appPath in double quotes
		string command = "\"" + appPath + "\"";
		system(command.c_str());
	}

};

