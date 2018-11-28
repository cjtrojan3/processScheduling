all: helpers.o executeMFQS.o executeRTS.o executeWHS.o main.o
	g++ -std=c++11 -o run helpers.o executeMFQS.o executeRTS.o executeWHS.o main.o

helpers.o: helpers.cpp
	g++ -std=c++11 -c helpers.cpp

executeMFQS.o: executeMFQS.cpp
	g++ -std=c++11 -c executeMFQS.cpp

executeRTS.o: executeRTS.cpp
	g++ -std=c++11 -c executeRTS.cpp

executeWHS.o: executeWHS.cpp
	g++ -std=c++11 -c executeWHS.cpp

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

clean:
	rm main.o executeMFQS.o executeRTS.o executeWHS.o helpers.o
