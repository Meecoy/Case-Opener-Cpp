all:
	g++ -o main main.cpp utilities.cpp -lsimdjson

clean:
	rm -f main

