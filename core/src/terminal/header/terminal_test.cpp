#include <iostream>
#include "TerminalCommandLoader.h"

using namespace std;

int main() {

	printf("-----------Starting program!-----------\n");

	TerminalCommandLoader commandLoader;
	char *path;
	printf("Please enter the absolute path of your commands directory:\n");
	cin >> path;
	std::vector<TerminalCommand> commands = commandLoader.ReadCommands(path);

	if (commands.size()) {
		printf("\n-----------Begin Testing!-----------\n");

		for(int i=0; i < commands.size(); i++) {
			cout << commands[i].name() << endl;
			cout << commands[i].description() << endl;
			cout << commands[i].ToString() << endl;
		}
	} else {
		printf("No commands found.\n");
	}
}
