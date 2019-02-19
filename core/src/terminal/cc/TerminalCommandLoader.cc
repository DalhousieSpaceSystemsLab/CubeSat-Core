#include "../header/TerminalCommandLoader.h"

std::vector<TerminalCommand> TerminalCommandLoader::ReadCommands(std::string commandPath) {
    char* commandPathCstr = new char[commandPath.size()+1];
    std::strcpy (commandPathCstr, commandPath.c_str());
    std::vector<TerminalCommand> commandObjects;
    DIR* directory = opendir(commandPathCstr);
    if (directory != NULL) {
        struct dirent *file;
        int index = 0;
        while ((file = readdir(directory)) != NULL) {
            char *match = NULL;
            match = strstr (file->d_name, ".txt");
            if(match) {
                ifstream commandFile;
                char absoluteFile[256] = "";
                strcat(absoluteFile, commandPathCstr);
                strcat(absoluteFile, "/");
                strcat(absoluteFile, file->d_name);
                commandFile.open(absoluteFile);
                if (commandFile.is_open()) {
                    std::string line;
                    while (getline(commandFile, line)) {
                        TerminalCommand command = CreateCommand(line);
                        cout << "Command " << index << " name: " << command.name() << endl;
                        cout << "Command " << index << " description: " << command.description() << endl;
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
    delete[] commandPathCstr;
    return commandObjects;
}

TerminalCommand TerminalCommandLoader::CreateCommand(const std::string& commandText) {
    std::stringstream lineStream(commandText);
    std::string seg;

    std::getline(lineStream, seg, ',');
    std::string name = seg;

    std::getline(lineStream, seg, ',');
    std::string description = seg;

    return TerminalCommand(name, description);
}
