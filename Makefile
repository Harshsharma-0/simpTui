hello:helloWorld.cpp ./include/*.hpp
	 g++ -std=c++20 helloWorld.cpp -o helloWorld -lncurses -lpanel -O3

window:windowExample.cpp ./include/*.hpp
	g++ -std=c++20 windowExample.cpp -o windowExample -lncurses -lpanel -O3


run: hello
	./helloWorld
