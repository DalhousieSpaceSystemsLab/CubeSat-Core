
output: terminal_test.o
	g++ terminal_test.o -o output
	
terminal_test.o: test/terminal_test.cpp src/terminal/header/GroundStation.h src/terminal/header/TerminalCommand.h src/terminal/header/TerminalCommandLoader.h src/terminal/header/Terminal.h
	g++ -c test/terminal_test.cpp

clean:
	rm *.o output
