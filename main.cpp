#include <boost/optional.hpp>

#include <cairo/cairo.h>
#include <cmath>

#include <podofo/podofo.h>

#include <iostream>
#include <map>
#include <numeric>
#include <vector>

struct RGB
{
	static RGB fromInt(int r, int g, int b)
	{
		return RGB{r, g, b};
	}
	static RGB fromDouble(double r, double g, double b)
	{
		return RGB{(int)r * 255, (int)g * 255, (int)b * 255};
	}
	double r_d() { return r/255.0; }
	double g_d() { return g/255.0; }
	double b_d() { return b/255.0; }
	int r, g, b;
};

inline bool operator < (const RGB& lhs, const RGB& rhs)
{
	if (lhs.r != rhs.r)
		return lhs.r < rhs.r;

	if (lhs.g != rhs.g)
		return lhs.g < rhs.g;

	return lhs.b < lhs.b;
}

unsigned distance(const RGB& lhs, const RGB& rhs)
{
	int r_delta = lhs.r - rhs.r;
	int g_delta = lhs.g - rhs.g;
	int b_delta = lhs.b - rhs.b;

	return sqrt(r_delta*r_delta + g_delta*g_delta + b_delta*b_delta);
}

unsigned distances(const std::vector<RGB>& vec)
{
	unsigned sum = 0;
	for (int i = 1; i < vec.size(); i++)
	{
		sum += distance(vec[i-1], vec[i]);
	}
	return sum;
}

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

std::map<RGB, unsigned> countColors(std::string path_to_file)
{
	return std::map<RGB, unsigned>();
}

template <typename T> 
std::vector<T> permutation(T vec)
{
	if (vec.size() == 0)
		return {{}};
	std::vector<T> result;
	for (int i = 0; i < vec.size(); i++)
	{
		T new_vec(vec.size() - 1);
		for (int j = 0; j < i; j++)
		{
			new_vec[j] = vec[j];
		}

		for (int j = i + 1; j < vec.size(); j++)
		{
			new_vec[j - 1] = vec[j];
		}

		auto subresult = permutation(new_vec);

		for (auto&& r : subresult)
		{
			r.push_back(vec[i]);
			result.push_back(r);
		}
	}
	return result;
}

std::vector<RGB> salesmanSort(std::vector<RGB> sequence)
{
	auto sequences = permutation(sequence);
	auto it = std::min_element(sequences.begin(), sequences.end(), 
		[](auto&& lhs, auto&& rhs)
		{
			return distances(lhs) < distances(rhs);
		});
	return *it;
}

void createPaletteImage(std::map<RGB, unsigned> palette)
{
	constexpr unsigned strip_len = 1000;
	constexpr unsigned strip_width = 50;

	std::vector<RGB> colors;
	for (auto&& pair : palette)
	{
		auto color = pair.first;
		colors.push_back(color);
	}
	colors = salesmanSort(colors);

	cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 50);
	cairo_t* cr = cairo_create (surface);
	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_set_line_width (cr, strip_width);
	cairo_move_to (cr, 0, strip_width);

	auto sum = std::accumulate(palette.begin(), palette.end(), 0u, 
		[](unsigned sum, auto&& p) { return sum + p.second; });

	std::cerr << "Sum : " << sum << std::endl;

	auto partial_sum = 0u;
	for (auto&& color : colors)
	{
		auto value = palette[color];

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
	{ { RGB::fromInt(230, 25, 75), 1}
	, { RGB::fromInt(60, 180, 75), 1}
	, { RGB::fromInt(255, 225, 25), 1}
	, { RGB::fromInt(0, 130, 200), 1}
	, { RGB::fromInt(245, 130, 48), 1}
	, { RGB::fromInt(145, 30, 180), 1}
	, { RGB::fromInt(70, 240, 240), 1}
	, { RGB::fromInt(240, 50, 230), 1}
	, { RGB::fromInt(210, 245, 60), 1}
	// , { RGB::fromInt(250, 190, 190), 1}
	// , { RGB::fromInt(0, 128, 128), 1}
	// , { RGB::fromInt(230, 190, 255), 1}
	// , { RGB::fromInt(170, 110, 40), 1}
	// , { RGB::fromInt(255, 250, 200), 1}
	// , { RGB::fromInt(128, 0, 0), 1}
	// , { RGB::fromInt(170, 255, 195), 1}
	// , { RGB::fromInt(128, 128, 0), 1}
	// , { RGB::fromInt(255, 215, 180), 1}
	// , { RGB::fromInt(0, 0, 128), 1}
	// , { RGB::fromInt(128, 128, 128), 1}
	// , { RGB::fromInt(255, 255, 255), 1}
	// , { RGB::fromInt(0, 0, 0), 1}
	});
}
