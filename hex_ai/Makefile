# ASG 5 Makefile

#just run make and it'll clean out the directory, compile, and run it for you
run:
	make clean
	make hex
	hex
	
test:
	make clean
	make hex
	hex <quickgame
	
hex: hex.o Graph.o List.o HexAI.o
	g++ -O3 -o hex hex.o Graph.o List.o HexAI.o

hex.o : hex.cpp Graph.h List.h
	g++ -O3 -c -Wall hex.cpp

Graph.o : Graph.cpp Graph.h List.h
	g++ -O3 -c -Wall Graph.cpp

List.o : List.cpp List.h
	g++ -O3 -c -Wall List.cpp
	
HexAI.o : HexAI.cpp HexAI.h
	g++ -O3 -c -Wall HexAI.cpp

clean :
	rm -f hex *.o
