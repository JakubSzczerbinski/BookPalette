

CXXFLAGS=-std=c++14 -Wall -Wextra -Og -ggdb `pkg-config cairo --libs`
SOURCES=main.cpp

book_palette: main.cpp
	g++ ${CXXFLAGS} ${SOURCES} -o book_palette

clean:
	rm -f book_palette