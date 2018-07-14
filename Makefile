CXXFLAGS=-std=c++14 -I${HOME}/podofo/include -L${HOME}/podofo/lib -lpodofo -Wall -Wextra -Og -ggdb `pkg-config cairo --libs` -lpthread -lfreetype -lz -lfontconfig -ljpeg -ltiff
SOURCES=main.cpp

book_palette: main.cpp
	g++ ${SOURCES} ${CXXFLAGS} -o book_palette

clean:
	rm -f book_palette
