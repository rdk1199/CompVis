#include <png++/png.hpp>
#include <algorithm>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::vector;

#include "image.h"
#include "../Math/stats.h"

Color col_median(std::vector<Color> colors)
{
	vector<float> r, g, b,a;

	for (int i = 0; i < colors.size(); i++)
	{
		r.push_back(colors[i].r);
		g.push_back(colors[i].g);
		b.push_back(colors[i].b);
		a.push_back(colors[i].a);
	}

	return { median(r), median(g), median(b), median(a) };

}

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

Image::Image(int width, int height, Color col) :
	_width(width),
	_height(height),
	pixels(vector<vector<Color>>(width, vector<Color>(height, col)))
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

	Image out(im1.width(), im1.height());

	for (int i = 0; i < im1.width(); i++)
	{
		for (int j = 0; j < im1.height(); j++)
		{
			out[i][j] = im1.at(i, j) + im2.at(i, j);
		}
	}

	return out;
}

Image operator-(const Image& im1, const Image& im2)
{
	if (im1.width() != im2.width() || im1.height() != im2.height())
	{
		cout << "illegal image operation attempted" << endl;
		exit(0);
	}

	Image out(im1.width(), im1.height());

	for (int i = 0; i < im1.width(); i++)
	{
		for (int j = 0; j < im1.height(); j++)
		{
			out[i][j] = im1.at(i, j) - im2.at(i, j);
		}
	}

	return out;
}

Image operator*(float c, const Image& img)
{
	Image out(img.width(), img.height());

	for (int i = 0; i < img.width(); i++)
	{
		for (int j = 0; j < img.height(); j++)
		{
			out[i][j] = c * img.at(i, j);
		}
	}

	return out;
}

bool Image::in_range(int x, int y) const
{
	return x >= 0 && x < _width&& y >= 0 && y < _height; 
}

Image Image::copy() const 
{
	return Image(pixels);
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

	Image new_px(width, height, col); //initialize new image as solid col

	for (int i = k; i < _width + k; i++) //copy original image into middle of new image
	{
		for (int j = k; j < _height + k; j++)
		{
			new_px[i][j] = pixels[i-k][j-k];
		}
	}

	return new_px;

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

	Image new_px(width, height);

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

	return new_px;
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
	Image out(_width, _height);

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

			out[i][j].r = 255.0 * cum_image[r_val].r;
			out[i][j].g = 255.0 * cum_image[g_val].g;
			out[i][j].b = 255.0 * cum_image[b_val].b;
			out[i][j].a = 255.0 * cum_image[a_val].a;

		}
	}

	return out;
}

Color compute_lin_fil_px_val(int i, int j, Image& padded_img, const Matrix<float>& filter) //helper function for lin_filter
{
	Color col;

	int rows = filter.n_rows();
	int cols = filter.n_cols();

	int pad_amt = std::max(rows, cols);

	for (int k = 0; k < rows; k++)
	{
		for (int l = 0; l < cols; l++)
		{
			col += filter.at(k,l) * padded_img[i - (rows / 2) + k + pad_amt][j - (cols / 2) + l + pad_amt];
		}
	}

	return col;
}

Image Image::lin_filter(const Matrix<float>& filter) const
{
	if (filter.n_cols() % 2 == 0 || filter.n_rows() % 2 == 0 || filter.n_rows() == 0)
	{
		cout << "invalid linear filter matrix; matrix must be nonempty and have odd number of rows and columns" << endl;
		exit(1);
	}

	int pad_amt = std::max(filter.n_cols(), filter.n_rows());

	Image padded_img = clamp_pad(pad_amt);

	Image out(_width, _height);


	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			out[i][j] = compute_lin_fil_px_val(i, j, padded_img, filter);
		}
	}



	return out;
}

Image Image::box_filter(int size) const
{
	size = size % 2 ? size: size + 1;//make the box odd sized

	Matrix<float> box(vector<vector<float>>(size, vector<float>(size, 1.0f/(size * size))));

	return lin_filter(box);


}

Image Image::gauss_blur() const
{

	Matrix<float> gauss_filter(
		{ {1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,36,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1} }

	);

	gauss_filter = (1.0f / 256.0f) * gauss_filter;

	return lin_filter(gauss_filter);
}

Image Image::sobel() const
{
	Matrix<float> sobel(
		{
		{-1,0,1},
		{-2,0,2},
		{-1,0,1}
		}
	);

	sobel = (1.0f / 8) * sobel;

	return lin_filter(sobel);
}

Image Image::corner() const
{

	Matrix<float> corners(
		{
			{1, -2, 1},
			{-2, 4, -2},
			{1, -2, 1}
		}
	);

	corners = 0.25f * corners;

	return lin_filter(corners);
}

Image Image::integral() const
{
	Image out(_width, _height);

	out[0][0] = pixels[0][0]; //left bottom corner

	for (int j = 1; j < _height; j++) //get left edge
	{
		out[0][j] = out[0][j - 1] + pixels[0][j];
	}

	for (int i = 1; i < _width; i++) //get bottom edge
	{
		out[i][0] = out[i - 1][0] + pixels[i][0];
	}

	for (int i = 1; i < _width; i++) //now fill the rest
	{
		for (int j = 1; j < _height; j++)
		{
			out[i][j] = out[i - 1][j] + out[i][j - 1] - out[i - 1][j - 1] + pixels[i][j];
		}
	}

	return out;
}

