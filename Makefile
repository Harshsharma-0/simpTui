main:main.cpp ./include/*.hpp
	 g++ -std=c++20 main.cpp -o main -lncurses -lpanel -O3

run: main
	./main
