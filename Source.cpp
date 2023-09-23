#pragma region ==INCLUDES==

// vvvv this line also requires the linker settings in project properties (linker>system) to be set to "windows subsystem" instead of "console subsystem" vvvv
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Button.h"
#include <iostream>
#include <boost/process.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "App.h"

using json = nlohmann::json;
namespace bp = boost::process;
namespace fs = boost::filesystem;
using namespace std;
#pragma endregion

// vvv NAME OF THE BOT TO DISPLAY ON THE VISOR vvv
const string botName1{ "GR33T-R" };
const string botName2{ "MODERATOR" };
const string botName3{ "DUNGEONMASTER" };
const string botName4{ "DUNGEONEER" };

// vvv LOCATION OF THE BOT SCRIPT TO RUN, WITHOUT THE SCRIPT ITSELF (the parent folder) vvv
const string bot1Path{ "C:/Users/dalto/Desktop/discord_bot" };
const string bot2Path{ "F:/code/bot" };
const string bot3Path{ "F:/code/bot/Gatekeeper" };
const string bot4Path{ "F:/code/bot" };

// vvv COMMAND TO RUN THE BOT SCRIPT vvv
const string bot1Command{ "node app.js" };
const string bot2Command{ "python moderator.py" };
const string bot3Command{ "python gatekeeper.py" };
const string bot4Command{ "python danny.py" };

#pragma region ==VARIABLES==

struct AppInfo 
{
    std::string appName;
    std::string appPath;
    std::string appIconPath;
};

const int width{ 500 };
const int height{ 500 };

sf::Texture textureBuffer;
string appsDirectory = "";
bool bot1Online = false;
bool bot2Online = false;
bool bot3Online = false;
bool bot4Online = false;
bool windowheld = false;
bool isDragging = false;
bool locked = false;
bool drawer = false;

int colorTicker = 0;
int colorBTicker = 10;
int ticker = 0;
int offset = 0; 

vector<sf::Color> colors;
sf::Color color;
sf::Color colorB;
sf::Vector2i dragStartPosition;
sf::Font font;
sf::Event event;
sf::Image icon;
sf::Sprite lockedSprite;
sf::Texture lockedIMG;
sf::Texture unlockedIMG;

sf::Text greeterTitle; //bot1
sf::Text modTitle; //bot2
sf::Text dmTitle; //bot3
sf::Text deTitle; //bot4
sf::Text title;
sf::Text outputText1;
sf::Text outputText2;
sf::Text outputText3;
sf::Text outputText4;

sf::Text testText;

sf::RectangleShape titlebar1(sf::Vector2f(width - 60, 2));
sf::RectangleShape titlebar2(sf::Vector2f(width - 65, 2));
sf::RectangleShape titlebarTop(sf::Vector2f(width, 2));
sf::RectangleShape borderTop(sf::Vector2f(width, 2));
sf::RectangleShape borderBottom(sf::Vector2f(width, 2));
sf::RectangleShape borderLeft(sf::Vector2f(2, height));
sf::RectangleShape borderRight(sf::Vector2f(2, height));
sf::RectangleShape backer0(sf::Vector2f(width-80, 50));
sf::RectangleShape backer1(sf::Vector2f(width-80, 50));
sf::RectangleShape backer2(sf::Vector2f(width-80, 50));
sf::RectangleShape backer3(sf::Vector2f(width-80, 50));
sf::RectangleShape drawerBacker(sf::Vector2f(width,height));

boost::mutex outputMutex1;
boost::mutex outputMutex2;
boost::mutex outputMutex3;
boost::mutex outputMutex4;

bp::child bot1process;
bp::child bot2process;
bp::child bot3process;
bp::child bot4process;

sf::Vector2i mousePos = sf::Mouse::getPosition();

#pragma endregion

int countApps(const std::string& directoryPath) {
    int jsonFileCount = 0;
    fs::path dirPath(directoryPath);

    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        for (fs::directory_iterator it(dirPath); it != fs::directory_iterator(); ++it) {
            if (fs::is_regular_file(*it) && it->path().extension() == ".json") {
                jsonFileCount++;
            }
        }
    } else {
        std::cerr << "Invalid directory path: " << directoryPath << std::endl;
    }

    return jsonFileCount;
}

