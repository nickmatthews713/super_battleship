battle:
	g++ bgame.cpp field.cpp user.cpp feed.cpp -ggdb -o battle -lcurses
clean:
	rm battle
