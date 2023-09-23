#include "iostream"
#include <string>
#include <fstream>
#include "BotSettings.h"

using namespace std;

class BotSettingFile
{
    public:
    

    BotSettingFile(BotSettings& settings) 
    {
    	// Load the bot settings from file
    	LoadBotSettings(settings);
    }

    // Load bot settings from file
    void LoadBotSettings(BotSettings& settings) {
        ifstream file("botSettings.txt");
        if (file.good()) {
            string line;
            int lineNumber = 0;

            while (getline(file, line)) {
                lineNumber++;

                // Check if the line number corresponds to a line with data
                switch (lineNumber) {
                    case 6:
                        settings.botName1 = line;
                        break;
                    case 9:
                        settings.botPath1 = line;
                        break;
                    case 12:
                        settings.botCommand1 = line;
                        break;
                    case 19:
                        settings.botName2 = line;
                        break;
                    case 22:
                        settings.botPath2 = line;
                        break;
                    case 25:
                        settings.botCommand2 = line;
                        break;
                    case 33:
                        settings.botName3 = line;
                        break;
                    case 36:
                        settings.botPath3 = line;
                        break;
                    case 39:
                        settings.botCommand3 = line;
                        break;
                    case 47:
                        settings.botName4 = line;
                        break;
                    case 50:
                        settings.botPath4 = line;
                        break;
                    case 53:
                        settings.botCommand4 = line;
                        break;
                    default:
                        // Skip lines that don't match the specified line numbers
                        break;
                }
            }
            file.close();
        }
    }



    // Save bot settings to file
    void SaveBotSettings(BotSettings& settings) {
        ofstream file("botSettings.txt");
        if (file.good()) {
            // Write settings to the file
            file << "// SLOT 1 //" << endl;
            file << "//NAME" << endl << settings.botName1 << endl;
            file << "//PATH" << endl << settings.botPath1 << endl;
            file << "//COMMAND TO RUN" << endl << settings.botCommand1 << endl;

            file << "// SLOT 2 //" << endl;
            file << "//NAME" << endl << settings.botName2 << endl;
            file << "//PATH" << endl << settings.botPath2 << endl;
            file << "//COMMAND TO RUN" << endl << settings.botCommand2 << endl;

            file << "// SLOT 3 //" << endl;
            file << "//NAME" << endl << settings.botName3 << endl;
            file << "//PATH" << endl << settings.botPath3 << endl;
            file << "//COMMAND TO RUN" << endl << settings.botCommand3 << endl;

            file << "// SLOT 4 //" << endl;
            file << "//NAME" << endl << settings.botName4 << endl;
            file << "//PATH" << endl << settings.botPath4 << endl;
            file << "//COMMAND TO RUN" << endl << settings.botCommand4 << endl;

            file.close();
        }
    }


    void Run() {
        BotSettings settings;

        // Load the bot settings from file
        LoadBotSettings(settings);

        // Show bot settings
        cout << "Bot Name 1: " << settings.botName1 << endl;
        cout << "Bot Path 1: " << settings.botPath1 << endl;
        cout << "Bot Command 1: " << settings.botCommand1 << endl;
        cout << "Bot Name 2: " << settings.botName2 << endl;
        cout << "Bot Path 2: " << settings.botPath2 << endl;
        cout << "Bot Command 2: " << settings.botCommand2 << endl;
        cout << "Bot Name 3: " << settings.botName3 << endl;
        cout << "Bot Path 3: " << settings.botPath3 << endl;
        cout << "Bot Command 3: " << settings.botCommand3 << endl;
        cout << "Bot Name 4: " << settings.botName4 << endl;
        cout << "Bot Path 4: " << settings.botPath4 << endl;
        cout << "Bot Command 4: " << settings.botCommand4 << endl;
    }
};


