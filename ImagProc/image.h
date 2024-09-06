#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "../Math/linalg.h"

struct Color
{
	double r = 0, g = 0, b = 0, a = 0;

	double sq_mag() const { return (r * r + g * g + b * b + a * a); } //magnitude of RGBA values in 0-1 scale
	double mag() const { return std::sqrt(sq_mag()); }
	Color invert() const { return { 255.0 - r, 255.0 - g, 255.0 - b, a }; }
	Color abs() const { return { std::abs(r), std::abs(g), std::abs(b), std::abs(a) }; }

	static Color white() { return { 255.0f, 255.0f, 255.0f, 255.0f }; }
	static Color black() { return { 0.0f, 0.0f, 0.0f, 255.0f}; }
	static Color red() { return { 255.0f, 0.0f, 0.0, 255.0f }; }
	static Color green() { return { 0.0f, 255.0f, 0.0, 255.0f }; }
	static Color blue() { return { 0.0f, 0.0f, 255.0, 255.0f }; }
	static Color gray(double val, double alpha = 255.0f) { return { val, val, val, alpha }; } //make grayscale color with given value and alpha
};


inline Color operator+(const Color& a, const Color& b)
{
	return { a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a };
}

inline Color operator+(const Color& a, double b)
{
	return { a.r + b, a.g + b, a.b + b, a.a + b};
}

inline Color operator+(double b, const Color& a)
{
	return a + b;
}

inline Color operator+=(Color& a, const Color& b)
{
	a = a + b;
	return a;
}

inline Color operator+=(Color& a, double b)
{
	a = a + b;
	return a;
}


inline Color operator-(const Color& a, const Color& b)
{
	return { a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a };
}

inline Color operator*(const Color& a, const Color& b)
{
	return { a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a };
}

inline Color operator/(const Color& a, const Color& b)
{
	return { a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a };
}

inline Color operator/(const Color& a, double b)
{
	return { a.r / b, a.g / b, a.b / b, a.a / b };
}

inline Color operator*(double b, const Color& a)
{
	return { a.r * b, a.g * b, a.b * b, a.a * b };
}

inline Color operator*(const Color& a, double b)
{
	return b * a;
}


inline double abs(const Color& a)
{
	return a.mag();
}


inline Color operator*=(Color& a, const Color& b)
{
	a = a * b;
	return a;
}

inline Color operator*=(Color& a, double b)
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
inline Color operator*=(double b, Color& a)
{
	a = a * b;
	return a;
}*/


inline Color pow(const Color& c, double g)
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
	void gain(double val);
	void gain(Color val);
	void bias(double val);
	void bias(Color val);
	void gamma_correct(double gamma);
	void abs(); //takes absolute value of every color (in case some color values are negative) -> mainly for measuring relative difference between two images



	//padding
	Image const_pad(int k, Color col = { 0,0,0,0 }) const; //adds a constant color pad of width k around the image 
	Image clamp_pad(int k) const; //adds clamp padding of width k

	/*
	//pad uniformly until image is desired size (keep original image centered) <- maybe implement this later if needed
	Image const_pad_to_size(int width, int height, Color col = { 0,0,0,0 }) const; 
	Image clamp_pad_to_size(int width, int height) const;
	*/

	static Image blend(const Image& im1, const Image& im2, double alpha);
	
	//image stats
	std::vector<ImgHistEntry> histogram() const;
	std::vector<ImgHistEntry> cum_dist() const; //normalized cumulative distribution function



	//filter
	Image copy() const; //returns identical copy

	Image hist_equalize() const; //histogram equalization
	Image lin_filter(const Matrix<double>& filter) const; //clamp pads image, then applies linear filter - filter must be square matrix of odd size
	
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
	Image sharpen(double alpha = 0.25f) const;
	Image median(int radius) const; //median filter with given radius (slow)
	Image bilateral(int radius, double sigma_d, double sigma_r) const;
	Image iter_bilateral(unsigned int iter, int radius, double sigma_d, double sigma_r) const;

	//monochromatize
	Image max_monochrome() const; //makes black and white copy -> takes the max of the R,G,B channels
	Image mean_monochrome() const; //makes black and white copy, taking the mean of each channel
	Image grayscale() const; //makes black and white copy using proper weighting to account for luminance

	//binary image processing 
	Image binarize(double threshold) const; //turn grayscale images into binary with given threshold (between 0 and 1)

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

	//interpolation
	Color bicubic_interp(double x, double y, double a = -1.0f) const;

	//scaling
	Image bicubic_upscale(int rate, double a = -1.0f) const; //scale up image by factor of rate (bicubic interpolation)
	Image bicubic_decimate(int rate, double a = -1.0f) const; //scale down image by factor of rate


	//transformations (inverse warping)
	Image affine_transform(const Matrix<double>& trans) const; //3 x 3 affine transform matrix (hom. coordinates) - implement inverse warping with bicubic interp
	Image translate(double dx, double dy) const;
	Image rotate(double angle) const; //rotates around CENTER of image
	Image scale(double sx, double sy) const; //places origin at CENTER of image for scaling
	


};

Image operator+(const Image& im1, const Image& im2);
Image operator-(const Image& im1, const Image& im2);
Image operator*(double c, const Image& img);



inline Image operator*(const Matrix<double>& filter, const Image& img) //convolve filter with image
{
	return img.lin_filter(filter);
}