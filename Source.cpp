#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Button.h"
#include <iostream>
#include <boost/process.hpp>
#include <boost/thread.hpp>

namespace bp = boost::process;

//in the standard namespace
using namespace std;

//variables
const int width{ 500 };
const int height{ 500 };
const string botName1{ "GR33T-R" };
const string botName2{ "MODERATOR" };
const string botName3{ "DUNGEONMASTER" };
const string botName4{ "DUNGEONEER" };
bool bot1Online = false;
bool bot2Online = false;
bool bot3Online = false;
bool bot4Online = false;
sf::Color color;
sf::Color colorB;
int colorTicker = 0;
int colorBTicker = 10;
int ticker = 0;
sf::Vector2i dragStartPosition;
bool windowheld = false;
bool isDragging = false;
sf::Font font;
sf::Event event;
sf::Image icon;
bool locked = false;

vector<sf::Color> colors;

int main()
{
	HWND hWnd = GetConsoleWindow();
	if (hWnd)
		ShowWindow(hWnd, SW_HIDE);
#pragma region ==VARIABLES==

	//add colors to vector
	colors.push_back(sf::Color::White);
	colors.push_back(sf::Color::Red);
	colors.push_back(sf::Color::Green);
	colors.push_back(sf::Color::Blue);
	colors.push_back(sf::Color::Yellow);
	colors.push_back(sf::Color::Magenta);
	colors.push_back(sf::Color::Cyan);
	colors.push_back(sf::Color(246,138,229));
	colors.push_back(sf::Color(98,164,222));
	colors.push_back(sf::Color(252,40,72));
	colors.push_back(sf::Color(0, 0, 0));
	colors.push_back(sf::Color(30,30,30));


	color = sf::Color::White;
	colorB = sf::Color::Black;
	sf::RenderWindow window(sf::VideoMode(width, height), "VISOR", sf::Style::None);
	icon.loadFromFile("Icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	font.loadFromFile("resources/monospace.ttf");

	sf::Text title;
	title.setFont(font);
	title.setString("VISOR");
	title.setCharacterSize(24);
	title.setFillColor(color);
	title.setStyle(sf::Text::Italic);
	title.setPosition(15, height-40);

	sf::Texture lockedIMG;
	lockedIMG.loadFromFile("resources/locked.png");
	sf::Sprite lockedSprite;
	lockedSprite.setPosition(width - 60, 11);
	sf::Texture unlockedIMG;
	unlockedIMG.loadFromFile("resources/unlocked.png");
	lockedSprite.setTexture(unlockedIMG);
	lockedSprite.setScale(sf::Vector2f(0.75f, 0.75f));

	sf::Text greeterTitle;
	greeterTitle.setFont(font);
	greeterTitle.setString(botName1);
	greeterTitle.setCharacterSize(30);
	greeterTitle.setFillColor(sf::Color::White);
	greeterTitle.setPosition(50, 55);

	sf::Text modTitle;
	modTitle.setFont(font);
	modTitle.setString(botName2);
	modTitle.setCharacterSize(30);
	modTitle.setFillColor(sf::Color::White);
	modTitle.setPosition(50, 155);

	sf::Text dmTitle;
	dmTitle.setFont(font);
	dmTitle.setString(botName3);
	dmTitle.setCharacterSize(30);
	dmTitle.setFillColor(sf::Color::White);
	dmTitle.setPosition(50, 255);

	sf::Text deTitle;
	deTitle.setFont(font);
	deTitle.setString(botName4);
	deTitle.setCharacterSize(30);
	deTitle.setFillColor(sf::Color::White);
	deTitle.setPosition(50, 355);

	Button exit(sf::Vector2f(width - 20, 20), sf::Vector2f(25, 25), sf::Color(0,0,0), sf::Color::White, "x", 24, true, false);
	Button greeter(sf::Vector2f(400,75),sf::Vector2f(100,30), sf::Color(0,0,0),sf::Color::Red,"OFFLINE",12,true, true);
	Button mod(sf::Vector2f(400, 175), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true);
	Button dm(sf::Vector2f(400, 275), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true);
	Button de(sf::Vector2f(400, 375), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true);

	sf::RectangleShape titlebar1(sf::Vector2f(width - 60, 2));
	sf::RectangleShape titlebar2(sf::Vector2f(width - 65, 2));
	sf::RectangleShape titlebarTop(sf::Vector2f(width, 2));
	titlebar1.setFillColor(color);
	titlebar2.setFillColor(color);
	titlebar1.setPosition(93, height - 27);
	titlebar2.setPosition(90, height - 22);
	titlebarTop.setFillColor(color);
	titlebarTop.setPosition(0, height-50);
	//TODO:: make an offset for the titlebar so it can be moved easily,
	//TODO:: add titlebarTop here for it so app bar can be made

	sf::RectangleShape borderTop(sf::Vector2f(width, 2));
	sf::RectangleShape borderBottom(sf::Vector2f(width, 2));
	sf::RectangleShape borderLeft(sf::Vector2f(2, height));
	sf::RectangleShape borderRight(sf::Vector2f(2, height));

	borderTop.setFillColor(color);
	borderBottom.setFillColor(color);
	borderLeft.setFillColor(color);
	borderRight.setFillColor(color);
	borderTop.setPosition(0, 0);
	borderBottom.setPosition(0, height - 2);
	borderLeft.setPosition(0, 0);
	borderRight.setPosition(width - 2, 0);

	sf::RectangleShape backer0(sf::Vector2f(width-80, 50));
	sf::RectangleShape backer1(sf::Vector2f(width-80, 50));
	sf::RectangleShape backer2(sf::Vector2f(width-80, 50));
	sf::RectangleShape backer3(sf::Vector2f(width-80, 50));

	backer0.setFillColor(sf::Color(20, 20, 20));
	backer1.setFillColor(sf::Color(20, 20, 20));
	backer2.setFillColor(sf::Color(20, 20, 20));
	backer3.setFillColor(sf::Color(20, 20, 20));

	backer0.setOutlineColor(color);
	backer1.setOutlineColor(color);
	backer2.setOutlineColor(color);
	backer3.setOutlineColor(color);

	backer0.setOutlineThickness(2);
	backer1.setOutlineThickness(2);
	backer2.setOutlineThickness(2);
	backer3.setOutlineThickness(2);


	backer0.setPosition(40, 50);
	backer1.setPosition(40, 150);
	backer2.setPosition(40, 250);
	backer3.setPosition(40, 350);

	boost::mutex outputMutex1;
	sf::Text outputText1;

	outputText1.setFont(font);
	outputText1.setCharacterSize(12); // Adjust the size as needed
	outputText1.setFillColor(sf::Color::Green);
	outputText1.setPosition(50, 110); // Adjust the position as needed

	boost::mutex outputMutex2;
	sf::Text outputText2;

	outputText2.setFont(font);
	outputText2.setCharacterSize(12); // Adjust the size as needed
	outputText2.setFillColor(sf::Color::Green);
	outputText2.setPosition(50, 210); // Adjust the position as needed

	boost::mutex outputMutex3;
	sf::Text outputText3;

	outputText3.setFont(font);
	outputText3.setCharacterSize(12); // Adjust the size as needed
	outputText3.setFillColor(sf::Color::Green);
	outputText3.setPosition(50, 310); // Adjust the position as needed

	boost::mutex outputMutex4;
	sf::Text outputText4;

	outputText4.setFont(font);
	outputText4.setCharacterSize(12); // Adjust the size as needed
	outputText4.setFillColor(sf::Color::Green);
	outputText4.setPosition(50, 410); // Adjust the position as needed

	bp::child bot1process;
	bp::child bot2process;
	bp::child bot3process;
	bp::child bot4process;



#pragma endregion

	

	while (window.isOpen())
	{
		borderTop.setFillColor(color);
		borderBottom.setFillColor(color);
		borderLeft.setFillColor(color);
		borderRight.setFillColor(color);
		titlebar1.setFillColor(color);
		titlebar2.setFillColor(color);
		title.setFillColor(color);
		titlebarTop.setFillColor(color);
		backer0.setOutlineColor(color);
		backer1.setOutlineColor(color);
		backer2.setOutlineColor(color);
		backer3.setOutlineColor(color);
		exit.text_color = color;
		lockedSprite.setColor(color);

		if (locked)
		{
			if (lockedSprite.getTexture() != &lockedIMG)
				lockedSprite.setTexture(lockedIMG);
		}
		else
		{
			if (lockedSprite.getTexture() != &unlockedIMG)
				lockedSprite.setTexture(unlockedIMG);
		}

		//if window is unfocused, lock it
		if (!window.hasFocus())
		{
			locked = true;
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (ticker > 0)
			ticker--;

		if (exit.clicked && !locked)
		{
			window.close();
		}

		if (greeter.clicked && ticker == 0 && !locked)
		{
			ticker = 20;
			if (bot1Online)
			{
				bot1process.terminate();
				bot1process.wait();
				bot1Online = false;
				outputText1.setString("");
				greeter.text_string = "OFFLINE";
				greeter.text_color = sf::Color::Red;
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream;
						bot1process = bp::child("cmd /c node app.js", bp::std_out > pipe_stream, bp::std_err > stderr, bp::start_dir("C:/Users/dalto/Desktop/discord_bot"));

						std::string line;
						std::string output;

						while (std::getline(pipe_stream, line))
						{
							output += line + "\n";
							//make output all uppercase
							std::transform(output.begin(), output.end(), output.begin(), ::toupper);
							outputText1.setString(output);
						}

						bot1Online = true;
						greeter.text_string = "ONLINE";
						greeter.text_color = sf::Color::Green;
					}
					catch (const boost::process::process_error& e)
					{
						std::cerr << "Error starting the process: " << e.what() << std::endl;
						outputText1.setString(e.what());
					}
					
				});
				bot1Online = true;
				greeter.text_string = "ONLINE";
				greeter.text_color = sf::Color::Green;
			}
		}

		if (mod.clicked && ticker == 0 && !locked)
		{
			
			ticker = 20;
			if (bot2Online)
			{
				bot2process.terminate();
				bot2process.wait();
				bot2Online = false;
				mod.text_string = "OFFLINE";
				mod.text_color = sf::Color::Red;
				outputText2.setString("");
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream2;
						bot2process = bp::child("cmd /c python moderator.py", bp::std_out > pipe_stream2, bp::std_err > stderr, bp::start_dir("F:/code/bot"));
						bot2Online = true;
						mod.text_string = "ONLINE";
						mod.text_color = sf::Color::Green;
						outputText2.setString("READY FOR COMMANDS");
					}
					catch (const boost::process::process_error& e)
					{
						std::cerr << "Error starting the process: " << e.what() << std::endl;
					}
					});
			}
		}

		if (dm.clicked && ticker == 0 && !locked)
		{
			ticker = 20;
			if (bot3Online)
			{
				bot3process.terminate();
				bot3process.wait();
				bot3Online = false;
				dm.text_string = "OFFLINE";
				dm.text_color = sf::Color::Red;
				outputText3.setString("");
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream3;
						bot3process = bp::child("cmd /c python gatekeeper.py", bp::std_out > pipe_stream3, bp::std_err > stderr, bp::start_dir("F:/code/bot/Gatekeeper"));
						bot3Online = true;
						dm.text_string = "ONLINE";
						dm.text_color = sf::Color::Green;
						outputText3.setString("READY FOR COMMANDS");
					}
					catch (const boost::process::process_error& e)
					{
						std::cerr << "Error starting the process: " << e.what() << std::endl;
					}
					});
			}
		}

		if (de.clicked && ticker == 0 && !locked)
		{
			ticker = 20;
			if (bot4Online)
			{
				bot4process.terminate();
				bot4process.wait();
				bot4Online = false;
				de.text_string = "OFFLINE";
				de.text_color = sf::Color::Red;
				outputText4.setString("");
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream4;
						bot4process = bp::child("cmd /c python danny.py", bp::std_out > pipe_stream4, bp::std_err > stderr, bp::start_dir("F:/code/bot"));

						bot4Online = true;
						de.text_string = "ONLINE";
						de.text_color = sf::Color::Green;
						outputText4.setString("READY FOR COMMANDS");
						
					}
					catch (const boost::process::process_error& e)
					{
						std::cerr << "Error starting the process: " << e.what() << std::endl;
					}
					});
			}
		}



		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//check if mouse is within 50px of the top of the window
					if (mousePos.y < 50)
					{
						//check if mouse is within 50px of the right of the window
						if (mousePos.x > window.getSize().x - 70)
						{
							
						}
						else
						{
							windowheld = true;

						}
					}

					//if lockedSprite is clicked, lock the window
					if (lockedSprite.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
						locked = !locked;
					}


				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					windowheld = false;
				}
			}
			//if arrow keys are pressed, change sf::Color color to the next sf::Color in vector of them
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Right && !locked)
				{
					colorTicker++;
					if (colorTicker > colors.size() - 1)
						colorTicker = 0;
					color = colors[colorTicker];
				}
				if (event.key.code == sf::Keyboard::Left && !locked)
				{
					colorTicker--;
					if (colorTicker < 0)
						colorTicker = colors.size() - 1;
					color = colors[colorTicker];
				}

				//up and down arrows change colorBticker
				if (event.key.code == sf::Keyboard::Up && !locked)
				{
					colorBTicker++;
					if (colorBTicker > colors.size() - 1)
						colorBTicker = 0;
					colorB = colors[colorBTicker];
				}
				if (event.key.code == sf::Keyboard::Down && !locked)
				{
					colorBTicker--;
					if (colorBTicker < 0)
						colorBTicker = colors.size() - 1;
					colorB = colors[colorBTicker];
				}
			}
		}

		if (windowheld && !locked)
			window.setPosition(sf::Mouse::getPosition() - sf::Vector2i(window.getSize().x / 2, 25));

		//clear the window
		window.clear(colorB);

		
		window.draw(borderTop);
		window.draw(borderBottom);
		window.draw(borderLeft);
		window.draw(borderRight);
		window.draw(backer0);
		window.draw(backer1);
		window.draw(backer2);
		window.draw(backer3);

		window.draw(greeterTitle);
		window.draw(modTitle);
		window.draw(dmTitle);
		window.draw(deTitle);
		
		greeter.update(window);
		greeter.draw(window);
		mod.update(window);
		mod.draw(window);
		dm.update(window);
		dm.draw(window);
		de.update(window);
		de.draw(window);

		exit.update(window);
		exit.draw(window);
		window.draw(lockedSprite);

		window.draw(title);
		window.draw(titlebar1);
		window.draw(titlebar2);
		window.draw(titlebarTop);

		window.draw(outputText1);
		window.draw(outputText2);
		window.draw(outputText3);
		window.draw(outputText4);

		window.setFramerateLimit(60);
		window.display();
	}
	//return okay if the program exits properly
    return 0;
}