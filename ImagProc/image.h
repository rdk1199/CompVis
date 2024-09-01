#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "../Math/linalg.h"

struct Color
{
	float r = 0, g = 0, b = 0, a = 0;

	float sq_mag() { return (r * r + g * g + b * b + a * a)/(255.0f * 255.0f); } //magnitude of RGBA values in 0-1 scale
	Color invert() const { return { 255.0 - r, 255.0 - g, 255.0 - b, a }; }

	static Color white() { return { 255.0f, 255.0f, 255.0f, 255.0f }; }
	static Color black() { return { 0.0f, 0.0f, 0.0f, 255.0f}; }
	static Color gray(float val, float alpha = 255.0f) { return { val, val, val, alpha }; } //make grayscale color with given value and alpha
};


inline Color operator+(const Color& a, const Color& b)
{
	return { a.r + b.r, a.g + b.g, a.b + b.b, std::max(a.a , b.a) };
}

inline Color operator+(const Color& a, float b)
{
	return { a.r + b, a.g + b, a.b + b, a.a};
}

inline Color operator+(float b, const Color& a)
{
	return { a.r + b, a.g + b, a.b + b, a.a};
}

inline Color operator+=(Color& a, const Color& b)
{
	a = a + b;
	return a;
}

inline Color operator+=(Color& a, float b)
{
	a = a + b;
	return a;
}


inline Color operator-(const Color& a, const Color& b)
{
	return { a.r - b.r, a.g - b.g, a.b - b.b, std::min(a.a, b.a) };
}

inline Color operator*(const Color& a, const Color& b)
{
	return { a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a };
}


inline Color operator*(float b, const Color& a)
{
	return { a.r * b, a.g * b, a.b * b, a.a };
}

inline Color operator*(const Color& a, float b)
{
	return { a.r * b, a.g * b, a.b * b, a.a };
}

inline Color operator*=(Color& a, const Color& b)
{
	a = a * b;
	return a;
}

inline Color operator*=(Color& a, float b)
{
	a = a * b;
	return a;
}

inline std::ostream& operator<<(std::ostream& stream, const Color& col)
{
	stream << col.r << " " << col.g << " " << col.b << " " << col.a;
	return stream;
}

Color col_median(std::vector<Color> colors);

/*
inline Color operator*=(float b, Color& a)
{
	a = a * b;
	return a;
}*/


inline Color pow(const Color& c, float g)
{
	return { std::pow(c.r, g), std::pow(c.g, g),std::pow(c.b, g),c.a };
}

typedef Color ImgHistEntry;

enum class ImagePadType {zero, clamp};

class Image
{
private:

	std::vector<std::vector<Color>> pixels;

	int _width;
	int _height;

public:

	// I/O
	Image(std::string file); //load image from file
	void save(std::string file); //save image to disk

	//constructors
	Image(const std::vector<std::vector<Color>>& px); //create image from bitmap
	Image(int width, int height, Color col = { 0,0,0,0 }); //create constant image of given size and color  

	//properties
	int width() const { return _width; }
	int height() const { return _height; }
	bool in_range(int x, int y) const; //return true iff x ,y is in range 

	//accessors
	Color at(int i, int j) const { return pixels[i][j]; }
	std::vector<Color>& operator[](int i) { return pixels[i]; }
	Color clamp_at(int i, int j) const; //returns pixel value if (i,j) is in range, else returns the clamped value (i.e. the same as if the image were clamp padded)

	//point processes
	void gain(float val);
	void gain(Color val);
	void bias(float val);
	void bias(Color val);
	void gamma_correct(float gamma);



	//padding
	Image const_pad(int k, Color col = { 0,0,0,0 }) const; //adds a constant color pad of width k around the image 
	Image clamp_pad(int k) const; //adds clamp padding of width k

	/*
	//pad uniformly until image is desired size (keep original image centered) <- maybe implement this later if needed
	Image const_pad_to_size(int width, int height, Color col = { 0,0,0,0 }) const; 
	Image clamp_pad_to_size(int width, int height) const;
	*/

	static Image blend(const Image& im1, const Image& im2, float alpha);


	//image stats
	std::vector<ImgHistEntry> histogram() const;
	std::vector<ImgHistEntry> cum_dist() const; //normalized cumulative distribution function

	//filter
	Image copy() const; //returns identical copy
	
	Image hist_equalize() const; //histogram equalization
	Image lin_filter(const Matrix<float>& filter) const; //clamp pads image, then applies linear filter - filter must be square matrix of odd size
	
	//FILTERS; all filters clamp pad the image before being applied

	//common linear filters
	Image box_filter(int box_size) const;
	Image gauss_blur() const;
	Image sobel() const;
	Image corner() const;


	//misc operations
	Image integral() const; //return integral image
	Image normalize() const; //scale color values so max is 255
	Image invert() const; //inverts color channels

	//non-linear filters
	Image sharpen(float alpha = 0.25f) const;
	Image median(int radius) const; //median filter with given radius (slow)
	Image bilateral(int radius, float sigma_d, float sigma_r) const;
	Image iter_bilateral(unsigned int iter, int radius, float sigma_d, float sigma_r) const;

	//monochromatize
	Image max_monochrome() const; //makes black and white copy -> takes the max of the R,G,B channels
	Image mean_monochrome() const; //makes black and white copy, taking the mean of each channel
	Image grayscale() const; //makes black and white copy using proper weighting to account for luminance

	//binary image processing 
	Image binarize(float threshold) const; //turn grayscale images into binary with given threshold (between 0 and 1)

	//helper for BINARY IMAGES
	bool is_one(int x, int y) const; 
	int count_ones(int x, int y, int radius) const; //count how many "1" pixels are within [x - radius, x + radius] * [y - radius, y+radius]

	//morphological operators for binary images
	Image dilate(int radius) const;
	Image erode(int radius) const;
	Image majority(int radius) const;
	Image open(int radius) const;
	Image close(int radius) const; 

	//distance transforms
	Image manhattan_dist_trans() const;
	//

	//scaling
	Image bicubic_interpolate(int rate, float a = -1.0f) const; //scale up image by factor of rate (bicubic interpolation)
	Image decimate(int rate) const; //scale down image by factor of rate
};

Image operator+(const Image& im1, const Image& im2);
Image operator-(const Image& im1, const Image& im2);
Image operator*(float c, const Image& img);


inline Image operator*(const Matrix<float>& filter, const Image& img) //convolve filter with image
{
	return img.lin_filter(filter);
}