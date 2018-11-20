all: helpers executeMFQS executeRTS executeWHS main
	g++ -std=c++11 -o run helpers.o executeMFQS.o executeRTS.o executeWHS.o main.o

helpers: helpers.cpp
	g++ -std=c++11 -c helpers.cpp

executeMFQS: executeMFQS.cpp
	g++ -std=c++11 -c executeMFQS.cpp

executeRTS: executeRTS.cpp
	g++ -std=c++11 -c executeRTS.cpp

executeWHS: executeWHS.cpp
	g++ -std=c++11 -c executeWHS.cpp

main: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm main.o executeMFQS.o executeRTS.o executeWHS.o helpers.o
