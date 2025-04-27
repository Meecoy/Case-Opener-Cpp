all:
	g++ -std=c++17 -Ilibs/simdjson/include -Ilibs/simdjson/src main.cpp utilities.cpp libs/simdjson/src/simdjson.cpp -o case_opener.exe

clean:
	rm -f case_opener.exe

