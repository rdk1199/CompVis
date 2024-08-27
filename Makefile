build:
	g++ -c -std=c++17 main.cpp Math/linalg.cpp ImagProc/image.cpp -Wno-narrowing
	g++ -o comp-vis main.o linalg.o image.o -lpng
	./comp-vis
