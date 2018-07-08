
#include <iostream>
#include <vector>
#include <map>
#include <boost/optional.hpp>

struct RGB
{
	unsigned r, g, b;
};

boost::optional<RGB> colorNameToRgb(std::string text)
{
	return boost::none;
};

std::map<RGB, unsigned> countColors(std::string path_to_file)
{
	return std::map<RGB, unsigned>();
}

void createPaletteImage(std::map<RGB, unsigned> palette);

int main()
{
	std::cout << "Hello XD" << std::endl;
}