Image Image::normalize() const
{
	float max = 0.0f;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			max = std::max({ max, pixels[i][j].r, pixels[i][j].g, pixels[i][j].b });
		}
	}

	float scale = 255.0f / max;

	return scale * (*this);
}

Image Image::invert() const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			out[i][j] = pixels[i][j].invert();
		}
	}

	return out;
}

Image Image::sharpen(float alpha) const
{
	return  (1.0f + alpha) * (*this) - alpha * gauss_blur();
}

Image Image::median(int radius) const
{
	if (radius < 0)
	{
		cout << "box radius for median filter must be non-negative";
	}

	if (radius == 0)
	{
		return copy();
	}

	Image padded_img = clamp_pad(radius);

	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{

			vector<Color> nbrhood;

			for (int k = -radius; k <= radius; k++)
			{
				for (int l = -radius; l <= radius; l++)
				{
					nbrhood.push_back(padded_img[i + radius + k][j + radius + l]);
				}

			}

			out[i][j] = col_median(nbrhood);
		}
	}

	return out;
}

//bilateral helpers
double bilateral_domain_kernel(int dx, int dy, float sigma_d)
{
	double sq_dist = dx * dx + dy * dy;

	double out = exp(-sq_dist / (2 * sigma_d * sigma_d));

	return out;

}

double bilateral_range_kernel(Color center_color, Color nbr_color, float sigma_r)
{
	double sq_dist = (nbr_color - center_color).sq_mag();

	double out = exp(-sq_dist / (2 * sigma_r * sigma_r));

	return out;
}

Image Image::bilateral(int radius, float sigma_d, float sigma_r) const
{
	if (radius < 0)
	{
		cout << "ERROR: bilateral filter radius must be non-negative" << endl;
		exit(1);
	}

	Image out(_width, _height);

	Image padded_img = clamp_pad(radius);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			Color weighted_sum = { 0,0,0,0 };
			float total_weight = 0.0f;

			for (int k = -radius; k <= radius; k++)
			{
				for (int l = -radius; l <= radius; l++)
				{
					float bilat_wt_func = bilateral_domain_kernel(k, l, sigma_d) * bilateral_range_kernel(padded_img[i + radius][j + radius], padded_img[i + radius + k][j + radius + l], sigma_r);

					weighted_sum += bilat_wt_func * padded_img[i + radius + k][j + radius + l];
					total_weight += bilat_wt_func;
			
				}
			}

			out[i][j] = (1.0f / total_weight) * weighted_sum;

		}
	}

	return out;

}

Image Image::iter_bilateral(unsigned int iter, int radius, float sigma_d, float sigma_r) const
{
	Image current = *this;

	for (int i = 0; i < iter; i++)
	{
		current = current.bilateral(radius, sigma_d, sigma_r);
	}

	return current;
}

Image Image::max_monochrome() const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			float max = std::max({ pixels[i][j].r, pixels[i][j].g, pixels[i][j].b });
			out[i][j] = { max, max, max, pixels[i][j].a };
		}
	}

	return out;
}

Image Image::mean_monochrome() const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			float avg = mean(vector<float>({ pixels[i][j].r, pixels[i][j].g, pixels[i][j].b }));
			out[i][j] = { avg, avg, avg, pixels[i][j].a };
		}
	}

	return out;
}

Image Image::grayscale() const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			float val = 0.2126 * pixels[i][j].r + 0.7152 * pixels[i][j].g + 0.0722 * pixels[i][j].b;
			out[i][j] = { val,val,val, pixels[i][j].a };
		}
	}

	return out;
}

Image Image::binarize(float threshold) const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			float val = pixels[i][j].r/255.0 >= threshold ? 255.0f : 0.0f;
			out[i][j] = { val, val, val, 255.0f };
		}
	}
	
	return out;
}

int Image::count_ones(int x, int y, int radius) const
{
	int count = 0;
	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			if (in_range(x+i, y+j) && pixels[x + i][y + j].r >= 254.0f) //this should work well enough for binary images
			{
				count++;
			}
		}
	}

	return count;
}

Image Image::dilate(int radius) const
{
	Image out(_width, _height);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			out[i][j] = threshold(count_ones(i, j, radius), 1)* Color{ 255.0f, 255.0f, 255.0f, 0.0f } + Color{0.0f, 0.0f, 0.0f, 255.0f};
		}
	}

	return out;
}


Image Image::erode(int radius) const
{
	Image out(_width, _height);

	int thresh = (2 * radius + 1) * (2 * radius + 1);

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			out[i][j] = threshold(count_ones(i, j, radius), thresh) * Color { 255.0f, 255.0f, 255.0f, 0.0f } + Color{ 0.0f, 0.0f, 0.0f, 255.0f };
		}
	}

	return out;
}

Image Image::majority(int radius) const
{
	Image out(_width, _height);

	int thresh = ((2 * radius + 1) * (2 * radius + 1))/2 + 1;

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			out[i][j] = threshold(count_ones(i, j, radius), thresh) * Color { 255.0f, 255.0f, 255.0f, 0.0f } + Color{ 0.0f, 0.0f, 0.0f, 255.0f };
		}
	}

	return out;
}

Image Image::open(int radius) const
{
	return erode(radius).dilate(radius);
}

Image Image::close(int radius) const
{
	return dilate(radius).erode(radius);
}