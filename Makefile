build:
	g++ -c main.cpp Math/linalg.cpp ImagProc/image.cpp
	g++ -o comp-vis main.o linalg.o image.o -lpng
	./comp-vis
