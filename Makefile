build:
	g++ -c -std=c++17 *.cpp Math/*.cpp ImagProc/*.cpp -Wno-narrowing
	g++ -o comp-vis *.o -lpng
	./comp-vis


clean: 
	rm -f *.o