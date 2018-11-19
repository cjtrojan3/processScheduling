all: helpers executeMFQS executeRTS executeWHS main
	g++ -o run helpers.o executeMFQS.o executeRTS.o executeWHS.o main.o

helpers: helpers.cpp
	g++ -c helpers.cpp

executeMFQS: executeMFQS.cpp
	g++ -c executeMFQS.cpp

executeRTS: executeRTS.cpp
	g++ -c executeRTS.cpp

executeWHS: executeWHS.cpp
	g++ -c executeWHS.cpp

main: main.cpp
	g++ -c main.cpp

clean:
	rm main.o executeMFQS.o executeRTS.o executeWHS.o helpers.o
