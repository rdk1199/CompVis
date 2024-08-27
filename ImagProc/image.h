

#include <string>
#include <vector>



struct Color
{
	float r = 0, g = 0, b = 0, a = 0;
};


class Image
{
private:
	
	std::vector<std::vector<Color>> pixels;

	int width;
	int height;

public:

	// I/O
	Image(std::string file); //load image from file
	void save(std::string file); //save image to disk



};