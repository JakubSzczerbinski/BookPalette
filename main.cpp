#include <numeric>
#include <iostream>
#include <vector>
#include <map>
#include <boost/optional.hpp>
#include <cairo/cairo.h>


struct RGB
{
	static RGB fromInt(unsigned r, unsigned g, unsigned b)
	{
		return RGB{r, g, b};
	}
	static RGB fromDouble(double r, double g, double b)
	{
		return RGB{(unsigned)r * 255, (unsigned)g * 255, (unsigned)b * 255};
	}
	double r_d() { return r/255.0; }
	double g_d() { return g/255.0; }
	double b_d() { return b/255.0; }
	unsigned r, g, b;
};

using maybeRGB = boost::optional<RGB>;

boost::optional<RGB> colorNameToRgb(std::string color_name)
{
	static const std::map<std::string, RGB> colorNameMap =
	{ {"red", RGB::fromInt(230, 25, 75)}
	, {"green", RGB::fromInt(60, 180, 75)}
	, {"yellow", RGB::fromInt(255, 225, 25)}
	, {"blue", RGB::fromInt(0, 130, 200)}
	, {"orange", RGB::fromInt(245, 130, 48)}
	, {"purple", RGB::fromInt(145, 30, 180)}
	, {"cyan", RGB::fromInt(70, 240, 240)}
	, {"magenta", RGB::fromInt(240, 50, 230)}
	, {"lime", RGB::fromInt(210, 245, 60)}
	, {"pink", RGB::fromInt(250, 190, 190)}
	, {"teal", RGB::fromInt(0, 128, 128)}
	, {"lavender", RGB::fromInt(230, 190, 255)}
	, {"brown", RGB::fromInt(170, 110, 40)}
	, {"beige", RGB::fromInt(255, 250, 200)}
	, {"maroon", RGB::fromInt(128, 0, 0)}
	, {"mint", RGB::fromInt(170, 255, 195)}
	, {"olive", RGB::fromInt(128, 128, 0)}
	, {"coral", RGB::fromInt(255, 215, 180)}
	, {"navy", RGB::fromInt(0, 0, 128)}
	, {"grey", RGB::fromInt(128, 128, 128)}
	, {"white", RGB::fromInt(255, 255, 255)}
	, {"black", RGB::fromInt(0, 0, 0)}
	};

	std::transform(color_name.begin(), color_name.end(), color_name.begin(), ::tolower);
	auto it = colorNameMap.find(color_name);
	return (it != colorNameMap.end() ? maybeRGB(it->second) : boost::none);
};

std::vector<std::pair<RGB, unsigned>> countColors(std::string path_to_file)
{
	return std::vector<std::pair<RGB, unsigned>>();
}

void salesmanSort(std::vector<std::pair<RGB, unsigned>> sequence)
{

}

void createPaletteImage(std::vector<std::pair<RGB, unsigned>> palette)
{
	constexpr unsigned strip_len = 1000;
	constexpr unsigned strip_width = 50;

	cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 50);
	cairo_t* cr = cairo_create (surface);
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_set_line_width (cr, strip_width/2.0);
	cairo_move_to (cr, 0, strip_width/2.0);

	auto sum = std::accumulate(palette.begin(), palette.end(), 0u, 
		[](unsigned sum, auto&& p) { return sum + p.second; });

	std::cerr << "Sum : " << sum << std::endl;

	auto partial_sum = 0u;
	for (auto&& pair : palette)
	{
		auto color = pair.first;
		auto value = pair.second;

		auto start = (strip_len * partial_sum) / sum;
		auto end = (strip_len * (partial_sum + value)) / sum;
		partial_sum += value;

		std::cerr << "Start: " << start << ", end: " << end << std::endl;

		cairo_move_to (cr, start, strip_width/2.0);
		cairo_set_source_rgba (cr, color.r_d(), color.g_d(), color.b_d(), 1);
		cairo_line_to (cr, end, strip_width/2.0);
		cairo_stroke (cr);
	}

	cairo_surface_write_to_png(surface, "test_image.png");
}

int main()
{

	createPaletteImage(
	{ {*colorNameToRgb("red"), 10}
	, {*colorNameToRgb("yellow"), 2}
	, {*colorNameToRgb("pink"), 32}
	, {*colorNameToRgb("cyan"), 32}
	, {*colorNameToRgb("purple"), 32}
	});
}