std::vector<AppInfo> extractAppInfoFromJsonFiles(const std::string& directoryPath) 
{
    std::vector<AppInfo> appInfoList;

    fs::path dirPath(directoryPath);

    if (fs::exists(dirPath) && fs::is_directory(dirPath)) {
        for (fs::directory_iterator it(dirPath); it != fs::directory_iterator(); ++it) {
            if (fs::is_regular_file(*it) && it->path().extension() == ".json") {
                std::ifstream jsonFile(it->path().string());
                if (jsonFile) {
                    json jsonData;
                    jsonFile >> jsonData;

                    // Assuming the JSON structure has fields 'appName', 'appPath', and 'appIconPath'
                    if (jsonData.contains("appName") && jsonData.contains("appPath") && jsonData.contains("appIconPath")) {
                        AppInfo appInfo;
                        appInfo.appName = jsonData["appName"];
                        appInfo.appPath = jsonData["appPath"];
                        appInfo.appIconPath = jsonData["appIconPath"];
                        appInfoList.push_back(appInfo);
                    } else {
                        std::cerr << "JSON file does not contain all required fields: " << it->path().string() << std::endl;
                    }

                    jsonFile.close();
                } else {
                    std::cerr << "Failed to open JSON file: " << it->path().string() << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Invalid directory path: " << directoryPath << std::endl;
    }

    return appInfoList;
}

int main()
{
	//appsDirectory is the folder named 'apps' inside the program's directory folder
	appsDirectory = fs::current_path().string() + "/apps";
	
	std::vector<AppInfo> appInfoList = extractAppInfoFromJsonFiles(appsDirectory);
	std::vector<App> apps;


#pragma region ==VARIABLES INIT==

	HWND hWnd = GetConsoleWindow();
	if (hWnd)
		ShowWindow(hWnd, SW_HIDE);

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
	
	title.setFont(font);
	title.setString("VISOR");
	title.setCharacterSize(24);
	title.setFillColor(color);
	title.setStyle(sf::Text::Italic);
	title.setPosition(15, height-40);

	lockedIMG.loadFromFile("resources/locked.png");
	
	lockedSprite.setPosition(width - 60, 11);
	
	unlockedIMG.loadFromFile("resources/unlocked.png");
	lockedSprite.setTexture(unlockedIMG);
	lockedSprite.setScale(sf::Vector2f(0.75f, 0.75f));

	greeterTitle.setFont(font);
	greeterTitle.setString(botName1);
	greeterTitle.setCharacterSize(30);
	greeterTitle.setFillColor(sf::Color::White);
	greeterTitle.setPosition(50, 55);

	modTitle.setFont(font);
	modTitle.setString(botName2);
	modTitle.setCharacterSize(30);
	modTitle.setFillColor(sf::Color::White);
	modTitle.setPosition(50, 155);

	dmTitle.setFont(font);
	dmTitle.setString(botName3);
	dmTitle.setCharacterSize(30);
	dmTitle.setFillColor(sf::Color::White);
	dmTitle.setPosition(50, 255);

	deTitle.setFont(font);
	deTitle.setString(botName4);
	deTitle.setCharacterSize(30);
	deTitle.setFillColor(sf::Color::White);
	deTitle.setPosition(50, 355);

	Button exit(sf::Vector2f(width - 20, 20), sf::Vector2f(25, 25), sf::Color(0,0,0), sf::Color::White, "x", 24, true, false, false);
	Button greeter(sf::Vector2f(400,75),sf::Vector2f(100,30), sf::Color(0,0,0),sf::Color::Red,"OFFLINE",12,true, true, false);
	Button mod(sf::Vector2f(400, 175), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true, false);
	Button dm(sf::Vector2f(400, 275), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true, false);
	Button de(sf::Vector2f(400, 375), sf::Vector2f(100, 30), sf::Color(0, 0, 0), sf::Color::Red, "OFFLINE", 12, true, true, false);
	Button titleName(sf::Vector2f(45,height-23),sf::Vector2f(80,30),sf::Color::Black,sf::Color::White,"VISOR",24,true,false, true);

	titlebar1.setFillColor(color);
	titlebar2.setFillColor(color);
	titlebar1.setPosition(93, height - 27);
	titlebar2.setPosition(90, height - 22);
	titlebarTop.setFillColor(color);
	titlebarTop.setPosition(0, height-50);
	
	borderTop.setFillColor(color);
	borderBottom.setFillColor(color);
	borderLeft.setFillColor(color);
	borderRight.setFillColor(color);
	borderTop.setPosition(0, 0);
	borderBottom.setPosition(0, height - 2);
	borderLeft.setPosition(0, 0);
	borderRight.setPosition(width - 2, 0);

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

	outputText1.setFont(font);
	outputText1.setCharacterSize(12); // Adjust the size as needed
	outputText1.setFillColor(sf::Color::Green);
	outputText1.setPosition(50, 110); // Adjust the position as needed

	outputText2.setFont(font);
	outputText2.setCharacterSize(12); // Adjust the size as needed
	outputText2.setFillColor(sf::Color::Green);
	outputText2.setPosition(50, 210); // Adjust the position as needed

	outputText3.setFont(font);
	outputText3.setCharacterSize(12); // Adjust the size as needed
	outputText3.setFillColor(sf::Color::Green);
	outputText3.setPosition(50, 310); // Adjust the position as needed

	outputText4.setFont(font);
	outputText4.setCharacterSize(12); // Adjust the size as needed
	outputText4.setFillColor(sf::Color::Green);
	outputText4.setPosition(50, 410); // Adjust the position as needed

	drawerBacker.setFillColor(sf::Color(0,0,0));
	drawerBacker.setPosition(sf::Vector2f(0, height - 50+offset));



#pragma endregion
	
	for (int i = 0; i < countApps(appsDirectory); i++) 
	{
		float posX = 100 + (i % 3) * 150;
		float posY = 570 + (i / 3) * 125;

		App app = App(appInfoList[i].appIconPath, appInfoList[i].appName, sf::Vector2f(posX, posY), appInfoList[i].appPath, font);

		apps.push_back(app);
	}


	while (window.isOpen())
	{
#pragma region ==VARS REFRESH==
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
		exit.outline_color = color;
		greeter.outline_color = color;
		mod.outline_color = color;
		dm.outline_color = color;
		de.outline_color = color;

		titleName.text_color = color;
		lockedSprite.setColor(color);

		titlebar1.setPosition(93, height - 27 + offset);
		titlebar2.setPosition(90, height - 22 + offset);
		titlebarTop.setPosition(0, height - 50 + offset);
		titleName.set_position(sf::Vector2f(45, height - 23 + offset));
		drawerBacker.setPosition(sf::Vector2f(0, height - 50 + offset));
#pragma endregion

		if (drawer) //offset is the position of the drawer handle for the app drawer, not the window offset
		{
			if (offset > -450)
				offset -= 25;
		}
		else
		{
			if (offset < 0)
				offset += 25;
		}

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

		mousePos = sf::Mouse::getPosition(window);
		if (ticker > 0)
			ticker--;

#pragma region ==BUTTON CLICKS==
		if (exit.clicked && !locked && window.hasFocus())
		{
			//close all child processes if they are running
			if (bot1Online)
			{
				TerminateProcess(bot1process.native_handle(), 0);
				bot1process.wait();
			}
			if (bot2Online)
			{
				TerminateProcess(bot2process.native_handle(), 0);
				bot2process.wait();
			}
			if (bot3Online)
			{
				TerminateProcess(bot3process.native_handle(), 0);
				bot3process.wait();
			}
			if (bot4Online)
			{
				TerminateProcess(bot4process.native_handle(), 0);
				bot4process.wait();
			}
			window.close();
		}

		if (greeter.clicked && ticker == 0 && !locked && window.hasFocus())
		{
			ticker = 20;
			if (bot1Online)
			{
				TerminateProcess(bot1process.native_handle(), 0);
				bot1process.wait();
				
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream;
						bot1process = bp::child("cmd /c " + bot1Command, bp::std_out > pipe_stream, bp::std_err > stderr, bp::start_dir(bot1Path));

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

		if (mod.clicked && ticker == 0 && !locked && window.hasFocus())
		{
			
			ticker = 20;
			if (bot2Online)
			{
				TerminateProcess(bot2process.native_handle(), 0);
				bot2process.wait();
				
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream2;
						bot2process = bp::child("cmd /c " + bot2Command, bp::std_out > pipe_stream2, bp::std_err > stderr, bp::start_dir(bot2Path));
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

		if (dm.clicked && ticker == 0 && !locked && window.hasFocus())
		{
			ticker = 20;
			if (bot3Online)
			{
				TerminateProcess(bot3process.native_handle(), 0);
				bot3process.wait();
				
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream3;
						bot3process = bp::child("cmd /c " + bot3Command, bp::std_out > pipe_stream3, bp::std_err > stderr, bp::start_dir(bot3Path));
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

		if (de.clicked && ticker == 0 && !locked && window.hasFocus())
		{
			ticker = 20;
			if (bot4Online)
			{
				TerminateProcess(bot4process.native_handle(), 0);
				bot4process.wait();
				
			}
			else
			{
				boost::thread([&]() {
					try
					{
						bp::ipstream pipe_stream4;
						bot4process = bp::child("cmd /c " + bot4Command, bp::std_out > pipe_stream4, bp::std_err > stderr, bp::start_dir(bot4Path));

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

		if (titleName.clicked && ticker == 0 && window.hasFocus())
		{
			ticker = 20;
			drawer = !drawer;
		}
#pragma endregion

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
						else if (mousePos.x < 100) //this is here to disallow clicking in that area
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
				if (event.key.code == sf::Keyboard::Space)
					countApps(appsDirectory);

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

				if (event.key.code == sf::Keyboard::Up && !locked)
				{
					//offset--;
				}
				if (event.key.code == sf::Keyboard::Down && !locked)
				{
					//offset++;
				}
			}
		}

		if (windowheld && !locked && !drawer) //offset to drag the window, but only under certain conditions
			window.setPosition(sf::Mouse::getPosition() - sf::Vector2i(window.getSize().x / 2, 25));

		// vvv I don't _need_ bot1Online etc, it's vestigial from when I was using a different method to check if the process was running but I'm too lazy to remove it lol

		if (bot1process.running())
		{
			bot1Online = true;
			greeter.text_string = "ONLINE";
			greeter.text_color = sf::Color::Green;
		}
		else
		{
			bot1Online = false;
			greeter.text_string = "OFFLINE";
			greeter.text_color = sf::Color::Red;
			outputText1.setString("");
		}

		if (bot2process.running())
		{
			bot2Online = true;
			mod.text_string = "ONLINE";
			mod.text_color = sf::Color::Green;
		}
		else
		{
			bot2Online = false;
			mod.text_string = "OFFLINE";
			mod.text_color = sf::Color::Red;
			outputText2.setString("");
		}
		
		if (bot3process.running())
		{
			bot3Online = true;
			dm.text_string = "ONLINE";
			dm.text_color = sf::Color::Green;
		}
		else
		{
			bot3Online = false;
			dm.text_string = "OFFLINE";
			dm.text_color = sf::Color::Red;
			outputText3.setString("");
		}
		
		if (bot4process.running())
		{
			bot4Online = true;
			de.text_string = "ONLINE";
			de.text_color = sf::Color::Green;
		}
		else
		{
			bot4Online = false;
			de.text_string = "OFFLINE";
			de.text_color = sf::Color::Red;
			outputText4.setString("");
		}
		




		window.clear(colorB);

#pragma region ==DRAW LOOP==
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

		window.draw(outputText1);
		window.draw(outputText2);
		window.draw(outputText3);
		window.draw(outputText4);

		window.draw(drawerBacker);
		titleName.draw(window);
		titleName.update(window);
		window.draw(titlebar1);
		window.draw(titlebar2);
		window.draw(titlebarTop);

		window.draw(borderTop);
		window.draw(borderBottom);
		window.draw(borderLeft);
		window.draw(borderRight);
#pragma endregion

		//window.draw(testText);
		for (int i = 0; i < apps.size(); i++)
		{
			apps[i].draw(window);
			apps[i].update(sf::Vector2f(0,offset));
		}

		window.setFramerateLimit(60);
		window.display();
	}
    return 0;
}