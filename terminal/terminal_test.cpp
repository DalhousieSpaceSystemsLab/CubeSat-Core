#include <iostream>
#include "TerminalCommandLoader.h"

using namespace std;

int main() {

	printf("-----------Starting program!-----------\n");

	TerminalCommandLoader commandLoader;
	char path[256];
	printf("Please enter the absolute path of your commands directory:\n");
	cin >> path;
	std::vector<TerminalCommand> commands = commandLoader.readCommands(path);

	if (commands.size()) {
		printf("\n-----------Begin Testing!-----------\n");

		for(int i=0; i < commands.size(); i++) {
			cout << commands[i].getName() << endl;
			cout << commands[i].getDescription() << endl;
			cout << commands[i].toString() << endl;
		}
	} else {
		printf("No commands found.\n");
	}
}
