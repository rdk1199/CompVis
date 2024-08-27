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


Image Image::const_pad(int k, Color col) const
{
	if (k < 0)
	{
		cout << "cannot pad image negatively" << endl;
		exit(0);
	}

	int width = _width + 2 * k;
	int height = _height + 2 * k;

	vector<vector<Color>> new_px(width, vector<Color>(height, col)); //initialize new image as solid col

	for (int i = k; i < _width + k; i++) //copy original image into middle of new image
	{
		for (int j = k; j < _height + k; j++)
		{
			new_px[i][j] = pixels[i-k][j-k];
		}
	}

	return Image(new_px);

}


Image Image::clamp_pad(int k) const
{
	if (k < 0)
	{
		cout << "cannot pad image negatively" << endl;
		exit(0);
	}

	int width = _width + 2 * k;
	int height = _height + 2 * k;

	vector<vector<Color>> new_px(width, vector<Color>(height));

	for (int i = k; i < _width + k; i++) //copy original image into middle of new image and add clamping on top and bottom
	{
		for (int j = k; j < _height + k; j++)
		{
			new_px[i][j] = pixels[i - k][j - k];
		}

		for (int j = 0; j < k; j++)
		{
			new_px[i][j] = pixels[i - k][0]; //bottom
			new_px[i][_height + k + j] = pixels[i - k][_height - 1]; //top
		}

	}

	
	//left side
	for (int i = 0; i < k; i++)
	{
		for (int j = k; j < _height + k; j++)
		{
			new_px[i][j] = pixels[0][j - k];
		}

		for (int j = 0; j < k; j++) //lower left corner
		{
			new_px[i][j] = pixels[0][0];
		}

		for (int j = _height + k; j < _height + 2 * k; j++) //upper left corner
		{
			new_px[i][j] = pixels[0][_height - 1];
		}


	}

	
	//right side
	for (int i = _width + k; i < _width + 2*k; i++)
	{
		for (int j = k; j < _height + k; j++)
		{
			new_px[i][j] = pixels[_width - 1][j - k];
		}

		for (int j = 0; j < k; j++) //lower right corner
		{
			new_px[i][j] = pixels[_width - 1][0];
		}

		for (int j = _height + k; j < _height + 2 * k; j++) //upper right corner
		{
			new_px[i][j] = pixels[_width-1][_height - 1];
		}

	}

	return Image(new_px);
}

/*
Image Image::const_pad_to_size(int width, int height, Color col) const
{

}

Image Image::clamp_pad_to_size(int width, int height) const
{

}
*/

