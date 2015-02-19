all: TweeterApp.o Tweeter.o User.o Tweet.o Streamer.o StreamBuffer.o
	g++ -lpthread -std=c++11 -o TweeterApp StreamBuffer.o TweeterApp.o Tweeter.o User.o Tweet.o Streamer.o

TweeterApp.o: TweeterApp.cpp
	g++ -c -std=c++11 TweeterApp.cpp

Tweeter.o: Tweeter.cpp
	g++ -c -std=c++11 Tweeter.cpp

User.o: User.cpp
	g++ -c -std=c++11 User.cpp

Tweet.o: Tweet.cpp
	g++ -c -std=c++11 Tweet.cpp

Streamer.o: Streamer.cpp
	g++ -c -std=c++11 -lpthread Streamer.cpp

StreamBuffer.o: StreamBuffer.cpp
	g++ -c -std=c++11 StreamBuffer.cpp

clean:
	rm *.o
