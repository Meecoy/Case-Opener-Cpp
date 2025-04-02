all:
	g++ -o main.exe main.cpp utilities.cpp -lsimdjson

clean:
	rm -f main

