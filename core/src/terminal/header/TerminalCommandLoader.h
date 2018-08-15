#include <iostream>
#include <fstream>
#include "TerminalCommand.h"
#include <dirent.h>
#include <vector>
#include "string.h"
#include <string>
#include <sstream>

using namespace std;

class TerminalCommandLoader {
	public:
		std::vector<TerminalCommand> readCommands(char commandPath[]) {
	    	std::vector<TerminalCommand> commandObjects;
			DIR* directory = opendir(commandPath);
			if (directory != NULL) {
				struct dirent *file;
				int index = 0;
		    	while ((file = readdir(directory)) != NULL) {
		    		char *match = NULL;
					match = strstr (file->d_name, ".txt");
					if(match) {
					    ifstream commandFile;
		    			char absoluteFile[256] = "";
		    			strcat(absoluteFile, commandPath);
		    			strcat(absoluteFile, "/");
		    			strcat(absoluteFile, file->d_name);
		    			commandFile.open(absoluteFile);
		    			if (commandFile.is_open()) {
		    				std::string line;
				    		while (getline(commandFile, line)) {
			    	  		TerminalCommand command = createCommand(line);
			    	  		cout << "Command " << index << " name: " << command.getName() << endl;
			    	  		cout << "Command " << index << " description: " << command.getDescription() << endl;
				      		commandObjects.push_back(command);
				      		index++;						
						    }
						    commandFile.close();
						  } else {
						  	cout << "Could not open file" << endl;
						  }
					}
		    	}
			} else {
				cout << "Could not open directory." << endl;
			}
		    closedir(directory);
			return commandObjects;
		}

		TerminalCommand createCommand(const std::string& commandText) {
		  std::stringstream lineStream(commandText);
	      std::string seg;

	      std::getline(lineStream, seg, ',');
	      std::string name = seg;

	      std::getline(lineStream, seg, ',');
	      std::string description = seg;

		  return TerminalCommand(name, description);
		}
};