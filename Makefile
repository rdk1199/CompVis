build:
	g++ -O2 -c -std=c++17 *.cpp */*.cpp -Wno-narrowing
	g++ -o comp-vis *.o -lpng
	./comp-vis


clean: 
	rm -f *.o
