#include <png++/png.hpp>

#include "image.h"

Image::Image(std::string file)
{
	png::image<png::rgba_pixel> img(file);

	width = img.get_width();
	height = img.get_height();

	pixels.resize(width);

	for (int i = 0; i < width; i++)
	{
		pixels[i].resize(height);

		for (int j = 0; j < height; j++)
		{
			png::rgba_pixel px = img[j][i];
			pixels[i][j] = { px.red, px.green, px.blue, px.alpha };
		}

	}

}

void Image::save(std::string file)
{
	png::image<png::rgba_pixel> out_img(width, height);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int r = pixels[i][j].r;
			int g = pixels[i][j].g;
			int b = pixels[i][j].b;
			int a = pixels[i][j].a;



			out_img[j][i] = png::rgba_pixel(r,g,b,a);
		}
	}

	out_img.write(file);

}