#pragma once

#include <string>
#include <vector>
#include <cmath>

struct Color
{
	float r = 0, g = 0, b = 0, a = 0;
};


inline Color operator+(const Color& a, const Color& b)
{
	return { a.r + b.r, a.g + b.g, a.b + b.b, std::max(a.a, b.a) };
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
	return { a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a };
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

struct ImgHistEntry
{
	int r = 0, g = 0, b = 0, a = 0;
};

class Image
{
private:

	std::vector<std::vector<Color>> pixels;

	int _width;
	int _height;

	std::vector<Color>& operator[](int i) { return pixels[i]; }



public:

	// I/O
	Image(std::string file); //load image from file
	void save(std::string file); //save image to disk

	//constructors
	Image(const std::vector<std::vector<Color>>& px); //create image from bitmap

	//properties
	int width() const { return _width; }
	int height() const { return _height; }

	//accessors
	Color at(int i, int j) const { return pixels[i][j]; }



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
	//pad uniformly until image is desired size
	Image const_pad_to_size(int width, int height, Color col = { 0,0,0,0 }) const; 
	Image clamp_pad_to_size(int width, int height) const;
	*/

	static Image blend(const Image& im1, const Image& im2, float alpha);


};

Image operator+(const Image& im1, const Image& im2);
Image operator*(float c, const Image& img);
