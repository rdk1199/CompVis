#include <png++/png.hpp>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

#include "image.h"

Image::Image(std::string file)
{
	png::image<png::rgba_pixel> img(file);

	_width = img.get_width();
	_height = img.get_height();

	pixels.resize(_width);

	for (int i = 0; i < _width; i++)
	{
		pixels[i].resize(_height);

		for (int j = 0; j < _height; j++)
		{
			png::rgba_pixel px = img[j][i];
			pixels[i][j] = { px.red, px.green, px.blue, px.alpha };
		}

	}

}

void Image::save(std::string file)
{
	png::image<png::rgba_pixel> out_img(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			int r = pixels[i][j].r;
			r = std::clamp(r, 0, 255);

			int g = pixels[i][j].g;
			g = std::clamp(g, 0, 255);

			int b = pixels[i][j].b;
			b = std::clamp(b, 0, 255);

			int a = pixels[i][j].a;
			a = std::clamp(a, 0, 255);



			out_img[j][i] = png::rgba_pixel(r,g,b,a);
		}
	}

	out_img.write(file);

}

Image::Image(const std::vector<std::vector<Color>>& px) :
	_width(px.size()),
	_height(px.size() ? px[0].size() : 0),
	pixels(px)
{}

void Image::gain(float val)
{
	for (int i = 0; i <_width; i++)
	{
		for (int j = 0; j <_height; j++)
		{
			pixels[i][j] *= val;
		}
	}
}

void Image::gain(Color val)
{
	for (int i = 0; i <_width; i++)
	{
		for (int j = 0; j <_height; j++)
		{
			pixels[i][j] *= val;
		}
	}
}

void Image::bias(float val)
{
	for (int i = 0; i <_width; i++)
	{
		for (int j = 0; j <_height; j++)
		{
			pixels[i][j] += val;
		}
	}
}

void Image::bias(Color val)
{
	for (int i = 0; i <_width; i++)
	{
		for (int j = 0; j <_height; j++)
		{
			pixels[i][j] += val;
		}
	}
}

void Image::gamma_correct(float gamma)
{
	for (int i = 0; i <_width; i++)
	{
		for (int j = 0; j <_height; j++)
		{
			pixels[i][j] = pow(pixels[i][j], 1.0f/gamma);
		}
	}
}

Image operator+(const Image& im1, const Image& im2)
{
	if (im1.width() != im2.width() || im1.height() != im2.height())
	{
		cout << "illegal image operation attempted" << endl;
		exit(0);
	}

	vector<vector<Color>> new_px(im1.width(), vector<Color>(im1.height()));

	for (int i = 0; i < im1.width(); i++)
	{
		for (int j = 0; j < im1.height(); j++)
		{
			new_px[i][j] = im1.at(i, j) + im2.at(i, j);
		}
	}

	return Image(new_px);
}

Image operator*(float c, const Image& img)
{
	vector<vector<Color>> new_px(img.width(), vector<Color>(img.height()));

	for (int i = 0; i < img.width(); i++)
	{
		for (int j = 0; j < img.height(); j++)
		{
			new_px[i][j] = c * img.at(i, j);
		}
	}

	return Image(new_px);
}

Image Image::blend(const Image& im1, const Image& im2, float alpha)
{
	alpha = std::clamp(alpha, 0.0f, 1.0f);

	return alpha * im1 + (1.0f - alpha) * im2;
}