main: src/main.cpp src/board.cpp src/utils.cpp include/board.h
	g++ src/main.cpp src/board.cpp src/utils.cpp -I include -o main.exe

run: main
	./main.exe

clean:
	del *.exe