test:
	g++ -O2 -c -std=c++17 *.cpp */*.cpp -Wno-narrowing
	g++ -o comp-vis *.o -lpng
	./comp-vis

editor:  Editor/*.py
	 python3 Editor/main.py


clean: 
	rm -f *.o

.PHONY: editor
