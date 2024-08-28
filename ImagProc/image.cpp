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

vector<ImgHistEntry> Image::histogram() const
{
	vector<ImgHistEntry> out(256);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			int r_val = static_cast<int>(std::round(pixels[i][j].r));
			int g_val = static_cast<int>(std::round(pixels[i][j].g));
			int b_val = static_cast<int>(std::round(pixels[i][j].b));
			int a_val = static_cast<int>(std::round(pixels[i][j].a));

			r_val = std::clamp(r_val, 0, 255);
			g_val = std::clamp(g_val, 0, 255);
			b_val = std::clamp(b_val, 0, 255);
			a_val = std::clamp(a_val, 0, 255);

			out[r_val].r++;
			out[g_val].g++;
			out[b_val].b++;
			out[a_val].a++;

		}
	}

	return out;
}

vector<ImgHistEntry> Image::cum_dist() const
{
	vector<ImgHistEntry> out(256);
	vector<ImgHistEntry> hist = histogram();

	unsigned long int N = _width * _height; 

	out[0] =  hist[0];

	for (int i = 1; i < out.size(); i++)
	{
		out[i] = out[i - 1] + hist[i];
	}
	
	for (int i = 0; i < out.size(); i++)
	{
		out[i] *= 1.0f / N;
	}
		
	return out;
}

Image Image::hist_equalize() const
{
	vector<vector<Color>> new_px(_width, vector<Color>(_height));

	vector<ImgHistEntry> cum_image = cum_dist();

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			int r_val = static_cast<int>(std::round(pixels[i][j].r));
			int g_val = static_cast<int>(std::round(pixels[i][j].g));
			int b_val = static_cast<int>(std::round(pixels[i][j].b));
			int a_val = static_cast<int>(std::round(pixels[i][j].a));

			r_val = std::clamp(r_val, 0, 255);
			g_val = std::clamp(g_val, 0, 255);
			b_val = std::clamp(b_val, 0, 255);
			a_val = std::clamp(a_val, 0, 255);

			new_px[i][j].r = 255.0 * cum_image[r_val].r;
			new_px[i][j].g = 255.0 * cum_image[g_val].g;
			new_px[i][j].b = 255.0 * cum_image[b_val].b;
			new_px[i][j].a = 255.0 * cum_image[a_val].a;

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

