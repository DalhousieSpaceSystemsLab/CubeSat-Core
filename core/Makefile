CC=g++
CFLAGS= -c -std=c++17

output: terminal_test.o
	$(CC) terminal_test.o -o output
	
terminal_test.o: src/terminal/header/terminal_test.cpp GroundStation.o TerminalCommand.o TerminalCommandLoader.o Terminal.o 
	$(CC) $(CFLAGS)  src/terminal/header/terminal_test.cpp obj/GroundStation.o obj/TerminalCommand.o obj/TerminalCommandLoader.o obj/Terminal.o -o terminal_test.o

GroundStation.o: src/terminal/header/GroundStation.h
	$(CC) $(CFLAGS) src/terminal/header/GroundStation.h -o obj/GroundStation.o

TerminalCommand.o: src/terminal/header/TerminalCommand.h 
	$(CC) $(CFLAGS) src/terminal/header/TerminalCommand.h -o obj/TerminalCommand.o

TerminalCommandLoader.o: src/terminal/header/TerminalCommandLoader.h 
	$(CC) $(CFLAGS) src/terminal/header/TerminalCommandLoader.h -o obj/TerminalCommandLoader.o

Terminal.o: src/terminal/header/Terminal.h
	$(CC) $(CFLAGS) src/terminal/header/Terminal.h -o obj/Terminal.o

clean:
	rm *.o output
