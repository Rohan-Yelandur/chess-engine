main: src/main.cpp src/board.cpp include/board.h
	g++ src/main.cpp src/board.cpp -I include -o main.exe

run: main
	./main.exe

clean:
	del *.exe