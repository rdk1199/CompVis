
build:
	g++ -c main.cpp Math/linalg.cpp
	g++ -o comp-vis main.o linalg.o
	./comp-vis
