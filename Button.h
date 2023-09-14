#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>

using namespace std;

class Button
{
public:
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Color color;
	sf::Color hover_color;
	sf::Color clicked_color;
	sf::Color text_color;
	sf::Color text_hover_color;
	sf::Color text_clicked_color;
	sf::Font font;
	bool clicked = false;
	bool hover = false;
	bool active = false;
	sf::RectangleShape shape;
	string text_string;
	int font_size;
	bool interactable;
	bool outlined;

	Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Color text_color, string text_string, int font_size, bool interactable, bool outlined)
	{
		this->position = position;
		this->size = size;
		this->color = color;
		this->text_color = text_color;
		this->font.loadFromFile("resources/monospace.ttf");
		this->text_string = text_string;
		this->font_size = font_size;
		this->interactable = interactable;
		this->outlined = outlined;
		
		shape.setSize(size);
		shape.setOrigin(size.x / 2, size.y / 2);
		shape.setPosition(position);
		shape.setFillColor(color);


		//if fill color is white, set outline color to black
		//else set outline color to white
		if (color == sf::Color::White)
		{
			shape.setOutlineColor(sf::Color::Black);
		}
		else
		{
			shape.setOutlineColor(sf::Color::White);
		}

		if (outlined)
			shape.setOutlineThickness(2);
		else
			shape.setOutlineThickness(0);
		hover_color = sf::Color(color.r + 50 > 255 ? 255 : color.r + 50, color.g + 50 > 255 ? 255 : color.g + 50, color.b + 50 > 255 ? 255 : color.b + 50);
		clicked_color = sf::Color(color.r + 100 > 255 ? 255 : color.r + 100, color.g + 100 > 255 ? 255 : color.g + 100, color.b + 100 > 255 ? 255 : color.b + 100);
		text_hover_color = sf::Color(text_color.r + 50 > 255 ? 255 : text_color.r + 50, text_color.g + 50 > 255 ? 255 : text_color.g + 50, text_color.b + 50 > 255 ? 255 : text_color.b + 50);
		text_clicked_color = sf::Color(text_color.r + 100 > 255 ? 255 : text_color.r + 100, text_color.g + 100 > 255 ? 255 : text_color.g + 100, text_color.b + 100 > 255 ? 255 : text_color.b + 100);
	}

	void set_position(sf::Vector2f position)
	{
		this->position = position;
		shape.setPosition(position);
	}

	void update(sf::RenderWindow& window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosF{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
		//bounds should take into account the absolute position of the button
		sf::FloatRect bounds = sf::FloatRect(position.x-size.x/2, position.y-size.y/2, size.x, size.y);
		clicked = false;
		hover = false;

		if (bounds.contains(mousePosF))
		{
			hover = true;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				clicked = true;
				active = true;
			}
		}
		else
		{
			active = false;
		}

		//if (!interactable) return;
		if (clicked)
		{
			shape.setFillColor(clicked_color);
		}
		else if (hover)
		{
			shape.setFillColor(hover_color);
		}
		else
		{
			shape.setFillColor(color);
		}
	}

	void draw(sf::RenderWindow& window)
	{
		sf::Text text = sf::Text{ text_string, font };
		text.setCharacterSize(font_size);
		sf::Vector2f center = sf::Vector2f(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
		sf::Vector2f localBounds = center + sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top);
		text.setOrigin(localBounds);
		text.setPosition(position);
		text.setStyle(sf::Text::Bold);
		if (clicked)
		{
			text.setFillColor(text_clicked_color);
		}
		else if (hover)
		{
			text.setFillColor(text_hover_color);
		}
		else
		{
			text.setFillColor(text_color);
		}


		window.draw(shape);
		window.draw(text);

	}


};