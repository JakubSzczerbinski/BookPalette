CXXFLAGS=-std=c++14 -Wall -Wextra -Og -ggdb `pkg-config cairo --libs` -Wl,-t
SOURCES=main.cpp

book_palette: main.cpp
	g++ ${SOURCES} ${CXXFLAGS} -o book_palette

clean:
	rm -f book_palette
