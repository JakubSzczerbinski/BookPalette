

CXXFLAGS=-std=c++11 -Wall -Wextra -Og -ggdb
SOURCES=main.cpp

book_palette: main.cpp
	g++ ${CXXFLAGS} ${SOURCES} -o book_palette

clean:
	rm -f book_palette